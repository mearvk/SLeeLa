# SST Tutorial 07 — SQL Connectors

SST can describe a database-backed system without embedding provider installation into the source sheet.

## MySQL

Use `sst.MySQLConnector`; its default port is 3306.

## PostgreSQL

Use `sst.PostgreSQLConnector`; its default port is 5432.

## Shared lifecycle

configured -> connected -> disconnected

The `install()` contract is intentionally deferred.

## Exercise

Model the same LedgerDatabase system twice, once with MySQL and once with PostgreSQL. Keep the system group and public name stable so the provider remains an implementation choice.
