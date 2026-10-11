# Native Database and Data-Source Connectors

The first native connector is MySQL/MariaDB through the MySQL-compatible C client API (mysqlclient or MariaDB Connector/C).

## MySQL adapter
- mysql/mysql_connector.h and mysql_connector.c: C ABI, connection lifecycle, ping, execution, errors, and affected-row access.
- mysql/mysql_connector.hpp and mysql_connector.cpp: C++17 RAII wrapper around the C ABI.
- mysql/Makefile: pkg-config build for Linux/macOS; configurable client flags for Windows toolchains.
- mysql/test_mysql_connector.c: opt-in integration smoke test.

Install a MySQL or MariaDB client development package, then run:

    make -C lib/connector/mysql

If pkg-config cannot find your client library, provide MYSQL_CFLAGS and MYSQL_LIBS explicitly. The connector does not embed a database server or download dependencies.

## Security and behavior
- Supply credentials through environment/configuration or a secret manager; never commit passwords.
- Use TLS and certificate verification for remote databases.
- Never concatenate untrusted values into SQL. Use prepared statements or safely bind values in the application layer.
- execute accepts one caller-supplied SQL statement and deliberately does not expose SELECT result rows.
- Network access occurs only when the caller explicitly opens a connection.
