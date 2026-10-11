# Change one field on matching rows.
UPDATE contacts SET email = 'ada@history.example.test' WHERE id = 1

# Update multiple fields in one statement.
UPDATE contacts SET name = 'Jane Doe', email = 'jane.doe@example.test' WHERE id = 4

# Verify the result.
SELECT * FROM contacts WHERE id = 1
SELECT * FROM contacts WHERE id = 4
