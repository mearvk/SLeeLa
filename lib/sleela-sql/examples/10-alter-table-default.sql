# Add a column and initialize existing rows with the default value.
ALTER TABLE contacts ADD COLUMN status DEFAULT 'active'
SELECT * FROM contacts
