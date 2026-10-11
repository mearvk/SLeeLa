# SLSQL and classic SQL: side-by-side

SLSQL is an alternative syntax for the supported subset; it does not create a separate execution engine.

| Intent | SLSQL | Classic SQL |
|---|---|---|
| Create | `table('games').create(id, title, year)` | `CREATE TABLE games (id, title, year)` |
| Insert | `into('games').insert(1, 'Metroid', 1986)` | `INSERT INTO games VALUES (1, 'Metroid', 1986)` |
| Select all | `from('games').select(*)` | `SELECT * FROM games` |
| Select columns | `from('games').select(title, year)` | `SELECT title, year FROM games` |
| Filter | `from('games').select(*).where(year == 1986)` | `SELECT * FROM games WHERE year = 1986` |
| Count | `from('games').select(count(*))` | `SELECT COUNT(*) FROM games` |
| List tables | `tables()` | `SHOW TABLES` |
| Drop | `from('games').drop()` | `DROP TABLE games` |
| Drop if present | `from('games').drop(ifExists)` | `DROP TABLE IF EXISTS games` |

## One engine, two front ends

Both dialects lower to the same internal statement representation and executor. The parser can auto-detect SLSQL roots (`table(`, `into(`, `from(`, `tables(`), or callers can explicitly select the dialect with the API or CLI.

## Scope warning

Do not assume all classic SQL operations have fluent SLSQL forms. In the current implementation, `ALTER TABLE ADD COLUMN`, `UPDATE`, and `DELETE` are classic-SQL-only. See `../../SPECIFICATION.md` for limits and persistence behavior.
