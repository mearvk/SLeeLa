# Connector Catalog

Finite connector registry. "Baseline adapter" means a C/C++ API and build target exist; it does not imply full production readiness or that integration tests have passed. Vendor dependencies must be installed separately.

## Database connectors

| Family / reference | Implementation | Current capability and limitation |
|---|---|---|
| `mysql` | MySQL-compatible C client API | C ABI, C++17 RAII, connect/ping/execute/error/affected rows; no row retrieval or typed prepared-statement API yet |
| `mariadb` | Shared `mysql` adapter, MariaDB Connector/C build profile | API-compatible profile; validate against target versions |
| `sqlite` | SQLite C API | C/C++ connection wrapper, prepared statements, text/int64/double/NULL binds, step/finalize |
| `postgres` | libpq | C/C++ connection wrapper and parameterized `PQexecParams`; results remain native `PGresult` objects |
| `odbc` | ODBC 3.x driver manager | Connection lifecycle, direct statement execution, row count; no portable parameter binding or row fetch yet |
| `sqlserver` | ODBC profile | Uses operator-installed Microsoft ODBC Driver for SQL Server through `odbc`; same limitations apply |
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

The baseline is deliberately honest about missing features. Before marking a family production-ready, add:
1. Typed prepared-parameter binding wherever the protocol supports it; never concatenate untrusted values into SQL.
2. Complete result/row streaming, deterministic cleanup, and documented cancellation/timeout semantics.
3. TLS/authentication configuration appropriate to each driver and safe secret handling.
4. Unit tests and opt-in integration tests against configured services.
5. Verified builds on Linux, macOS, and Windows 10+ with documented dependency versions.

No live database, REST endpoint, or MQTT broker integration test was run as part of these file additions.
