# Standalone negative test: the first ALTER succeeds; the second should fail.
CREATE TABLE contacts_duplicate (id, name)
INSERT INTO contacts_duplicate VALUES (1, 'Ada')
ALTER TABLE contacts_duplicate ADD COLUMN status DEFAULT 'active'
ALTER TABLE contacts_duplicate ADD COLUMN status
