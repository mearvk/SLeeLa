# Connector Catalog

Finite connector registry. "Baseline adapter" means a C/C++ API and build target exist; it does not imply full production readiness or that integration tests have passed. Vendor dependencies must be installed separately.

## Database connectors

| Family / reference | Implementation | Current capability and limitation |
|---|---|---|
| `mysql` | MySQL-compatible C client API | C ABI, C++17 RAII, connect/ping/execute/errors, native typed prepared-statement parameter/result binding and fetch wrappers; result binding uses MySQL's `MYSQL_BIND` API |
| `mariadb` | Shared `mysql` adapter, MariaDB Connector/C build profile | API-compatible profile; validate against target versions |
| `sqlite` | SQLite C API | C/C++ connection wrapper, prepared statements, text/int64/double/NULL binds, step/finalize |
| `postgres` | libpq | C/C++ connection wrapper and parameterized `PQexecParams`; results remain native `PGresult` objects |
| `odbc` | ODBC 3.x driver manager | Connection lifecycle, direct/prepared execution, row fetch and column reads; native `SQLBindParameter` is available through the exposed statement handle |
| `sqlserver` | ODBC profile | Uses operator-installed Microsoft ODBC Driver for SQL Server through `odbc`; same API/capabilities apply |
| `oracle` | ODBC profile | Uses operator-installed Oracle ODBC driver through `odbc`; this is not a direct OCI implementation |
| Generic ODBC | ODBC 3.x | Supports configured DSN/driver connections through the generic adapter |

## Non-database data sources

| Family / reference | Implementation | Current capability and limitation |
|---|---|---|
| `delimited` | Dependency-free C parser + C++ facade | One CSV/TSV record, quoted fields and doubled quotes; caller owns file iteration and encoding |
| `json` | Dependency-free structural tokenizer + C++ facade | Token offsets and parent links; not a full standards-compliant JSON DOM/validator |
| `xml` | libxml2 validator + C++ facade | Well-formedness validation with network fetching disabled; no XPath/DOM/schema API |
| `rest` | libcurl C API + C++ facade | GET and JSON POST, timeouts, TLS peer/host verification, redirects disabled; no OAuth/arbitrary-header/retry layer |
| `mqtt` | Eclipse Mosquitto C API + C++ publisher facade | Connect/publish/network loop; subscriptions, TLS/auth configuration helpers, and reconnect policy not exposed yet |

## Deterministic connector references

Use explicit names such as `mysql`, `postgres`, `sqlite`, `odbc`, `sqlserver`, `oracle`, `rest`, and `mqtt`. A future SLeeLa/Dynamite registry must reject duplicate references and ambiguous unnamed connectors rather than silently selecting one. Keep per-connector properties scoped to the chosen reference.

## Production-readiness checklist

Before marking a family production-ready, add:
1. Integration tests against configured database/services and verified platform builds.
2. Fully documented parameter lifetimes, result ownership, transactions, cancellation, and timeout semantics.
3. TLS/authentication configuration appropriate to each driver and safe secret handling.
4. Full result/row streaming and capability discovery where relevant.
5. Verified builds on Linux, macOS, and Windows 10+ with documented dependency versions.

A connector build/test workflow has been added for Linux and macOS. It is not yet a verified result until the workflow finishes; no live database, REST endpoint, or MQTT broker integration test was run during these edits.
