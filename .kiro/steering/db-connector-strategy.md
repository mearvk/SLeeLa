---
inclusion: always
---

# Database, connector, and assembly strategy

SLeeLa accesses databases and external data sources the way Java does: a stable
**API shape** plus pluggable **connectors/drivers** chosen at connect time (the
JDBC model). Build all such work to the layering below, and — critically —
**reuse the existing homes rather than duplicating them**. There are several
DB/connection-related trees in `lib/`; know which is which before adding code.

## The authoritative homes (do not duplicate)

| Concern | Canonical home | What lives there |
|---|---|---|
| External DB + data-source **drivers** (the collection) | **`lib/connector/`** | Real per-vendor adapters (C ABI + C++17 RAII) over the vendors' own client libraries, plus a connector **registry**. This is where "MySQL et al." already lives. |
| A **uniform, driver-agnostic ABI** (open-by-URL lifecycle) | `lib/db-connector/` (`sldbc_*`) | One portable surface — open/prepare/bind `?`/execute/query/ResultSet/tx — with the bundled `sleelasql` driver. A façade concept, NOT a second place for vendor drivers. |
| The SLeeLa-language DB **API shape** | `lib/database/*.sleela` | `SLDatabase`, `SLPreparedStatement`, `SLResultSet`, `SLConnectionPool`, `SLTransaction`, … (facade classes). |
| The local embedded **engine** | `lib/sleela-sql/` (`ssql_*`) | CSV-backed SQL engine: both dialects, prepared statements, snapshot transactions. |
| External-DB connection **config** | `api/database/` | `sleela_database_config_t` (host/port/database/username/`password_env`/options) + `sleela_database_validate`. |
| Driver package placeholder | `lib/drivers/` | Currently only an `SLPackage.sleela` placeholder — not a second driver tree. |

## `lib/connector/` is the driver collection — it already covers MySQL et al.

Before writing any new database or data-source connector, check
`lib/connector/CONNECTOR-CATALOG.md`. Baseline adapters already exist (C ABI +
C++17 facade, build targets present; "baseline" = not yet certified
production-ready):

- **Database:** `mysql` (MySQL C client: open/close/ping/execute/prepare/
  `MYSQL_BIND` param+result binding/fetch/affected_rows), `mariadb` (shared
  mysql adapter, MariaDB Connector/C profile), `sqlite` (SQLite C API),
  `postgres` (libpq `PQexecParams`), `odbc` (ODBC 3.x), `sqlserver` and
  `oracle` (ODBC profiles).
- **Non-database sources:** `delimited` (CSV/TSV), `json`, `xml` (libxml2),
  `rest` (libcurl), `mqtt` (Mosquitto).
- **Registry:** `connector_registry.h/.c` + C++ facade — named references,
  non-owning handles, duplicate rejection, deterministic lookup; unnamed
  resolution succeeds only when exactly one connector is registered.

Build: `make -C lib/connector build` (or `database` / `data` / `test`).

## Go-forward intent: a strong driver collection (MySQL et al.)

The deliberate plan is a first-class driver collection for the major external
databases. **That collection lives in `lib/connector/`, where it already
exists.** When advancing it:

1. **Extend the existing `lib/connector/<vendor>` adapter** (and its catalog
   entry); do NOT start a parallel connector elsewhere. If an adapter is a
   baseline, deepen it toward the production-readiness checklist in
   `CONNECTOR-CATALOG.md` (integration tests, documented parameter lifetimes /
   result ownership / transactions / timeouts, TLS + safe secrets, result
   streaming, verified Linux/macOS/Windows builds).
2. Register every connection through `connector_registry` (named references,
   duplicate rejection) so e.g. `mysql` + `postgres` never silently resolve to
   an arbitrary connection.
3. Configure connections via `api/database`'s `sleela_database_config_t`;
   honour `password_env` (read the secret from the environment — never
   hard-code or log credentials). Vendor client libraries are opt-in
   dependencies the operator installs; access begins only on explicit open.
4. **Honest capability only.** Do not claim ACID, isolation, wire-protocol
   completeness, or "production-ready" until integration tests pass against a
   real server and the behaviour is documented. A known-but-unimplemented
   backend must fail cleanly, never fake a connection. (The `lib/db-connector`
   ABI mirrors this with `SLDBC_UNSUPPORTED` / `SLDBC_ERR_NODRIVER`.)

## The uniform ABI vs. the driver collection

`lib/db-connector` (`sldbc_*`) provides ONE portable, driver-agnostic surface
(URL scheme selects the driver) and today ships only the `sleelasql` driver; it
treats external schemes as honest stubs. It must NOT grow its own copies of the
MySQL/Postgres/etc. drivers — those belong in `lib/connector/`. If a uniform
surface over the real vendor adapters is wanted, implement `lib/db-connector`
drivers as thin shims that dispatch to the corresponding `lib/connector/<vendor>`
adapter. Keep exactly one implementation of each vendor's client code.

## Terminology → where it maps

When a request mentions any of these, resolve it to the home above and reuse it:

- "DB connector", "database driver", "JDBC-like", "connector jar", "MySQL /
  MariaDB / PostgreSQL / SQLite / SQL Server / Oracle / ODBC connector",
  "data-source adapter" (CSV/TSV, JSON, XML, REST, MQTT) → **`lib/connector/`**.
- "connector registry", "driver manager", "resolve/register a connection",
  "assembly/wiring of connections", "named data source" → the
  **`connector_registry`** in `lib/connector/`.
- "uniform DB API", "open-by-URL", "driver-agnostic query/ResultSet",
  "SLeeLaSQL/embedded queries" → **`lib/db-connector`** (`sldbc_*`) and
  **`lib/sleela-sql`**.
- "SLeeLa DB classes", "SLDatabase/SLResultSet/SLPreparedStatement/pool/
  transaction objects" → **`lib/database/*.sleela`** (these should ultimately
  call through the connector layer, not reimplement it).
- "database connection config/credentials/DSN" → **`api/database`**.
