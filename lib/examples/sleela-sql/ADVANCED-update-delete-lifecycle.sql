# ADVANCED: the full row lifecycle -- insert, update, verify, delete, verify.
# UPDATE and DELETE each take at most ONE equality predicate in the WHERE clause;
# omitting WHERE affects every row. Each UPDATE/DELETE rewrites the CSV via a
# temporary file (not a crash-safe transaction -- see SPECIFICATION.md).
CREATE TABLE IF NOT EXISTS tickets (id, owner, state)
INSERT INTO tickets VALUES (100, "Ada", "open")
INSERT INTO tickets VALUES (101, "Linus", "open")
INSERT INTO tickets VALUES (102, "Grace", "open")

# Promote one ticket (single equality predicate on id):
UPDATE tickets SET state = "in-progress" WHERE id = 101

# Verify the change and count the still-open tickets:
SELECT * FROM tickets WHERE id = 101
SELECT COUNT(*) FROM tickets WHERE state = "open"

# Close out a ticket by removing it:
DELETE FROM tickets WHERE id = 100

# Final state of the table:
SELECT * FROM tickets
SELECT COUNT(*) FROM tickets
