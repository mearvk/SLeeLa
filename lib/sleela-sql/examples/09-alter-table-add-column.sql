# Standalone example: add a column; old rows receive an empty string.
CREATE TABLE contacts_phone (id, name)
INSERT INTO contacts_phone VALUES (1, 'Ada')
ALTER TABLE contacts_phone ADD COLUMN phone
SELECT * FROM contacts_phone
