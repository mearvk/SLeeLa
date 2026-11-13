<img align="right" src="../../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# sleela-sql: Tutorial

| | |
|---|---|
| Nature | datasource (`bin/sleelactl help sleela-sql`) |
| Registry summary | SLeeLa SQL dialect/engine |
| Lives in | `lib/sleela-sql/` (`native/` C11 engine and CLI, `cpp/` C++17 facade, `SqlModel.sleela`, `SPECIFICATION.md`, `ROADMAP.md`, `examples/`, `samples/`) |
| Writeup type | Tutorial, because the engine has a command-line program you build and use, and I ran every step below. |
| Verified | 2026-11-13 against `master` e048707fd: `make` and `make test` in `lib/sleela-sql` succeed; the CLI ran both sample sessions and further statements with the outputs shown. One part of the module (`SqlModel.sleela`) does not run as shipped; see the end. |

## What it is

`sleela-sql` is a small, local, CSV-backed SQL engine in C11 (one `.csv` file per table) with a C++17 wrapper and a command-line tool. It speaks two dialects that compile to the same internal statement: classic SQL and a fluent form called SLeeLaSQL, for example `from('games').select(title, year).where(year == '1986')` ([`README.md`](../../../lib/sleela-sql/README.md), [`docs/SLEELASQL.md`](../../../lib/sleela-sql/docs/SLEELASQL.md)). [`SPECIFICATION.md`](../../../lib/sleela-sql/SPECIFICATION.md) calls it "a bounded local-file SQL engine, not a general-purpose SQL server".

## Before you start

You need `cc`, `c++` (g++ worked) and `ar`. Build outputs go to `lib/sleela-sql/build/`, git-ignored (`**/build/`). `sleelactl` only reports on this module (`bin/sleelactl available sleela-sql` prints `✓ sleela-sql library present at lib/sleela-sql` and exits 0).

## Step 1 - Build and test

```sh
cd lib/sleela-sql
timeout 300 make
timeout 300 make test
```

`make` builds `build/sleela-sql` (the CLI), `build/libsleelasql.a` and `build/libsleelasql++.a`. `make test` builds and runs three tests (C, C++ and transactions). The C and C++ tests print nothing on success. The last lines are:

```text
build/test-txn build/test-data
all sleela-sql transaction tests passed
```

The build gave one compiler warning, in the test and not in the engine: `tests/test_txn.c:25: warning: implicit declaration of function 'strtol'` (the file lacks `#include <stdlib.h>`). It did not stop the build.

## Step 2 - Run the classic SQL sample

The CLI reads one statement per line on stdin (`#` lines are comments) and prints rows as CSV.

```sh
mkdir -p /tmp/sleela-sql-demo
build/sleela-sql /tmp/sleela-sql-demo < samples/demo.sql
```

```text
id,title,year
1,Metroid,1986
2,Mega Man,1987
3,"Castlevania, Deluxe",1986
title,year
Metroid,1986
"Castlevania, Deluxe",1986
Tables_in_sleela_sql
games
```

The values are strings; quoting appears only for the value with a comma. `cat /tmp/sleela-sql-demo/games.csv` shows the same rows as plain CSV on disk.

## Step 3 - Use the fluent dialect and more statements

```sh
build/sleela-sql /tmp/sleela-sql-demo --sleela "from('games').select(title, year).where(year == '1986')"
build/sleela-sql /tmp/sleela-sql-demo "UPDATE games SET year = '1990' WHERE id = '1'"
build/sleela-sql /tmp/sleela-sql-demo "SELECT COUNT(*) FROM games WHERE year = '1990'"
build/sleela-sql /tmp/sleela-sql-demo "ALTER TABLE games ADD COLUMN rating DEFAULT 5"
build/sleela-sql /tmp/sleela-sql-demo "SELECT * FROM games"
```

```text
title,year
Metroid,1986
"Castlevania, Deluxe",1986
COUNT(*)
1
id,title,year,rating
1,Metroid,1990,5
2,Mega Man,1987,5
3,"Castlevania, Deluxe",1986,5
```

