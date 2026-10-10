<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-logo-004.jpg" alt="SLeeLa">






# SLeeLa Native Database Connector

SLeeLa now has a native database connector contract for commonly deployed database systems: PostgreSQL, MySQL, MariaDB, SQLite, Microsoft SQL Server, Oracle Database, and ODBC-compatible databases.

Supported host platforms are Linux, Windows 8+, and macOS.

The connector vocabulary is: validate, connect, ping, query, execute, begin, commit, rollback, and close. The C facade validates bounded metadata; native client libraries and drivers remain platform-managed dependencies.

The module deliberately does not bundle database servers. Installation and upgrade adapters use the host's supported package facility and fail when a safe native package identity is unavailable.

Credentials must be supplied through environment variables or an OS credential facility. Do not put passwords in source control or command-line arguments.

See API.html and the platform adapters for deployment details.

## SLeeLa procedural surfaces — query / alter / read a table

Two SLeeLa (`#sleela 1.3`) surfaces let you query, alter, or read a table in a few lines of procedural code. Both accept statements in either dialect the engine understands — classic SQL or the fluent SLeeLaSQL — and both are thin surfaces over the connector vocabulary above; the native connector and platform driver still do the work.

- **BODI™** — [`sleela/DatabaseBodi.sleela`](sleela/DatabaseBodi.sleela): each database action is a witnessed BODI™ middle verb against an addressed table (`open`→connect, `push`→execute/alter, `pull`→query/read, `activate`/`commit`/`rollback`, `close`). Good for step-by-step work and explicit transactions.
- **Munction™** — [`sleela/DatabaseMunction.sleela`](sleela/DatabaseMunction.sleela): one bounded `db:` reach sentence that connects, sends a statement, consumes rows, latches, and closes with a witnessed receipt. The database is reached the same way every other system method is; only the `db:` scheme is new.

See [`sleela/DATABASE_REACH.md`](sleela/DATABASE_REACH.md) for the full guide, the verb mapping, and runnable examples.