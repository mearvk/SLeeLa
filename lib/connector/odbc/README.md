<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian/debian-logo.png" width="75" height="75" alt="SLeeLa">

# Generic ODBC connector

Provides ODBC 3.x connection lifecycle, direct and prepared execution, row fetch, column reads through `SQLGetData`, and access to the native statement handle for `SQLBindParameter` and advanced ODBC operations. It can address Microsoft SQL Server, Oracle ODBC drivers, and other DSN/driver-backed sources by using the relevant installed driver and a caller-supplied connection string.

Bound values, buffers, and length/null indicators must remain valid for the lifetime required by the driver. Connection strings can contain secrets; use environment/configuration or a secret manager. Enable encryption and certificate validation in vendor driver options. Build with `make -C lib/connector/odbc` after installing an ODBC development package and driver manager.