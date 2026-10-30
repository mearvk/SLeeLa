# BASIC: create a table and add rows.
# SLeeLa SQL is a small CSV-backed subset. CREATE TABLE writes t.csv with a
# header; INSERT appends one positional row at a time (values line up with the
# column order declared in CREATE TABLE).
CREATE TABLE contacts (id, name, email)
INSERT INTO contacts VALUES (1, "Ada", "ada@example.test")
INSERT INTO contacts VALUES (2, "Linus", "linus@example.test")
INSERT INTO contacts VALUES (3, "Grace", "grace@example.test")
