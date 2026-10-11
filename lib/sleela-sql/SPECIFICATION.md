# SLeeLa SQL Native Specification

**Status:** bounded local-file SQL engine, not a general-purpose SQL server.  
**Implementation:** C11 core with optional C++17 RAII facade.  
**Storage:** one CSV file per table; header row contains column names.

## 1. Architecture

Classic SQL and fluent SLeeLaSQL are front ends to one parser/compiler and one executor. Both lower to the same opaque `ssql_stmt` representation. The SLeeLa source model in `SqlModel.sleela` is a separate validation/reporting layer; it does not call the C engine automatically.

## 2. Supported statement subset

| Operation | SQL syntax | Semantics |
|---|---|---|
| Create | `CREATE TABLE [IF NOT EXISTS] t (c1, c2, ...)` | Creates `t.csv` with a header; optional clause makes existing-table creation idempotent |
| Drop | `DROP TABLE t`; `DROP TABLE IF EXISTS t` | Removes table file |
| Insert | `INSERT INTO t VALUES (v1, ...)` | Appends one positional row |
| Update | `UPDATE t SET c = v [, ...] [WHERE c = v]` | Rewrites matching rows; omitted WHERE updates all rows |
| Delete | `DELETE FROM t [WHERE c = v]` | Removes matching rows; omitted WHERE deletes all rows |
| Select | `SELECT * FROM t [WHERE c = v]`; `SELECT COUNT(*) FROM t [WHERE c = v]` | Reads projected rows or counts matching rows; one equality predicate |
| List | `SHOW TABLES` | Emits CSV table names |

Fluent equivalents: `table('t').create(c1, c2)`, `from('t').drop()`, `from('t').drop(ifExists)`, `into('t').insert(v1, v2)`, `from('t').select(*)`, `from('t').select(count(*))`, `from('t').select(c1).where(c == v)`, and `tables()`.

This is a subset. It is **not** MySQL/PostgreSQL/SQLite wire-protocol compatible. UPDATE and DELETE are currently SQL-dialect-only and support a single equality predicate; fluent equivalents, compound predicates, joins, general aggregate functions beyond `COUNT(*)`, indexes, schema types, constraints, subqueries, ordering, grouping, and explicit multi-statement transactions are not implemented.

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
- CSV is the persistence format, not an ACID database file format. UPDATE/DELETE write a temporary file and replace the table; this reduces partial-write risk but does not provide crash-recoverable transactions, durable multi-table commits, or concurrent-writer isolation.
- Only use trusted table names and a directory not writable by hostile users; this implementation does not provide a hardened path sandbox.

## 6. C and C++ contracts

The C API is the core ABI. Include `native/include/sleela_sql.h`, link the C library, and compile as C11. The C++17 facade in `cpp/include/sleela_sql.hpp` wraps that API through `sleela::sql::Database` and move-only `sleela::sql::Statement`; it does not fork SQL semantics.

## 7. Portable file I/O

- macOS and other POSIX platforms use `opendir`/`readdir`, `mkdir`, and standard C file streams.
- Windows builds use the CRT directory-search adapter (`_findfirst`/`_findnext`), `_mkdir`, and `_stricmp`; MinGW-w64 is the supported Windows toolchain in CI.
- `ssql_open` creates the final database directory when it does not exist, and rejects paths that exceed the fixed directory buffer or resolve to a non-directory.
- Table identifiers must begin with an ASCII letter or underscore and contain only ASCII letters, digits, underscore, or hyphen. This prevents table-name path traversal. Parent directories are not created recursively.
- Windows file paths are passed through the active C runtime; non-ASCII Windows path behavior depends on the runtime's locale/code-page configuration.

## 8. Build and verification

From `lib/sleela-sql`, run `make` and `make test`. CI builds and runs the C11 core and C++17 facade on Linux, macOS, and Windows (MinGW-w64).
