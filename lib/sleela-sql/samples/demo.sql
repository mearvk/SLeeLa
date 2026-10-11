# demo.sql -- a worked session for sleela-sql.
# Feed it on stdin:   sleela-sql ./data < samples/demo.sql
# Each non-comment line is one statement (trailing ';' optional).

CREATE TABLE games (id, title, year)

INSERT INTO games VALUES (1, 'Metroid', 1986)
INSERT INTO games VALUES (2, 'Mega Man', 1987)
INSERT INTO games VALUES (3, 'Castlevania, Deluxe', 1986)

# all columns
SELECT * FROM games

# projection + equality filter
SELECT title, year FROM games WHERE year = '1986'

# list tables
SHOW TABLES
