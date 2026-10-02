# SST and Nordshrift SQL Connectors

SST and Nordshrift now have a common connector foundation for MySQL and PostgreSQL.

## Architecture

.sst -> SST SQLConnector -> MySQL/PostgreSQL connector -> Nordshrift semantic boundary -> native SQL connector -> provider client

The provider client and installation mechanism are intentionally deferred.

## Configuration

A connector models provider, host, port, database, user, and connection state. Default ports are 3306 for MySQL and 5432 for PostgreSQL.

Credentials and secrets should not be committed into SST source; a future configuration/secret layer should supply them at runtime.

## Installation

The install operation exists as a contract/status boundary only. No package manager, OS installer, database client, or network connection is invoked by this change.

## Security

The connector does not imply authorization, transaction correctness, accounting controls, or production financial-system compliance. Those remain separate layers.
