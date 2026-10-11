# Native Database and Data-Source Connectors

This directory contains native C interfaces and C++17 facades for database and non-database sources. Each adapter is opt-in: its external client library must be installed, and network or database access begins only when the caller explicitly opens a connection or makes a request.

## Group build

From the repository root, use:

- `make -C lib/connector build` — build the registry, all database connectors, and all data-source connectors.
- `make -C lib/connector database` — build MySQL, MariaDB, SQLite, PostgreSQL, ODBC, SQL Server, and Oracle profiles.
- `make -C lib/connector data` — build CSV/TSV, JSON, XML, REST, and MQTT connectors.
- `make -C lib/connector test` — run dependency-free registry, CSV/TSV, JSON, and XML smoke tests.
- `make -C lib/connector test-rest` — run REST argument/lifecycle guard tests (requires libcurl development files; no network request is made).
- `make -C lib/connector test-mqtt` — run MQTT invalid-state/lifecycle guard tests (requires libmosquitto development files; no broker is contacted).
- `make -C lib/connector check-deps` — report dependencies discoverable through pkg-config.
- `make -C lib/connector clean` — clean connector build outputs.

If your current directory is `lib/connector`, the equivalent commands are `make build`, `make database`, `make data`, `make test`, `make check-deps`, and `make clean`. The `build` target is an alias for `all`.

Convenience entry points are `sh lib/connector/build.sh all` on Linux/macOS from the repository root, or `build.bat all` from a Windows command prompt while in `lib/connector`. Windows requires GNU Make (or MinGW Make), a configured C/C++ toolchain, and development headers/libraries for every selected connector; set connector-specific `*_CFLAGS` and `*_LIBS` variables when pkg-config metadata is unavailable. The Windows build has not yet been verified by the connector CI.

## Individual connector builds

Commands in this table are run from the repository root.

| Connector | Build command | External dependency |
|---|---|---|
| Registry and scoped properties | `make -C lib/connector registry` | None |
| MySQL | `make -C lib/connector/mysql build` | mysqlclient or MariaDB Connector/C |
| MariaDB profile | `make -C lib/connector/mariadb build` | MariaDB Connector/C |
| SQLite | `make -C lib/connector/sqlite build` | SQLite |
| PostgreSQL | `make -C lib/connector/postgres build` | libpq |
| Generic ODBC | `make -C lib/connector/odbc build` | ODBC development package and configured driver |
| SQL Server profile | `make -C lib/connector/sqlserver build` | Microsoft ODBC Driver + ODBC |
| Oracle profile | `make -C lib/connector/oracle build` | Oracle ODBC driver + ODBC |
| CSV/TSV | `make -C lib/connector/delimited build` | None |
| JSON tokenizer | `make -C lib/connector/json build` | None |
| XML validator | `make -C lib/connector/xml build` | libxml2 |
| HTTP/REST | `make -C lib/connector/rest build` | libcurl |
| MQTT client | `make -C lib/connector/mqtt build` | Eclipse Mosquitto client |

The SQL Server and Oracle profiles reuse the shared ODBC implementation; they are not separate protocol implementations. Build each family independently if you do not need every optional third-party dependency.

## Named connector registry

The shared registry supports deterministic references such as `mysql`, `postgres`, and `sqlite`, keeps properties scoped to each reference, rejects duplicate names, and rejects ambiguous unnamed lookup. It stores native connector handles as non-owning pointers; the caller remains responsible for opening and closing each connector.

## API and security notes

- Store credentials in environment/configuration or a secret manager; never commit passwords or tokens.
- Use TLS with certificate verification for remote services. SQL Server and Oracle require their vendor drivers to be installed and configured by the operator.
- SQLite, PostgreSQL, MySQL, and ODBC expose prepared/parameterized statement paths. For MySQL, keep `MYSQL_BIND` buffers and length indicators alive through execution/fetch. For ODBC, use `SQLBindParameter` with the native statement handle and maintain bound values/indicators for the driver's documented lifetime.
- HTTP responses must be checked for HTTP status separately from transport success. REST redirects are disabled and TLS verification remains enabled.
- JSON support is a structural tokenizer, not a semantic JSON parser or complete standards validator. Validate untrusted input with a standards-compliant parser. XML validation uses libxml2 with network fetching disabled; review DTD/entity handling for untrusted documents.
- MQTT supports publish and callback-based subscribe; call `sleela_mqtt_global_cleanup()` only after all MQTT clients are closed. TLS/auth helpers and reconnect policy are not exposed yet.
- The default offline test suite exercises registry, delimited, JSON, and XML behavior. Optional `test-rest` and `test-mqtt` targets exercise argument and invalid-state guards without contacting remote services, but require their respective development libraries. The default suite does not connect to live databases, REST endpoints, or MQTT brokers. Check the latest CI run before treating a platform as verified.
- Review [CONNECTOR-CATALOG.md](CONNECTOR-CATALOG.md) for exact capabilities and known gaps. These baseline adapters are not represented as production-certified.
