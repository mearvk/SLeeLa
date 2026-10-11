# MySQL connector

C ABI plus C++17 RAII connection and prepared-statement wrappers using mysqlclient or MariaDB Connector/C. Supports connection lifecycle, ping, one-statement execution, affected rows, last-error reporting, prepared statement creation, typed parameter/result binding through `MYSQL_BIND`, execute/store/fetch, and deterministic statement cleanup.

Build with `make -C lib/connector/mysql`. Install the client development headers/library first; `MYSQL_PKG`, `MYSQL_CFLAGS`, and `MYSQL_LIBS` can be overridden for nonstandard or Windows toolchains. The opt-in integration smoke test needs a reachable database and `SLEELA_MYSQL_HOST`, `SLEELA_MYSQL_USER`, `SLEELA_MYSQL_PASSWORD`, and optionally `SLEELA_MYSQL_DATABASE`/`SLEELA_MYSQL_PORT`.

Keep all `MYSQL_BIND` buffers and length/null indicators alive for the duration required by MySQL's statement API. Use parameter binding for untrusted values. The raw execute method discards result rows; use prepared result binding/fetch when rows are needed. Configure TLS and certificate verification through client connection options for remote servers.
