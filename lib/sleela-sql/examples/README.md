# SLeeLa SQL examples

These examples use the local CSV-backed `sleela-sql` command-line engine. Run commands from the repository root or adjust paths as needed.

## Build

```sh
make -C lib/sleela-sql
make -C lib/sleela-sql test
```

## Run examples

Each non-comment line is one statement. The engine creates the database directory if it does not exist; the parent directory must already exist.

```sh
mkdir -p /tmp/sleela-sql-demo
lib/sleela-sql/build/sleela-sql /tmp/sleela-sql-demo < lib/sleela-sql/examples/01-create-table.sql
lib/sleela-sql/build/sleela-sql /tmp/sleela-sql-demo < lib/sleela-sql/examples/02-insert.sql
lib/sleela-sql/build/sleela-sql /tmp/sleela-sql-demo < lib/sleela-sql/examples/03-select.sql
lib/sleela-sql/build/sleela-sql /tmp/sleela-sql-demo < lib/sleela-sql/examples/04-update.sql
lib/sleela-sql/build/sleela-sql /tmp/sleela-sql-demo < lib/sleela-sql/examples/05-delete.sql
```

Use a fresh database directory if you want to replay the create-table example from the beginning.

## Example catalog

| File | Purpose |
|---|---|
| `01-create-table.sql` | Create a table, including idempotent creation |
| `02-insert.sql` | Insert rows, including values containing commas |
| `03-select.sql` | Select columns, filter rows, and count matches |
| `04-update.sql` | Change one or more values in matching rows |
| `05-delete.sql` | Delete matching rows and inspect the remaining count |
| `06-list-and-drop.sql` | List tables and drop a table |
| `07-fluent.ssql` | Examples using the fluent SLeeLaSQL dialect |
| `08-prepared-statements.c` | Reuse a prepared statement with bound parameters |
| `09-alter-table-proposal.sql` | Reference syntax for adding a column with ALTER TABLE |

## Important: ALTER TABLE support

The current native engine does **not** implement `ALTER TABLE` or `ADD COLUMN`. The file `09-alter-table-proposal.sql` is a design/example reference only and must not be passed to the current CLI expecting it to execute. To change a schema today, create a new table with the desired columns and migrate rows through supported operations, after backing up the CSV data. The fluent dialect currently supports create, drop, insert, select, filtering, and listing tables; UPDATE and DELETE are classic-SQL-only in the current implementation.

These are examples of the implemented SQL subset, not a claim of full SQL-standard compatibility.
