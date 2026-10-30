# SLeeLa SQL examples — what to learn here

SLeeLa SQL is a small, **CSV-backed** SQL subset — each table is a `.csv` file
with a header row. These examples teach exactly what the subset supports, and
(just as important) where it stops. Build and run instructions live in
`/lib/sleela-sql`; the authoritative dialect reference is
`/lib/sleela-sql/SPECIFICATION.md`.

Read them in order.

## The three cases

| Level | File | The one new idea |
|---|---|---|
| Basic | [`BASIC-create-and-insert.sql`](BASIC-create-and-insert.sql) | `CREATE TABLE` writes a CSV with a header; `INSERT ... VALUES` appends positional rows. |
| Moderate | [`MODERATE-select-where.sql`](MODERATE-select-where.sql) | Read back with `SELECT *`, one equality `WHERE`, and `SELECT COUNT(*)`. |
| Advanced | [`ADVANCED-update-delete-lifecycle.sql`](ADVANCED-update-delete-lifecycle.sql) | The full row lifecycle: insert → update → verify → delete → verify. |

The original [`CLASSIC-create-table.sql`](CLASSIC-create-table.sql) is the
shortest starter.

## What we want you to take away

1. **A table is a CSV file.** `CREATE TABLE contacts (id, name, email)` produces
   `contacts.csv` with that header. Rows are appended in column order, so the
   order of values in an `INSERT` must match the order of columns in the
   `CREATE`. Seeing the storage format demystifies the engine.
2. **One equality predicate — on purpose.** `WHERE c = v` is the whole filter
   language. There is **no** `AND`/`OR`, no ranges, no `ORDER BY`, no `JOIN`, and
   no aggregates beyond `COUNT(*)`. The moderate and advanced examples stay
   strictly inside that line so they actually run.
3. **Mutations rewrite the file.** `UPDATE` and `DELETE` write a temporary file
   and replace the table. That lowers partial-write risk but is **not** a
   crash-safe transaction — do not treat this as an ACID database. Read the
   "Limitations" section of the specification before relying on it.

## What is *not* here (by design)

Joins, subqueries, grouping, ordering, compound predicates, schema types,
constraints, indexes, and multi-statement transactions are not implemented. If an
example seems to need one of those, it belongs to a different engine — not this
teaching subset.

## Run them

Follow the build/run instructions in `/lib/sleela-sql/README.md` to point the
engine at a working directory, then feed it these scripts. Because tables are
CSV files, you can open the resulting `.csv` files to confirm the rows.
