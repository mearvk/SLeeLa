# SLeeLa SQL — native C/C++ engine and SLeeLa model

This package groups the SLeeLa model with the native C SQL engine and a C++17 RAII wrapper.

- `SqlModel.sleela`: SLeeLa-side plan validation and verdicts.
- `native/include/sleela_sql.h`, `native/src/sleela_sql.c`: C11 engine and public API.
- `native/src/sleela_sql_cli.c`: `sleela-sql` command-line program.
- `cpp/include/sleela_sql.hpp`, `cpp/src/sleela_sql.cpp`: C++17 facade over the same C API.
- `docs/SLEELASQL.md`: fluent dialect grammar.
- `SPECIFICATION.md`: supported subset, bounds, API, and limitations.
- `samples/`: classic SQL and fluent SLeeLaSQL sessions.

## Build and test

```sh
cd lib/sleela-sql
make
make test
```

Outputs are kept under `build/`: CLI, C static library, and C++ facade library. Consumers of the C++ facade must link both the C++ and C core libraries.

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