(`UPDATE` and `ALTER` print nothing on success.) The fluent sample also works against a fresh directory: `build/sleela-sql <dir> --sleela < samples/demo.ssql` printed the same three tables of rows as the SQL sample.

## Step 4 - See the refusals

```sh
build/sleela-sql /tmp/sleela-sql-demo "CREATE TABLE games (id)"
build/sleela-sql /tmp/sleela-sql-demo "ALTER TABLE games ADD COLUMN year"
build/sleela-sql /tmp/sleela-sql-demo "SELECT * FROM nothere"
```

```text
sleela-sql: table already exists
sleela-sql: column already exists
sleela-sql: no such table
```

Each exits with code 1. Clean up with `rm -rf /tmp/sleela-sql-demo`.

## What you did

You built the engine, ran its test suite, drove it from the CLI in both dialects, and used `UPDATE`, `DELETE`, `COUNT(*)` and `ALTER TABLE ADD COLUMN`. More runnable files are in [`examples/`](../../../lib/sleela-sql/examples/README.md) and [`examples/slsql/`](../../../lib/sleela-sql/examples/slsql/README.md).

## Limits and problems found

- **`SqlModel.sleela` does not run.** The README and the file header say `sleela run lib/sleela-sql/SqlModel.sleela`. With the current binary it fails:

  ```text
  sleelvac: lib/sleela-sql/SqlModel.sleela: Syntax error (line 36, col 22): expected ';' but found '='
  ```

  Line 36 is `String metaVersion = "1.0.0";`, inside `struct Schema`. The generated metadata block put initialised fields into a `struct`, which the grammar rejects. In a scratch copy with the `meta*` fields and accessors removed from the two structs, the model ran and printed verdicts such as `REFUSE INSERT games  (2 values vs 3 columns -- ARITY)`. The model only validates plans from facts you pass it; it does not call the C engine (README says so).
- **Transactions: two documents disagree.** `SPECIFICATION.md` section 2 lists "explicit multi-statement transactions" as not implemented, but `ROADMAP.md` marks `ssql_begin`/`commit`/`rollback` as implemented (snapshot-based, single connection), `native/include/sleela_sql.h` declares them, and `tests/test_txn.c` passes. The CLI itself has no `BEGIN` statement: `sleela-sql <dir> "BEGIN"` printed `sleela-sql: syntax error`. Transactions are reachable from C (and through [db-connector](db-connector.md)), not from the CLI.
- **ALTER TABLE is listed as open in the roadmap** (`[ ] ALTER TABLE ADD/RENAME/DROP COLUMN`) while ADD works, as you just saw; the roadmap line also covers RENAME and DROP, which I did not try.
- Not ACID, no crash recovery, no concurrent-writer isolation, one equality predicate, 64 columns, string values only (`SPECIFICATION.md` sections 2 and 5).

## Next steps

- [`SPECIFICATION.md`](../../../lib/sleela-sql/SPECIFICATION.md), [`ROADMAP.md`](../../../lib/sleela-sql/ROADMAP.md).
- [`examples/08-prepared-statements.c`](../../../lib/sleela-sql/examples/08-prepared-statements.c): positional `?` binds from C.
- [db-connector](db-connector.md) wraps this engine in a JDBC-style API; [database](../explanations/database.md) is the SLeeLa facade that does not yet call either.

## Sources

Files read: `bin/sleelactl.d/registry.conf` (line 42), `lib/sleela-sql/README.md`, `SPECIFICATION.md`, `ROADMAP.md`, `Makefile`, `examples/README.md`, `examples/slsql/README.md` (head), `samples/demo.sql`, `samples/demo.ssql`, `native/src/sleela_sql_cli.c` (usage), `native/include/sleela_sql.h` (grep), `SqlModel.sleela`.

Commands run (with `timeout`): `bin/sleelactl help|available|status|open|close sleela-sql`; `make` and `make test`; the CLI commands above; `impl/build/sleela run lib/sleela-sql/SqlModel.sleela` and `check`; a scratch copy of `SqlModel.sleela` with the struct metadata removed. Built here: `lib/sleela-sql/build/` (git-ignored). Database directories were under `/tmp` or a scratch directory and were deleted.
