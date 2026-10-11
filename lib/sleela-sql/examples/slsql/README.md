<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLSQL — SLeeLa's fluent SQL-like language

This folder demonstrates **SLSQL (SLeeLaSQL)**, SLeeLa's own fluent, SQL-like surface language for the `sleela-sql` engine. Instead of writing SQL clauses, SLSQL starts with a table-oriented root and chains operations with dotted verbs.

SLSQL is a dialect handled by the existing SLeeLa SQL engine, not a separate database engine. Supported statements are compiled to the same internal statement representation as classic SQL and use the same executor.

## Quick start

From the repository root:

```sh
make -C lib/sleela-sql
mkdir -p /tmp/slsql-demo
lib/sleela-sql/build/sleela-sql /tmp/slsql-demo --sleela < lib/sleela-sql/examples/slsql/01-contacts.ssql
```

If your CLI build uses positional options differently, invoke the statements with the CLI's `--sleela` dialect option as documented in `lib/sleela-sql/docs/SLEELASQL.md`. Run the examples in order against a fresh database directory; rerunning a `.create(...)` example against an existing table may return an exists error.

## Language examples

| File | What it teaches |
|---|---|
| `01-contacts.ssql` | Create a table and insert rows |
| `02-select-and-filter.ssql` | Select all or chosen columns and filter with `.where(...)` |
| `03-table-management.ssql` | List tables and drop a table |
| `04-prepared-statements.c` | Use the SLSQL dialect with reusable bound parameters from C |
| `05-sql-equivalents.md` | Compare SLSQL expressions with their classic SQL equivalents |

## The shape of SLSQL

```text
table('contacts').create(id, name, email)
into('contacts').insert(1, 'Ada Lovelace', 'ada@example.test')
from('contacts').select(name, email).where(id == 1)
tables()
```

- `table(...).create(...)` defines a table and its columns.
- `into(...).insert(...)` adds a positional row; value count must match the column count.
- `from(...).select(...)` reads all columns with `*` or a chosen projection.
- `.where(column == value)` filters with one equality predicate. `=` is also accepted.
- `tables()` lists the tables in the selected database directory.
- `?` placeholders can be used in prepared statements and bound by position.

## Current limitations

The fluent SLSQL dialect currently supports table creation, insertion, selection, equality filtering, table listing, and dropping tables. Classic SQL-only features include `ALTER TABLE ... ADD COLUMN`, `UPDATE`, and `DELETE`; their fluent equivalents are not implemented yet. Joins, ordering, grouping, compound predicates, indexes, schema types, and transactions are also outside the supported subset. Values are stored as strings in CSV files.

For the authoritative grammar and C API details, see [SLeeLaSQL dialect documentation](../../docs/SLEELASQL.md) and [native specification](../../SPECIFICATION.md).
