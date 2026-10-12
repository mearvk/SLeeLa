<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# PostgreSQL connector

Uses libpq, with parameterized SQL through `PQexecParams`. Parameter values are passed separately from SQL; use PostgreSQL's positional placeholders (`$1`, `$2`, …). Results are returned as owned `PGresult*` and must be released with `sleela_postgres_result_clear`. Connection strings may contain secrets; prefer environment variables or a secret manager and configure TLS with libpq's `sslmode=verify-full` and trusted CA settings for remote services.

Build with `make -C lib/connector/postgres` after installing libpq development headers. Integration against a live server is not run by the default build.