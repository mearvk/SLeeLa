<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

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
| `09-alter-table-add-column.sql` | Add a column with an empty default for existing rows |
| `09-alter-table-proposal.sql` | Historical design note; the feature is now implemented |
| `10-alter-table-default.sql` | Add a column and populate existing rows with a default value |
| `11-alter-table-errors.sql` | Demonstrate duplicate-column rejection (second statement expects an error) |
| [`slsql/`](slsql/README.md) | Dedicated SLSQL language walkthrough with runnable `.ssql` examples, prepared statements, and SQL equivalents |

## ALTER TABLE / ADD COLUMN

The native engine supports classic-SQL `ALTER TABLE table ADD [COLUMN] column [DEFAULT value]`. Existing rows receive the supplied default or an empty string. The operation writes a temporary CSV and replaces the original using the platform-specific replacement routine; it rejects duplicate columns and tables already at the 64-column limit. Back up important CSV data before schema migrations. The fluent SLeeLaSQL dialect does not yet expose ALTER TABLE, UPDATE, or DELETE.

These are examples of the implemented SQL subset, not a claim of full SQL-standard compatibility.