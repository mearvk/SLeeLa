# Show every row.
SELECT * FROM contacts

# Project selected columns.
SELECT name, email FROM contacts

# Filter by equality.
SELECT id, name FROM contacts WHERE id = 2

# Count all rows or only rows matching a predicate.
SELECT COUNT(*) FROM contacts
SELECT COUNT(*) FROM contacts WHERE email = 'grace@example.test'
