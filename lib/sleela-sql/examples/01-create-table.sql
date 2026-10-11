# Create a new table.
CREATE TABLE contacts (id, name, email)

# Idempotent creation: leaves the existing table unchanged if it exists.
CREATE TABLE IF NOT EXISTS contacts (id, name, email)
