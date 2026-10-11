# SLeeLaSQL — the fluent dialect

**SLeeLaSQL** is a clean, method-chaining surface language for the `sleela-sql`
engine. It is a *second front-end* to the engine, not a second engine: every
SLeeLaSQL statement compiles to the **same internal operation** as its classic
SQL counterpart and runs on the **same executor**, so the two dialects are
feature-complete with each other and produce byte-identical results.

It reads the way SLeeLa code reads — a root, then dotted verbs — so a developer
already fluent in SLeeLa can drive the database without switching mental models.

```text
                 SQL  ──┐
                        ├──►  one compiled statement  ──►  one executor
           SLeeLaSQL  ──┘        (ssql_stmt IR)
```

## Why a second dialect

- **Completeness.** Anything expressible in the SQL subset is expressible in
  SLeeLaSQL and vice-versa — same five operations, same `WHERE`, same `?`
  placeholders.
- **A clean way to speed things up.** Because both dialects lower to the same
  compiled `ssql_stmt`, either can be **prepared once and executed many times**
  with bound values — the PreparedStatement win — with no dialect penalty.
- **Ergonomics.** The fluent form keeps the table name at the front and the verb
  next to its arguments, which reads naturally and composes cleanly.

## Grammar

A statement is a **root** that names the table, followed by a dotted **verb**
(and, for `select`, an optional `.where(...)`). `?` is a positional placeholder
in both dialects. Values may be bare, single-quoted, or double-quoted.

| Operation | SLeeLaSQL | Classic SQL |
|---|---|---|
| Create | `table("t").create(c1, c2, ...)` | `CREATE TABLE t (c1, c2, ...)` |
| Drop | `from("t").drop()` | `DROP TABLE t` |
| Drop if exists | `from("t").drop(ifExists)` | `DROP TABLE IF EXISTS t` |
| Insert | `into("t").insert(v1, v2, ...)` | `INSERT INTO t VALUES (v1, v2, ...)` |
| Select all | `from("t").select(*)` | `SELECT * FROM t` |
| Select cols | `from("t").select(c1, c2)` | `SELECT c1, c2 FROM t` |
| Filter | `from("t").select(*).where(c == v)` | `SELECT * FROM t WHERE c = v` |
| List tables | `tables()` | `SHOW TABLES` |

Notes:
- The three roots are `table(...)` (for `create`), `into(...)` (for `insert`),
  and `from(...)` (for `select` / `drop`); `tables()` stands alone.
- In `.where(...)`, both `==` and `=` are accepted (`==` reads more like SLeeLa).
- `insert` is positional: the value count must equal the table's column count.
- A table name may be quoted (`from('games')`) or bare (`from(games)`).

## Prepared statements (both dialects)

Compile once, bind, run — repeatedly. The `?` placeholders are 1-based, as in
JDBC/SQLite.

```c
ssql_stmt *st = NULL;
ssql_prepare(&db, "into('games').insert(?, ?, ?)", SSQL_DIALECT_SLEELA, &st);

for (int i = 0; i < n; i++) {
    ssql_reset(st);
    ssql_bind(st, 1, ids[i]);
    ssql_bind(st, 2, titles[i]);
    ssql_bind(st, 3, years[i]);
    ssql_run(st, NULL);          /* parsed once; only bind+execute per row */
}
ssql_finalize(st);
```

The identical pattern works for classic SQL — just prepare
`"INSERT INTO games VALUES (?, ?, ?)"` with `SSQL_DIALECT_SQL` (or `AUTO`).

## Dialect selection

- **API:** pass `SSQL_DIALECT_SQL`, `SSQL_DIALECT_SLEELA`, or `SSQL_DIALECT_AUTO`
  to `ssql_prepare` / `ssql_exec_dialect`. `ssql_exec` uses `AUTO`.
- **AUTO sniffing:** a statement whose first identifier is `table`, `into`,
  `from`, or `tables` immediately followed by `(` is treated as SLeeLaSQL;
  everything else is SQL. The two keyword sets do not collide, so a mixed stream
  (some SQL lines, some SLeeLaSQL lines) auto-detects per statement.
- **CLI:** `--sql` / `--sleela` force a dialect; omit them to sniff.

## Worked example

```sh
# identical effect, two dialects
sleela-sql ./data "SELECT title, year FROM games WHERE year = '1986'"
sleela-sql ./data "from('games').select(title, year).where(year == '1986')"
```

Both print:

```
title,year
Metroid,1986
```

See [`../samples/demo.ssql`](../samples/demo.ssql) for a full SLeeLaSQL session.
