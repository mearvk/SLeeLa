# Delete only rows matching the equality predicate.
DELETE FROM contacts WHERE id = 3

# Check how many rows remain.
SELECT COUNT(*) FROM contacts

# Delete all rows by omitting WHERE. Use carefully.
# DELETE FROM contacts
