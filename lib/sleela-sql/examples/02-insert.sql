# Add rows to the contacts table created by 01-create-table.sql.
INSERT INTO contacts VALUES (1, 'Ada Lovelace', 'ada@example.test')
INSERT INTO contacts VALUES (2, 'Grace Hopper', 'grace@example.test')
INSERT INTO contacts VALUES (3, 'Linus Torvalds', 'linus@example.test')

# Commas inside quoted values are supported.
INSERT INTO contacts VALUES (4, 'Doe, Jane', 'jane@example.test')
