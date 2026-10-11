# SLeeLa SQL Native Specification

**Status:** bounded local-file SQL engine, not a general-purpose SQL server.  
**Implementation:** C11 core with optional C++17 RAII facade.  
**Storage:** one CSV file per table; header row contains column names.

## 1. Architecture

Classic SQL and fluent SLeeLaSQL are front ends to one parser/compiler and one executor. Both lower to the same opaque `ssql_stmt` representation. The SLeeLa source model in `SqlModel.sleela` is a separate validation/reporting layer; it does not call the C engine automatically.

## 2. Supported statement subset

| Operation | SQL syntax | Semantics |
|---|---|---|
| Create | `CREATE TABLE t (c1, c2, ...)` | Creates `t.csv` with a header |
| Drop | `DROP TABLE t`; `DROP TABLE IF EXISTS t` | Removes table file |
| Insert | `INSERT INTO t VALUES (v1, ...)` | Appends one positional row |
| Select | `SELECT * FROM t [WHERE c = v]` | Reads all/projected columns; one equality predicate |
| List | `SHOW TABLES` | Emits CSV table names |

Fluent equivalents: `table('t').create(c1, c2)`, `from('t').drop()`, `from('t').drop(ifExists)`, `into('t').insert(v1, v2)`, `from('t').select(*)`, `from('t').select(c1).where(c == v)`, and `tables()`.

This is a subset. It is **not** MySQL/PostgreSQL/SQLite wire-protocol compatible and does not implement joins, aggregates, indexes, schema types, constraints, subqueries, ordering, grouping, or general transactions.

## 3. Prepared statements

- Placeholder: `?`, positional and 1-based.
- `ssql_prepare` parses/compiles once; `ssql_bind` copies a value into a placeholder.
- `ssql_reset` clears bound values; every placeholder must be rebound before a subsequent run.
- Call `ssql_finalize` for each successful C prepare. C++ `Statement` is move-only and finalizes automatically.
- Binding does not interpolate values into SQL text. This small engine is not a hardened security boundary.

## 4. Status codes

`SSQL_OK`, `SSQL_ERR_IO`, `SSQL_ERR_SYNTAX`, `SSQL_ERR_NOTABLE`, `SSQL_ERR_EXISTS`, `SSQL_ERR_NOCOL`, `SSQL_ERR_ARITY`, `SSQL_ERR_ARG`, `SSQL_ERR_BIND`, `SSQL_ERR_OOM`.

## 5. Bounds and persistence

- Maximum 64 columns/values (`SSQL_MAX_COLS`).
- Field and directory buffers are bounded by `SSQL_MAX_FIELD = 512`.
- Values are strings; there is no SQL type system or numeric coercion.
- CSV is the persistence format, not an ACID database file format.
- Only use trusted table names and a directory not writable by hostile users; this implementation does not provide a hardened path sandbox.

## 6. C and C++ contracts

The C API is the core ABI. Include `native/include/sleela_sql.h`, link the C library, and compile as C11. The C++17 facade in `cpp/include/sleela_sql.hpp` wraps that API through `sleela::sql::Database` and move-only `sleela::sql::Statement`; it does not fork SQL semantics.

## 7. Build and verification

From `lib/sleela-sql`, run `make` and `make test`. The engine currently uses POSIX directory enumeration (`dirent.h`); do not claim an MSVC/Windows build until a Windows directory-enumeration adapter is implemented and tested.
