# Connector Catalog

Finite connector registry. "Baseline adapter" means a C/C++ API and build target exist; it does not imply full production readiness or that integration tests have passed. Vendor dependencies must be installed separately. Build commands and security notes are in [README.md](README.md).

## Database connectors

| Family / reference | Implementation | Current capability and limitation |
|---|---|---|
| `mysql` | MySQL-compatible C client API | C ABI, C++17 RAII, connect/ping/execute/errors, prepared-statement parameter/result binding and fetch wrappers using MySQL's `MYSQL_BIND` API; bound buffers and indicators must remain valid for the required statement lifetime |
| `mariadb` | Shared `mysql` adapter, MariaDB Connector/C build profile | API-compatible build profile; validate against target server/client versions |
| `sqlite` | SQLite C API | C/C++ connection wrapper, prepared statements, text/int64/double/NULL binds, step/finalize |
| `postgres` | libpq | C/C++ connection wrapper and parameterized `PQexecParams`; results remain native `PGresult` objects owned by the caller |
| `odbc` | ODBC 3.x driver manager | Connection lifecycle, direct/prepared execution, row fetch and column reads; native `SQLBindParameter` is available through the exposed statement handle |
| `sqlserver` | ODBC profile | Uses operator-installed Microsoft ODBC Driver for SQL Server through `odbc`; same API and capabilities apply |
| `oracle` | ODBC profile | Uses operator-installed Oracle ODBC driver through `odbc`; this is not a direct OCI implementation |

## Non-database data sources

| Family / reference | Implementation | Current capability and limitation |
|---|---|---|
| `delimited` | Dependency-free C parser + C++ facade | Parses one CSV/TSV record, quoted fields and doubled quotes; caller owns file iteration, encoding, headers, and schema inference |
| `json` | Dependency-free structural tokenizer + C++ facade | Records token offsets/types/parent links and checks structural nesting/string escapes; it is not a semantic JSON DOM or complete standards-compliant validator, and primitive syntax/string decoding are not fully validated |
| `xml` | libxml2 validator + C++ facade | Well-formedness validation with network fetching disabled; no XPath/DOM/schema API |
| `rest` | libcurl C API + C++ facade | GET and JSON POST, timeouts, TLS peer/host verification, redirects disabled; no OAuth/arbitrary-header/retry layer |
| `mqtt` | Eclipse Mosquitto C API + C++ client facade | Connect/publish/subscribe/callback dispatch/network loop; TLS/authentication helpers and reconnect policy are not exposed yet |

## Connector reference registry

`connector_registry.h/.c` and the C++17 facade provide explicit named references, non-owning native handles, per-reference properties, and deterministic lookup. Duplicate references are rejected. Unnamed lookup succeeds only with exactly one registered connector; zero or multiple candidates are errors. This prevents a configuration such as `mysql` plus `postgres` from silently resolving to an arbitrary connection.

## Production-readiness checklist

Before marking a family production-ready, add:

1. Integration tests against configured database/services and verified platform builds.
2. Fully documented parameter lifetimes, result ownership, transactions, cancellation, and timeout semantics.
3. TLS/authentication configuration appropriate to each driver and safe secret handling.
4. Full result/row streaming and capability discovery where relevant.
5. Verified builds on Linux, macOS, and Windows 10+ with documented dependency versions.

The workflow runs build and smoke-test jobs on Linux and macOS. Windows is not yet covered by that workflow. No live database, REST endpoint, or MQTT broker integration test is represented by the default CI; review the latest workflow result before treating the build as verified.
