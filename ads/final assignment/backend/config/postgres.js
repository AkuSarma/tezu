const { Pool } = require("pg");

const pool = new Pool({
  host: process.env.PG_HOST,
  user: process.env.PG_USER,
  password: process.env.PG_PASSWORD,
  database: process.env.PG_DB,
  port: 5432,
});

const connectPostgres = async () => {
  let retries = 5;

  while (retries) {
    try {
      const res = await pool.query("SELECT NOW()");
      console.log("PostgreSQL connected:", res.rows[0]);
      break;
    } catch (err) {
      console.log("Retrying PostgreSQL connection...");
      retries -= 1;
      await new Promise((res) => setTimeout(res, 5000));
    }
  }
};

module.exports = connectPostgres;
