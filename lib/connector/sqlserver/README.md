<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# Microsoft SQL Server connector profile

Uses the shared ODBC C/C++ connector in `../odbc` with Microsoft's ODBC Driver for SQL Server installed and configured by the operator. Supply a DSN or connection string at runtime; do not embed passwords in source or commit configuration files. Enable encryption and certificate validation in the driver connection options. Prepared execution and row/column retrieval are available through the shared API; use native `SQLBindParameter` on the exposed statement handle for typed values, and maintain bound buffers/indicators for the driver's documented lifetime.

Build from the repository root with `make -C lib/connector/sqlserver build`, or from this directory with `make build`. The group build command is `make -C lib/connector build`; it requires the ODBC development package and Microsoft SQL Server ODBC driver to be installed and configured.