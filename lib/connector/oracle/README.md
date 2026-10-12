<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# Oracle Database connector profile

Uses the shared ODBC C/C++ connector in `../odbc` with an Oracle-supported ODBC driver/driver manager configured by the operator. This profile does not bundle Oracle Instant Client or OCI. Use wallet/driver TLS configuration and external secret management for credentials. Prepared execution and row/column retrieval are available through the shared API; use native `SQLBindParameter` on the exposed statement handle for typed values, and maintain bound buffers/indicators for the driver's documented lifetime.

Build from the repository root with `make -C lib/connector/oracle build`, or from this directory with `make build`. The group build command is `make -C lib/connector build`; it requires the ODBC development package and Oracle ODBC driver to be installed and configured.