# Connector Catalog

This is a finite, extensible registry of connector families—not a claim that every vendor or protocol has been implemented. A family is considered implemented only when its adapter, build target, and tests exist.

## Database connectors

| Family | Native interface | Status |
|---|---|---|
| MySQL | MySQL-compatible C client API | Initial C API and C++ RAII wrapper added under mysql/ |
| MariaDB | MariaDB Connector/C (compatible API) | Covered by the initial MySQL-compatible adapter; validate against target server/client version |
| SQLite | SQLite C API | Planned |
| PostgreSQL | libpq | Planned |
| Microsoft SQL Server | ODBC / Microsoft driver | Planned |
| Oracle Database | Oracle OCI or ODBC | Planned |
| Generic ODBC data sources | ODBC driver manager | Planned |

## Non-database data sources

Separate adapters can later cover:
- Local delimited files (CSV/TSV)
- JSON and XML documents
- HTTP/REST endpoints
- ODBC-backed enterprise data sources
- Message/event sources where a stable protocol and lifecycle are specified

These should not all be forced through a SQL connection abstraction. Shared interfaces should cover lifecycle, capability discovery, status/error reporting, cancellation/timeouts, and data exchange; protocol-specific APIs should retain prepared statements, typed parameters, streaming, transactions, and schema metadata where supported.

## Completion criteria

A connector is ready to be called implemented only when it has:
1. A documented C ABI and, where useful, a C++ RAII facade.
2. Explicit dependency detection and platform-specific build instructions for Linux, macOS, and Windows 10+.
3. Safe parameter binding for untrusted values, with no string-concatenation shortcut presented as safe.
4. Deterministic error/status handling and resource cleanup.
5. Unit tests plus an opt-in integration test requiring an explicitly configured service.
6. Clear documentation of unsupported capabilities, TLS/authentication behavior, and driver/version assumptions.
