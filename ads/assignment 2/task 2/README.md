# Hybrid Database Setup – PostgreSQL and MongoDB

## Overview

This project demonstrates the setup and population of a hybrid database system using PostgreSQL and MongoDB. PostgreSQL is used to store structured transactional data such as users, products, and orders, while MongoDB is used to store flexible semi-structured data such as product details, reviews, and user activity logs. Both databases are deployed using Docker containers to simplify installation and ensure a consistent environment.

---

# Prerequisites

Before starting the setup, the following software should be installed:

* Docker
* Visual Studio Code

---

# Install Docker (Ubuntu)

```bash
sudo apt update
sudo apt install docker.io -y
sudo systemctl start docker
sudo systemctl enable docker
```

Verify Docker installation:

```bash
docker --version
```

---

# Step 1 – Start PostgreSQL Container

Run the following command to start the PostgreSQL container:

```bash
docker run --name postgres-db \
-e POSTGRES_USER=admin \
-e POSTGRES_PASSWORD=admin123 \
-e POSTGRES_DB=ecommerce_db \
-p 5432:5432 \
-d postgres
```

This command:

* Creates a container named **postgres-db**
* Sets the database username and password
* Creates a database named **ecommerce_db**
* Exposes PostgreSQL on port **5432**

---

# Step 2 – Start MongoDB Container

Run the following command:

```bash
docker run --name mongodb \
-p 27017:27017 \
-d mongo
```

This command:

* Creates a MongoDB container named **mongodb**
* Exposes the default MongoDB port **27017**

---

# Step 3 – Verify Running Containers

Check whether both containers are running:

```bash
docker ps
```

Expected containers:

* postgres-db
* mongodb

---

# Step 4 – Create PostgreSQL Tables

The relational database schema is defined in `ddl.sql`.

Tables created:

* users
* products
* orders
* order_items

Execute the script:

```bash
docker exec -i postgres-db psql -U admin -d ecommerce_db < ddl.sql
```

---

# Step 5 – Populate PostgreSQL Tables

The `populate_postgres.sql` script inserts sample data.

Data generated:

* 20 users
* 50 products
* 20 orders
* order_items records

Run the script:

```bash
docker exec -i postgres-db psql -U admin -d ecommerce_db < populate_postgres.sql
```

---

# Step 6 – Verify PostgreSQL Data

Enter the PostgreSQL shell:

```bash
docker exec -it postgres-db psql -U admin -d ecommerce_db
```

Run the following queries:

```sql
SELECT * FROM users;
SELECT * FROM products;
SELECT * FROM orders;
SELECT * FROM order_items;
```

These queries display the populated relational tables.

Exit PostgreSQL:

```
\q
```

---

# Step 7 – Populate MongoDB Collections

MongoDB stores semi-structured data using documents.

Enter MongoDB shell:

```bash
docker exec -it mongodb mongosh
```

Create database:

```javascript
use ecommerce_db
```

Collections created:

* product_details
* user_activity

Run the script to insert sample documents:

```bash
docker exec -i mongodb mongosh ecommerce_db < populate_mongo.js
```

---

# Step 8 – Verify MongoDB Data

Enter MongoDB shell:

```bash
docker exec -it mongodb mongosh
```

Switch to the database:

```javascript
use ecommerce_db
```

Show collections:

```javascript
show collections
```

View inserted documents:

```javascript
db.product_details.find().pretty()
db.user_activity.find().pretty()
```

Exit MongoDB:

```javascript
exit
```

---

# Hybrid Database Architecture

This project demonstrates a hybrid database architecture combining relational and document-based databases.

```
Application Layer
      |
 -------------------------
 |                       |
PostgreSQL           MongoDB
(users, orders)   (product_details, activity_logs)
```

### PostgreSQL

PostgreSQL is used for structured transactional data such as:

* users
* products
* orders
* order_items

It enforces relationships using **primary keys and foreign keys**, ensuring strong data consistency.

### MongoDB

MongoDB is used for flexible document-based data such as:

* product descriptions
* product images
* product attributes
* customer reviews
* user activity logs

The document model allows nested objects and arrays, making it suitable for semi-structured information.

---

# Conclusion

This project successfully demonstrates the setup of PostgreSQL and MongoDB using Docker containers and the population of relational tables and document collections with sample data.

The hybrid database approach allows efficient management of both structured transactional data and flexible semi-structured data within the same system, combining the advantages of relational and NoSQL databases.

