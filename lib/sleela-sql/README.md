<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa SQL — native C/C++ engine and SLeeLa model

This package groups the SLeeLa model with the native C SQL engine and a C++17 RAII wrapper.

- `SqlModel.sleela`: SLeeLa-side plan validation and verdicts.
- `native/include/sleela_sql.h`, `native/src/sleela_sql.c`: C11 engine and public API.
- `native/src/sleela_sql_cli.c`: `sleela-sql` command-line program.
- `cpp/include/sleela_sql.hpp`, `cpp/src/sleela_sql.cpp`: C++17 facade over the same C API.
- `docs/SLEELASQL.md`: fluent dialect grammar.
- `SPECIFICATION.md`: supported subset, bounds, API, and limitations.
- `samples/`: classic SQL and fluent SLeeLaSQL sessions.
- Portable storage I/O: POSIX directory enumeration on macOS/Linux and a MinGW-w64 CRT adapter on Windows.

## Build and test

```sh
cd lib/sleela-sql
make
make test
```

Outputs are kept under `build/`: CLI, C static library, and C++ facade library. Consumers of the C++ facade must link both the C++ and C core libraries. `ssql_open` creates the final database directory if absent; it does not recursively create parent directories.

The SQL subset supports idempotent `CREATE TABLE IF NOT EXISTS`, `SELECT COUNT(*) ... [WHERE column = value]`, `UPDATE table SET column = value [, ...] [WHERE column = value]`, and `DELETE FROM table [WHERE column = value]`. UPDATE/DELETE rewrite a temporary CSV and replace the table file; on Windows a backup is used to restore the original if replacement fails. These statements currently use equality-only WHERE predicates and are SQL-dialect features; fluent SLeeLaSQL update/delete and richer predicates are not yet implemented.

## C example

```c
#include "sleela_sql.h"
ssql_db db;
ssql_open(&db, "./data");
ssql_exec(&db, "CREATE TABLE games (id, title)", NULL);
ssql_exec(&db, "INSERT INTO games VALUES (1, 'Metroid')", NULL);
```

## C++ example

```cpp
#include "sleela_sql.hpp"
sleela::sql::Database db("./data");
if (db.status() == SSQL_OK)
    db.execute("from('games').select(*)", stdout);
```

The engine is a local CSV-backed SQL subset, not a network database server or a full MySQL implementation. Read `SPECIFICATION.md` before relying on it for production data. `SqlModel.sleela` validates planned operations but does not automatically invoke the native library; that runtime bridge remains separate integration work.

## Dialects, prepared statements, and model boundary

Classic SQL and fluent SLeeLaSQL compile to the same internal statement and executor. The C API supports reusable prepared statements in either dialect with positional `?` placeholders (`ssql_prepare`, `ssql_bind`, `ssql_reset`, `ssql_run`, and `ssql_finalize`). See [`docs/SLEELASQL.md`](docs/SLEELASQL.md) for grammar and examples.

`SqlModel.sleela` validates planned operations using schema facts supplied by the caller: CREATE collisions, INSERT arity, and projected-column existence. It is a SLeeLa-side model/validation example, not an automatic runtime bridge to the native C engine.

