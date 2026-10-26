---
inclusion: always
---

# Database connectivity strategy (the SLeeLa connector)

SLeeLa accesses databases the way Java does: a stable **API shape** plus
pluggable **drivers** selected at connect time — the JDBC model. Build all
database work to this layering; do not bypass it or invent a parallel path.

| Java | SLeeLa — canonical home |
|---|---|
| `java.sql.*` API | `lib/database/*.sleela` facade classes |
| JDBC driver (the connector jar) | `lib/db-connector` — the C ABI `sldbc_*` |
| the database engine | `lib/sleela-sql` (local), or an external RDBMS |

## The contract

- **One driver-agnostic ABI**: `lib/db-connector/include/sldbc.h` (`sldbc_*`).
  The JDBC lifecycle: `sldbc_open(url)` → `prepare` → `bind` (1-based `?`) →
  `execute` / `query` → `ResultSet` accessors → `begin`/`commit`/`rollback` →
  `close`. New database capability is exposed **through this ABI**, not as a
  one-off.
- **The connection URL scheme selects the driver**, exactly like a JDBC URL:
  `sleelasql:<dir>`, `mysql:<...>`, `postgresql:<...>`, etc. Register every new
  backend as a scheme in the connector's driver table.
- **The bundled real driver is `sleelasql`**, wrapping the `lib/sleela-sql`
  engine (`ssql_*`): both dialects, prepared statements, snapshot transactions.
- **Honest stubs, never fakes.** A known-but-unimplemented backend returns
  `SLDBC_UNSUPPORTED`; an unknown scheme returns `SLDBC_ERR_NODRIVER`. Never
  return a connection for a backend you cannot actually reach, and never
  claim a capability (ACID, isolation, wire-protocol support) that is not
  implemented and tested. The external kinds named in `api/database`
  (postgresql / mysql / mariadb / sqlite / sqlserver / oracle / odbc) start as
  stubs and are lit up only when a real driver passes tests.

## Go-forward intent: a strong driver collection (MySQL et al.)

The deliberate plan is to grow a **first-class driver collection for the major
external databases — MySQL first, then PostgreSQL/MariaDB/SQLite and the rest**.
When implementing a new external driver:

1. Add it under `lib/db-connector` behind the existing `sldbc_*` ABI (a new
   driver implementation + its scheme), keeping the connector the single entry
   point. Do not fork a separate connector per database.
2. Treat the existing `sleelasql` driver as the reference for the lifecycle
   (open/prepare/bind/execute/query/rows/tx) and match its behaviour and error
   mapping.
3. Reuse `api/database`'s `sleela_database_config_t` (host/port/database/
   username/`password_env`/options) for connection configuration; honour
   `password_env` (read the secret from the environment, never hard-code or log
   credentials).
4. Only flip a scheme from `SLDBC_UNSUPPORTED` to active once a real
   connect + prepared-statement + result-set + transaction path works against
   that engine and a test proves it. Document durability/isolation guarantees
   honestly, as `lib/sleela-sql/ROADMAP.md` does.
5. Prefer the database vendor's documented client protocol/library as the
   dependency boundary; keep the driver's surface small and the connector ABI
   unchanged so callers are driver-agnostic.

## When wiring SLeeLa programs

The `lib/database/*.sleela` facades (`SLDatabase`, `SLPreparedStatement`,
`SLResultSet`, `SLConnectionPool`, `SLTransaction`, …) are the SLeeLa-language
front. They should ultimately call **through** `lib/db-connector`'s ABI so a
SLeeLa program uses one connector regardless of backend — the same way a Java
program uses one `java.sql` API across every JDBC driver.
