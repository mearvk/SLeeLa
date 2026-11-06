# MODERATE: read data back with projection, a single equality filter, and a count.
# The SELECT subset supports `SELECT * FROM t`, `SELECT * FROM t WHERE c = v`,
# and `SELECT COUNT(*) FROM t [WHERE c = v]`. Exactly ONE equality predicate is
# allowed -- no AND/OR, no ranges, no ordering.
CREATE TABLE IF NOT EXISTS orders (id, customer, status)
INSERT INTO orders VALUES (10, "Ada", "shipped")
INSERT INTO orders VALUES (11, "Linus", "pending")
INSERT INTO orders VALUES (12, "Ada", "pending")

# All rows:
SELECT * FROM orders
# Only Ada's orders (single equality predicate):
SELECT * FROM orders WHERE customer = "Ada"
# How many are still pending:
SELECT COUNT(*) FROM orders WHERE status = "pending"
