# Oracle Database connector profile

Uses the shared ODBC C/C++ connector in `../odbc` with an Oracle-supported ODBC driver/driver manager configured by the operator. This profile does not bundle Oracle Instant Client or OCI. Use wallet/driver TLS configuration and external secret management for credentials. Prepared execution and row/column retrieval are available through the shared API; use native `SQLBindParameter` on the exposed statement handle for typed values, and maintain bound buffers/indicators for the driver's documented lifetime.
