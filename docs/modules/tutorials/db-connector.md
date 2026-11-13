<img align="right" src="../../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# db-connector: Tutorial

| | |
|---|---|
| Nature | datasource (`bin/sleelactl help db-connector`) |
| Registry summary | Native database connector driver |
| Lives in | `lib/db-connector/` (`include/sldbc.h`, `src/sldbc.c`, `tests/test_connector.c`, `Makefile`) |
| Writeup type | Tutorial, because it is a C library you build and program against, and I built it, ran its tests and ran a small program of my own against it. |
| Verified | 2026-11-13 against `master` e048707fd: `make` and `make test` in `lib/db-connector` end with `all db-connector tests passed`; a 20-line C program created a table, inserted and queried a row. |

## What it is

`db-connector` is a JDBC-style C API (`sldbc_*`) with a driver manager. The connection URL scheme selects the driver. One driver really executes: `sleelasql:<dir>`, which wraps the CSV-backed engine in [`lib/sleela-sql`](sleela-sql.md). Other schemes (`postgresql:`, `mysql:`, `mariadb:`, `sqlite:`, `sqlserver:`, `oracle:`, `odbc:`) are stubs that return `SLDBC_UNSUPPORTED`; anything else returns `SLDBC_ERR_NODRIVER` ([`README.md`](../../../lib/db-connector/README.md)).

## Before you start

You need `cc` and `ar`. The Makefile compiles `../sleela-sql/native/src/sleela_sql.c` too, so keep that directory in place. Build outputs go to `lib/db-connector/build/`, which `.gitignore` excludes (`**/build/`).

`sleelactl` only reports on this module. These all exit 0 and only print guidance (for example `db-connector is an open/close/active data surface (SLDataSource)` and `✓ db-connector library present at lib/db-connector`):

```sh
cd SLeeLa            # the repository root
timeout 60 bin/sleelactl available db-connector
```

The real work is below.

## Step 1 - Build the library

```sh
cd lib/db-connector
timeout 300 make
```

```text
mkdir -p build
cc -I include -I ../sleela-sql/native/include -std=c11 -O2 -Wall -Wextra -Wpedantic -c src/sldbc.c -o build/sldbc.o
cc -I ../sleela-sql/native/include -std=c11 -O2 -Wall -Wextra -Wpedantic -c ../sleela-sql/native/src/sleela_sql.c -o build/sleela_sql.o
ar rcs build/libsldbc.a build/sldbc.o build/sleela_sql.o
```

No warnings. The archive bundles the SQL engine, so you link only `libsldbc.a`.

## Step 2 - Run the test program

```sh
timeout 300 make test
```

```text
rm -rf build/test-data && mkdir -p build/test-data
build/test-connector build/test-data
sldbc version 1.0.0
ok:   sleelasql driver available
ok:   postgresql is a stub (not available)
ok:   open sleelasql connection
ok:   driver name is sleelasql
ok:   external backend refused (UNSUPPORTED)
...
ok:   rollback undid the insert (count=2)

all db-connector tests passed
```

Every line starts `ok:`. The test ([`tests/test_connector.c`](../../../lib/db-connector/tests/test_connector.c)) covers open, `CREATE`, parameterised `INSERT`, `SELECT`, out-of-range access, and `begin`/`rollback`.

## Step 3 - Write your own program

Save this as `/tmp/sldbc-app/app.c`:

```c
#include <stdio.h>
#include "sldbc.h"
int main(void) {
  sldbc_conn *c; sldbc_stmt *s; sldbc_rows *r;
  if (sldbc_open("sleelasql:./dbdata", &c) != SLDBC_OK) return 1;
  printf("driver=%s version=%s\n", sldbc_driver_name(c), sldbc_version());
  sldbc_prepare(c, "CREATE TABLE person (id, name)", &s);
  printf("create: %s\n", sldbc_strerror(sldbc_execute(s))); sldbc_finalize(s);
  sldbc_prepare(c, "INSERT INTO person VALUES (?, ?)", &s);
  sldbc_bind(s, 1, "1"); sldbc_bind(s, 2, "Ada");
  printf("insert: %s\n", sldbc_strerror(sldbc_execute(s))); sldbc_finalize(s);
  sldbc_prepare(c, "SELECT id, name FROM person", &s);
  sldbc_query(s, &r);
  for (size_t i = 0; i < sldbc_rows_count(r); ++i)
    printf("row %zu: %s\n", i, sldbc_rows_value_named(r, i, "name"));
  sldbc_rows_free(r); sldbc_finalize(s);
  sldbc_conn *pg = NULL;
  printf("postgresql: %s\n", sldbc_strerror(sldbc_open("postgresql:localhost/db", &pg)));
  sldbc_close(c); return 0;
}
```

Build and run it from its own directory (the database directory `./dbdata` must have an existing parent; the engine creates the last component, and I created it up front):

```sh
mkdir -p /tmp/sldbc-app/dbdata && cd /tmp/sldbc-app
cc -std=c11 -I $SLEELA_HOME/lib/db-connector/include app.c \
   $SLEELA_HOME/lib/db-connector/build/libsldbc.a -o app
./app
ls dbdata
```

```text
driver=sleelasql version=1.0.0
create: ok
insert: ok
row 0: Ada
postgresql: driver not yet implemented
person.csv
```

The table is one CSV file. The last line of program output shows the honest stub: asking for PostgreSQL does not fake a connection.

Clean up: `rm -rf /tmp/sldbc-app`.

## What you did

You built the connector, ran its lifecycle tests, and used the API yourself: open, prepare, bind, execute, query, close, plus the refusal of an external backend.

## Next steps

- [`lib/db-connector/README.md`](../../../lib/db-connector/README.md): the full API tour and the "Honest limits" section.
- [`lib/db-connector/include/sldbc.h`](../../../lib/db-connector/include/sldbc.h): status codes (`SLDBC_OK` 0 to `SLDBC_ERR_OOM` -8) and every function.
- [sleela-sql](sleela-sql.md): the engine's own CLI and SQL subset.
- Limits to remember: the SLeeLa classes in [`lib/database`](../explanations/database.md) do not call this API (README: "not done here"), and transactions are snapshot-only, not ACID.

## Sources

Files read: `bin/sleelactl.d/registry.conf` (line 41), `lib/db-connector/README.md`, `Makefile`, `include/sldbc.h`, `tests/test_connector.c` (head), `lib/sleela-sql/ROADMAP.md`.

Commands run (with `timeout`): `bin/sleelactl help|available|status|open|close db-connector`; `make` and `make test` in `lib/db-connector`; the Step 3 `cc` build and run (done in a scratch directory outside the repository). Built here: `lib/db-connector/build/` (git-ignored).
