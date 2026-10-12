<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SQLite connector

C ABI plus C++17 RAII connection wrapper using the system SQLite C library. Prepared statements and typed text/integer/double/NULL binding are exposed directly; callers must check each SQLite return code and finalize statements. The connector opens/creates the path supplied by the caller and sets a 5-second busy timeout. No file is opened until `sleela_sqlite_open` is called.

Build with `make -C lib/connector/sqlite`; install SQLite development headers and library first. Override `SQLITE_CFLAGS` and `SQLITE_LIBS` for non-pkg-config toolchains. Use parameter binding, never interpolate untrusted values into SQL.