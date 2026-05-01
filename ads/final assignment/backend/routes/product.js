const express = require("express");
const router = express.Router();
const pool = require("../config/postgres");
const mongoose = require("mongoose");

const ProductDetails = mongoose.model(
  "ProductDetails",
  new mongoose.Schema({}, { strict: false }),
);

router.post("/add-product", async (req, res) => {
  const { name, price, stock, category, description } = req.body;

  try {
    // Insert into PostgreSQL
    const result = await pool.query(
      "INSERT INTO products (name, price, stock_quantity, category) VALUES ($1,$2,$3,$4) RETURNING id",
      [name, price, stock, category],
    );

    const productId = result.rows[0].id;

    // Insert into MongoDB
    await ProductDetails.create({
      product_id: productId,
      description,
      images: [],
      attributes: {},
      reviews: [],
    });

    res.json({ message: "Product added", productId });
  } catch (err) {
    res.status(500).json(err.message);
  }
});

router.post("/place-order", async (req, res) => {
  const client = await pool.connect();

  try {
    const { user_id, items } = req.body;

    await client.query("BEGIN");

    let total = 0;

    for (let item of items) {
      const product = await client.query("SELECT * FROM products WHERE id=$1", [
        item.product_id,
      ]);

      if (product.rows[0].stock_quantity < item.quantity) {
        throw new Error("Not enough stock");
      }

      total += product.rows[0].price * item.quantity;

      await client.query(
        "UPDATE products SET stock_quantity = stock_quantity - $1 WHERE id=$2",
        [item.quantity, item.product_id],
      );
    }

    const order = await client.query(
      "INSERT INTO orders (user_id, total_amount, status) VALUES ($1,$2,$3) RETURNING id",
      [user_id, total, "placed"],
    );

    for (let item of items) {
      await client.query("INSERT INTO order_items VALUES ($1,$2,$3,$4)", [
        order.rows[0].id,
        item.product_id,
        item.quantity,
        item.price,
      ]);
    }

    await client.query("COMMIT");

    res.json({ message: "Order placed successfully" });
  } catch (err) {
    await client.query("ROLLBACK");
    res.status(500).json(err.message);
  } finally {
    client.release();
  }
});

router.get("/product/:id", async (req, res) => {
  const id = req.params.id;

  const sqlData = await pool.query("SELECT * FROM products WHERE id=$1", [id]);
  const mongoData = await ProductDetails.findOne({ product_id: parseInt(id) });

  res.json({
    sql: sqlData.rows[0],
    mongo: mongoData,
  });
});

router.post("/add-review", async (req, res) => {
  const { product_id, user_id, rating, comment } = req.body;

  await ProductDetails.updateOne(
    { product_id },
    {
      $push: {
        reviews: { user_id, rating, comment, timestamp: new Date() },
      },
    },
  );

  res.json({ message: "Review added" });
});

router.get("/user/:id/orders", async (req, res) => {
  const id = req.params.id;

  const result = await pool.query(
    `
    SELECT o.id, o.total_amount, oi.product_id, oi.quantity
    FROM orders o
    JOIN order_items oi ON o.id = oi.order_id
    WHERE o.user_id = $1
  `,
    [id],
  );

  res.json(result.rows);
});

module.exports = router;
