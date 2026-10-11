# Native Database and Data-Source Connectors

This directory contains native C interfaces and C++17 facades for database and non-database sources. Every adapter is opt-in: its external client library must be installed, and network access occurs only when the caller explicitly opens a connection or makes a request.

## Build entry points

| Connector | Build command | External dependency |
|---|---|---|
| MySQL | `make -C lib/connector/mysql` | mysqlclient or MariaDB Connector/C |
| MariaDB profile | `make -C lib/connector/mariadb` | MariaDB Connector/C |
| SQLite | `make -C lib/connector/sqlite` | SQLite |
| PostgreSQL | `make -C lib/connector/postgres` | libpq |
| Generic ODBC | `make -C lib/connector/odbc` | ODBC development package and configured driver |
| SQL Server profile | `make -C lib/connector/sqlserver` | Microsoft ODBC Driver + ODBC |
| Oracle profile | `make -C lib/connector/oracle` | Oracle ODBC driver + ODBC |
| CSV/TSV | `make -C lib/connector/delimited` | None |
| JSON tokenizer | `make -C lib/connector/json` | None |
| XML validator | `make -C lib/connector/xml` | libxml2 |
| HTTP/REST | `make -C lib/connector/rest` | libcurl |
| MQTT publisher | `make -C lib/connector/mqtt` | Eclipse Mosquitto client |

Override each Makefile's `*_CFLAGS` and `*_LIBS` variables for nonstandard installations. The vendor profile directories reuse shared adapters rather than duplicating vendor protocol code.

## API and security notes

- Store credentials in environment/configuration or a secret manager; never commit passwords or tokens.
- Use TLS with certificate verification for remote services. SQL Server and Oracle require their vendor drivers to be installed and configured by the operator.
- SQLite, PostgreSQL, MySQL, and ODBC expose prepared/parameterized statement paths. For MySQL, keep `MYSQL_BIND` buffers and length indicators alive through execution/fetch. For ODBC, use `SQLBindParameter` with the native statement handle and maintain bound values/indicators for the driver's documented lifetime.
- HTTP responses must be checked for HTTP status separately from transport success. REST redirects are disabled and TLS verification remains enabled.
- JSON support is a structural tokenizer, not a full semantic parser. XML validation uses libxml2 with network fetching disabled; harden DTD/entity handling for untrusted documents.
- MQTT currently exposes publish, not subscription callback dispatch, and does not yet expose TLS/auth helpers or automatic reconnect policy.
- Review [CONNECTOR-CATALOG.md](CONNECTOR-CATALOG.md) for exact capabilities and gaps. Baseline code is not represented as production-certified. The connector CI workflow is configured for Linux and macOS; check its run result before treating builds as verified.
