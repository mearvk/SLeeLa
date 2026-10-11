# Generic ODBC connector

Provides connection lifecycle and statement execution through ODBC 3.x. It can address Microsoft SQL Server, Oracle ODBC drivers, and other DSN/driver-backed sources by using the relevant installed driver and a caller-supplied connection string. This is a generic baseline; it does not yet expose row fetching or portable typed parameter binding, so do not use it to execute SQL containing untrusted input. Prefer driver-supported prepared statements until parameter APIs are added here.

Build with `make -C lib/connector/odbc` after installing an ODBC development package and a driver manager. SQL Server and Oracle drivers are vendor-provided and must be installed/configured separately. Diagnostic detail is intentionally conservative in this first adapter.
