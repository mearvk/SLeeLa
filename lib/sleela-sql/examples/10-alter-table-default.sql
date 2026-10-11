# Standalone example: existing rows receive the declared default.
CREATE TABLE contacts_status (id, name)
INSERT INTO contacts_status VALUES (1, 'Ada')
ALTER TABLE contacts_status ADD COLUMN status DEFAULT 'active'
SELECT * FROM contacts_status
