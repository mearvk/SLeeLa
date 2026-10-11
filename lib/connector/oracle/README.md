# Oracle Database connector profile

Uses the shared ODBC C/C++ connector in `../odbc` with an Oracle-supported ODBC driver/driver manager configured by the operator. This profile does not bundle Oracle Instant Client or OCI. Use wallet/driver TLS configuration and external secret management for credentials. The shared ODBC baseline currently supports connection lifecycle, statement execution, and affected-row count; it does not yet expose result fetching or portable typed parameter binding, so do not execute SQL containing untrusted input.
