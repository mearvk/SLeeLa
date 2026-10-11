# Native Database and Data-Source Connectors

This directory contains native C interfaces and C++17 facades for database and non-database sources. Every adapter is opt-in: its external client library must be installed, and network access occurs only when the caller explicitly opens a connection or makes a request.

## Group build

Run from this directory:

- `make all` — build the registry, all database connectors and all data-source connectors.
- `make database` — build MySQL, MariaDB, SQLite, PostgreSQL, ODBC, SQL Server and Oracle profiles.
- `make data` — build CSV/TSV, JSON, XML, REST and MQTT connectors.
- `make test` — run the registry, CSV/TSV, JSON and XML smoke tests.
- `make check-deps` — report dependencies discoverable through pkg-config.
- `make clean` — clean all connector build outputs.

Convenience entry points: `sh build.sh all` on Linux/macOS and `build.bat all` on Windows with GNU Make plus a configured C/C++ toolchain. Windows requires each third-party development library and its headers/libraries to be installed for the selected compiler; set the connector `*_CFLAGS` and `*_LIBS` variables when pkg-config metadata is unavailable.

## Individual connector builds

| Connector | Build command | External dependency |
|---|---|---|
| Registry and scoped properties | `make` (this directory) | None |
| MySQL | `make -C mysql` | mysqlclient or MariaDB Connector/C |
| MariaDB profile | `make -C mariadb` | MariaDB Connector/C |
| SQLite | `make -C sqlite` | SQLite |
| PostgreSQL | `make -C postgres` | libpq |
| Generic ODBC | `make -C odbc` | ODBC development package and configured driver |
| SQL Server profile | `make -C sqlserver` | Microsoft ODBC Driver + ODBC |
| Oracle profile | `make -C oracle` | Oracle ODBC driver + ODBC |
| CSV/TSV | `make -C delimited` | None |
| JSON tokenizer | `make -C json` | None |
| XML validator | `make -C xml` | libxml2 |
| HTTP/REST | `make -C rest` | libcurl |
| MQTT client | `make -C mqtt` | Eclipse Mosquitto client |

Each database connector can also be built independently. The SQL Server and Oracle profiles reuse the shared ODBC implementation; they are not separate protocol implementations.

## Named connector registry

The shared registry supports deterministic references such as `mysql`, `postgres`, and `sqlite`, keeps properties scoped to each reference, rejects duplicate names, and rejects ambiguous unnamed lookup. It stores native connector handles as non-owning pointers; the caller remains responsible for opening and closing each connector.

## API and security notes

- Store credentials in environment/configuration or a secret manager; never commit passwords or tokens.
- Use TLS with certificate verification for remote services. SQL Server and Oracle require their vendor drivers to be installed and configured by the operator.
- SQLite, PostgreSQL, MySQL, and ODBC expose prepared/parameterized statement paths. For MySQL, keep `MYSQL_BIND` buffers and length indicators alive through execution/fetch. For ODBC, use `SQLBindParameter` with the native statement handle and maintain bound values/indicators for the driver's documented lifetime.
- HTTP responses must be checked for HTTP status separately from transport success. REST redirects are disabled and TLS verification remains enabled.
- JSON support is a structural tokenizer, not a full semantic parser. XML validation uses libxml2 with network fetching disabled; harden DTD/entity handling for untrusted documents.
- MQTT supports publish and callback-based subscribe; call `sleela_mqtt_global_cleanup()` only after all MQTT clients are closed. TLS/auth helpers and reconnect policy are not exposed yet.
- Review [CONNECTOR-CATALOG.md](CONNECTOR-CATALOG.md) for exact capabilities and gaps. Baseline code is not represented as production-certified.
