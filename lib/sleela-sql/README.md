# lib/sleela-sql — the SLeeLa model for the CSV SQL engine

The **SLeeLa counterpart** to the C engine in [`/sleela-sql`](../../sleela-sql/).
Part of `mearvk/Nintendo`.

SLeeLa's file surface is **string-oriented, not raw-byte**, so — exactly as with
`NesEditModel.sleela` in the Professional Editor — it does not touch the CSV
bytes. Instead it owns the **model and validation** layer: given a planned
statement and a table's schema, it decides whether the statement is well-formed
*before* the byte engine runs, and prints a clear `OK` / `REFUSE` verdict.

The engine accepts two surface languages — classic **SQL** and the fluent
**SLeeLaSQL** (see [`/sleela-sql/docs/SLEELASQL.md`](../../sleela-sql/docs/SLEELASQL.md)) —
but both lower to the **same compiled operation**. This model validates that
lowered form, so one verdict covers a plan regardless of which dialect expressed
it, and regardless of whether it used `?` prepared-statement placeholders
(binding fills values, not shape).

It runs on the **SLeeLa Native VM**:

```sh
sleela run lib/sleela-sql/SqlModel.sleela
```

## Invariants it checks (mirroring `sleela_sql.c`)

| Invariant | Byte-engine status it mirrors |
|---|---|
| `INSERT` value count == column count | `SSQL_ERR_ARITY` |
| projected / `WHERE` column exists in the schema | `SSQL_ERR_NOCOL` |
| `CREATE` does not collide with an existing table | `SSQL_ERR_EXISTS` |

The worked plans in `main()` mirror [`/sleela-sql/samples/demo.sql`](../../sleela-sql/samples/demo.sql),
so the model and the byte tool agree on which statements are accepted and which
are refused.

## The A→B relationship

```
SqlModel.sleela   validates the planned statement against the schema  (model)
        │
        ▼
sleela-sql (C)    parses the SQL and reads/writes the CSV table files (bytes)
```

SLeeLa reasons about the plan; the C tool in `/sleela-sql` executes it.
