# lib/db-connector — the SLeeLa DB connector

The SLeeLa equivalent of **JDBC plus a driver**. Java splits database access
into an API (`java.sql.*`) and pluggable **drivers** shipped as jars (e.g. the
PostgreSQL connector jar). SLeeLa has the same split, and this package is the
missing middle layer:

| Java | SLeeLa |
|---|---|
| `java.sql.*` API | `lib/database/*.sleela` facade classes |
| JDBC **driver** (the connector jar) | **this package (`sldbc_*`)** |
| the database | `lib/sleela-sql` engine (or, later, an external RDBMS) |

## What it is

A small, driver-agnostic C ABI (`include/sldbc.h`) with the JDBC lifecycle:

```c
sldbc_conn *c;
sldbc_open("sleelasql:./data", &c);          /* DriverManager.getConnection */

sldbc_stmt *s;
sldbc_prepare(c, "INSERT INTO person VALUES (?, ?)", &s);
sldbc_bind(s, 1, "1");                        /* PreparedStatement.setString */
sldbc_bind(s, 2, "Ada");
sldbc_execute(s);                             /* executeUpdate */
sldbc_finalize(s);

sldbc_prepare(c, "SELECT id, name FROM person", &s);
sldbc_rows *r;
sldbc_query(s, &r);                           /* executeQuery -> ResultSet */
for (size_t i = 0; i < sldbc_rows_count(r); ++i)
    printf("%s\n", sldbc_rows_value_named(r, i, "name"));
sldbc_rows_free(r);
sldbc_finalize(s);

sldbc_begin(c); /* ... */ sldbc_rollback(c);  /* setAutoCommit(false)/rollback */
sldbc_close(c);
```

## Drivers

The connection **URL scheme** selects the driver, exactly like a JDBC URL:

- **`sleelasql:<dir>`** — the one real, bundled driver. Wraps the local
  `lib/sleela-sql` engine (`ssql_*`): both SQL and fluent SLeeLaSQL dialects,
  positional `?` binds, result sets, and the engine's snapshot transactions
  (`begin`/`commit`/`rollback`).
- **`postgresql:` / `mysql:` / `mariadb:` / `sqlite:` / `sqlserver:` /
  `oracle:` / `odbc:`** — known external backends (the kinds named in
  `api/database`). These are **honest stubs**: `sldbc_open` returns
  `SLDBC_UNSUPPORTED` rather than pretending to connect. Add a real driver to
  light one up.
- Any other scheme → `SLDBC_ERR_NODRIVER`.

This keeps the connector truthful: it only claims the backend it can actually
reach today, while providing the full API shape for the rest.

## Build and test

```sh
cd lib/db-connector
make          # -> build/libsldbc.a (bundles the sleela-sql engine core)
make test     # full JDBC-style lifecycle + honest-stub behaviour
```

The static archive bundles the engine core, so a consumer links one library:

```sh
cc myapp.c -I lib/db-connector/include lib/db-connector/build/libsldbc.a -o myapp
```

## Honest limits

- The only executing backend is `sleelasql` (a local CSV-backed SQL subset);
  external-RDBMS drivers are not implemented.
- Transactions inherit the `lib/sleela-sql` guarantees: single-connection
  snapshot rollback, **not** full ACID / crash recovery / multi-writer
  isolation yet (see `lib/sleela-sql/ROADMAP.md`).
- This is the C connector. Wiring the `lib/database/*.sleela` facade classes to
  call through this ABI (so SLeeLa programs use it directly) is the natural
  next step and is not done here.
