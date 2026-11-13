<img align="right" src="../../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# database: Explanation

| | |
|---|---|
| Nature | datasource (`bin/sleelactl help database`) |
| Registry summary | SQL data sources + connection pool (open/close/active) |
| Lives in | `lib/database/` (16 `.sleela` files, 51 lines each) |
| Writeup type | Explanation, because the 16 classes are identical placeholder state flags with no database behind them, no demo, and a `close()` method that fails at runtime. |
| Verified | 2026-11-13 against `master` e048707fd: `bin/sleelactl available/status/open/close database` exit 0 and only print guidance; a scratch program using the `SLDataSource` body ran `configure`/`open`/`active` correctly and then died on `close()`. |

## What it is

`lib/database/` is the SLeeLa-side facade for SQL access: `SLDataSource`, `SLConnectionPool`, `SLDatabase`, `SLDatabaseConfig`, `SLQuery`, `SLPreparedStatement`, `SLResultSet`, `SLRow`, `SLColumn`, `SLTable`, `SLSchema`, `SLIndex`, `SLMigration`, `SLStatement`, `SLQueryParameter` and `SLTransaction`. The names mirror Java's `java.sql`, as [`lib/db-connector/README.md`](../../../lib/db-connector/README.md) says ("`java.sql.*` API: `lib/database/*.sleela` facade classes"). The same README states that wiring these classes to the C connector "is the natural next step and is not done here".

## How it works

Every file in the directory has the same body. Apart from the class name, the code is this (from [`SLDataSource.sleela`](../../../lib/database/SLDataSource.sleela), last lines):

```java
String name; int state; int count;
void configure(String n){name=n;}
void open(){state=1;}
void close(){state=0;}
void add(){count=count+1;}
boolean active(){return state==1;}
```

I confirmed this by stripping the comment header and metadata accessors from all 16 files; the remaining text was identical in each, for example `SLQuery`, `SLRow` and `SLTransaction`. So `open()` sets a flag, `active()` reads it, and `add()` increments a counter. There is no connection, no SQL text, no result set, and no call into `lib/db-connector` or `lib/sleela-sql`. The file's own comment says "Native implementation crosses an explicit VM/OS bridge", but nothing in the file calls one.

Each file also carries the standard eleven-attribute metadata record (version, id, author, date and so on) with getters and setters; that is boilerplate shared with every library class.

`bin/sleelactl` treats the module as a "data surface" and does not hold a connection itself. `sleelactl open database` prints:

```text
  ! open acts on a live SLDataSource instance inside a running SLeeLa program.
  sleelactl reports availability; it does not hold a connection of its own.
  in SLeeLa:  SLDataSource ds = new SLDataSource(); ds.configure("database"); ds.open();
```

`available` and `status` print `✓ database library present at lib/database` and the line "availability is per-connection runtime state: open() → active() == true; close() → false". `close` prints the `ds.close()` analogue of the text above. All four verbs exit 0. This matches [`bin/SLEELACTL.md`](../../../bin/SLEELACTL.md) line 632.

## Where it lives

| File | Role |
|---|---|
| `lib/database/SLDataSource.sleela` | The front-end object named by `sleelactl`; the flag/counter body above |
| `lib/database/SLConnectionPool.sleela` | Same body; no pooling logic |
| `lib/database/SLDatabase.sleela`, `SLDatabaseConfig.sleela` | Same body |
| `lib/database/SLQuery.sleela`, `SLPreparedStatement.sleela`, `SLStatement.sleela`, `SLQueryParameter.sleela`, `SLResultSet.sleela`, `SLRow.sleela`, `SLColumn.sleela` | Same body |
| `lib/database/SLTable.sleela`, `SLSchema.sleela`, `SLIndex.sleela`, `SLMigration.sleela`, `SLTransaction.sleela` | Same body |
| `bin/sleelactl.d/registry.conf` line 40 | Registers `database` as `nature=datasource dir=lib/database` |

## Status and limits

- **No runnable demo or test.** The directory has no `main`, no Makefile and no tests. `impl/build/sleela check lib/database/SLDataSource.sleela` fails with `Semantic error: no 'main' method found`, so it cannot be run by itself either. A program must declare or import the class itself.
- **The state flag works.** I copied the `SLDataSource` body into a scratch file with a `main` and ran it:

  ```text
  active before open: false
  active after open:  true
  count: 2
  ```
- **`close()` is broken at runtime.** The next statement in that program, `ds.close();`, ended the run with:

  ```text
  sleelvac: runtime error: Munction closeWithReceipt() requires a reach handle
  ```

  The exit code was 1, even though `sleela check` accepts the file (`ok (syntax declared 2.5)`). This is a known gotcha: [`.kiro/steering/sleela-authoring-gotchas.md`](../../../.kiro/steering/sleela-authoring-gotchas.md) item 2 says a no-argument method named `close()` collides with the reserved Munction built-in (see `impl/core/sleela_core.c` line 3519 and `impl/frontend/compiler.cpp` line 1364). Renaming the method to `shut()` in my scratch copy made the run finish and print `closed false`. The repository's own `sleelactl open|close` hint, which tells you to call `ds.close()`, therefore leads to this error. All 16 files define `close()`.
- **No SQL.** Nothing here parses or executes SQL. The working SQL engine is [`lib/sleela-sql`](../tutorials/sleela-sql.md), and the C driver layer is [`lib/db-connector`](../tutorials/db-connector.md).
- **Connection pool is a name only.** `SLConnectionPool` has no pool, size or lease code.

## Related

- [db-connector](../tutorials/db-connector.md): the C driver that this facade is meant to call.
- [sleela-sql](../tutorials/sleela-sql.md): the CSV-backed engine behind the only real driver.
- [ledger](ledger.md), [registry-store](../tutorials/registry-store.md): the other datasource modules.

## Sources

Files read: `bin/sleelactl.d/registry.conf`, `bin/SLEELACTL.md` (lines 1-12, 32, 632), `lib/database/*.sleela` (all 16, bodies compared), `lib/db-connector/README.md`, `.kiro/steering/sleela-authoring-gotchas.md`, `impl/core/sleela_core.c` (grep hit only).

Commands run (with `timeout`): `bin/sleelactl help|available|status|open|close database`; `impl/build/sleela check lib/database/SLDataSource.sleela`; `impl/build/sleela run` and `check` on two scratch programs (a copy of the `SLDataSource` body with a `main`, once with `close()` and once with the method renamed). The scratch files were deleted afterwards.
