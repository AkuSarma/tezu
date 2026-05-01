const express = require("express");
const connectPostgres = require("./config/postgres");
const connectMongo = require("./config/mongo");

const app = express();
app.use(express.json());

connectPostgres();
connectMongo();

app.get("/", (req, res) => {
  res.send("Hybrid DB Backend Running 🚀");
});

app.get("/test", (req, res) => {
  res.json({ message: "API working!" });
});

const productRoutes = require("./routes/product");
app.use("/api", productRoutes);

app.listen(5000, () => {
  console.log("Server running on port 5000");
});

