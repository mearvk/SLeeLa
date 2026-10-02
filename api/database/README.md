<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-logo-004.jpg" alt="SLeeLa">








# SLeeLa Native Database Connector

SLeeLa now has a native database connector contract for commonly deployed database systems: PostgreSQL, MySQL, MariaDB, SQLite, Microsoft SQL Server, Oracle Database, and ODBC-compatible databases.

Supported host platforms are Linux, Windows 8+, and macOS.

The connector vocabulary is: validate, connect, ping, query, execute, begin, commit, rollback, and close. The C facade validates bounded metadata; native client libraries and drivers remain platform-managed dependencies.

The module deliberately does not bundle database servers. Installation and upgrade adapters use the host's supported package facility and fail when a safe native package identity is unavailable.

Credentials must be supplied through environment variables or an OS credential facility. Do not put passwords in source control or command-line arguments.

See API.html and the platform adapters for deployment details.