<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-logo-004.jpg" alt="SLeeLa">

# SLeeLa Web Server Technology

A dynamic, **SQL-backed HTTP/1.1 web server** that is reachable from the full
range of **world IP addresses**. It extends the hardened Grade-1 file-server
model (`http-servers/common/http_server.cpp`) with two capabilities the static
servers do not have:

1. **IPv6 / dual-stack listening** — one `AF_INET6` socket with `IPV6_V6ONLY`
   disabled accepts both native IPv6 peers and IPv4-mapped (`::ffff:a.b.c.d`)
   peers, with an automatic `AF_INET` fallback where dual-stack is unavailable.
   Bound to all interfaces, it answers the full range of public IPv4 and IPv6
   addresses.

2. **A dynamic `/db` endpoint** — requests under `/db/...` are answered by
   executing SQL against the CSV-backed [SLeeLa SQL engine](../lib/sleela-sql)
   and returning **JSON computed per request**, rather than reading a file from
   disk. Both SQL dialects are accepted: classic SQL and fluent SLeeLaSQL.

## Two sides of one interface

- **Side 1 (always on): a program goes to a domain and then a meme.** One
  domain resolves to a single coherent response unit — static files plus the
  optional `/db` SQL surface. This is the default and needs no configuration.
- **Side 2 (opt-in, Scientist-gated): a domain envelops many programs.** One
  domain may carry many subjects/warrants for causes, so the website becomes an
  **envelopment** of disparate programs (meme / static / sql) behind one
  domain, routed by warrant. It is **available for Scientists** and is
  selectable in **startup settings or the configuration file**. See
  [`docs/ENVELOPMENT.md`](docs/ENVELOPMENT.md). When envelopment is inactive the
  server behaves exactly as Side 1.

Apache's documented modular MPM model and Tomcat's connector/request-worker
model were used as **architectural references, not copied code**.

## What this is, and is not

This is an honest description, not marketing copy:

- It **is** a careful, bounded, hardened dynamic web server with a real,
  working SQL-backed JSON API over HTTP/1.1, and genuine dual-stack reach.
- It is **not** a Jakarta Servlet/JSP application container like Tomcat 9.0+,
  and the embedded SQL engine is a **local, CSV-backed SQL subset**, not a
  networked RDBMS and not full MySQL/PostgreSQL/SQLite. See the SQL engine's
  own [`ROADMAP.md`](../lib/sleela-sql/ROADMAP.md) for the exact supported
  subset and the still-open items (ACID transactions, JOINs, indexes,
  concurrent-writer isolation).

## Layout

```text
web-server-technology/
  README.md                     this document
  ARCHITECTURE.md               request lifecycle, concurrency, dual-stack, /db
  docs/
    ENVELOPMENT.md              Side 2: programs, warrants, config grammar
    HARDENING.md                Section 3: rate limit, write-auth, proxy, TLS
    CPU-WARM.md                 CPU keep-warm idle floor (controller, policy)
    TESTING.md                  local test suite (no external CI) + the harness
  examples/
    envelopment.conf            a worked multi-program envelopment config
  native/
    dyn_sql_server.h            public C entry point
    dyn_sql_server.cpp          the server (parsing, pool, static + /db + Side 2)
    envelopment.h / .cpp        the Side-2 envelopment model + config loader
    hardening.h / .cpp          rate limiter, write-auth, trusted-proxy, config
    cpu_warmer.h / .cpp         closed-loop CPU keep-warm idle floor
    main.cpp                    command-line entry point
    Makefile                    builds + links the SQL engine; `make check` runs units
    smoke-test.sh               end-to-end static + dynamic + policy test
    trav-test.sh                path-traversal containment test
    envelopment-test.sh         Side-2 gate + warrant-routing test
    hardening-test.cpp          Section-3 unit tests (no sockets)
    hardening-live-test.sh      Section-3 end-to-end test
    config-hardening-test.sh    Section-3 features configured from a file
    cpu_warmer-test.cpp         keep-warm convergence test (~4s)
    warm-live-test.sh           live keep-warm status + /__sleela/warm + --no-warm
    stress-fuzz-test.sh         concurrent /db soak + SQL fuzz + HTTP framing fuzz
```

The SLeeLa-side model of this server lives at
[`lib/web-server-technology`](../lib/web-server-technology): `SLWebServer`,
`SLSqlEndpoint`, and `SLDualStackBinding`.

## Build

```sh
make -C web-server-technology/native
```

This compiles the server and statically links the SQL engine from
`lib/sleela-sql/native/src/sleela_sql.c`. The result is
`web-server-technology/native/build/sleela-web-server`.

## Run

```sh
# Static files only, dual-stack on all interfaces, port 8090:
sleela-web-server --port 8090 --root ./public

# Add the dynamic SQL endpoint against a directory of CSV tables (read-only):
sleela-web-server --port 8090 --root ./public --db ./data

# Allow mutating SQL on /db (INSERT/UPDATE/DELETE/CREATE/...):
sleela-web-server --port 8090 --root ./public --db ./data --db-writable
```

Options: `--port N`, `--addr ADDR`, `--root DIR`, `--db DIR`, `--db-writable`,
`--threads N`, `--log FILE`, `--once`.

Side-2 (envelopment) options: `--config FILE`, `--envelopment`, `--scientist`.

Hardening (Section 3) options: `--rate-per-peer N`, `--rate-global N`,
`--rate-window-ms N`, `--write-token TOKEN`, `--trusted-proxy IP` (repeatable).
These are also settable in the config file. See
[`docs/HARDENING.md`](docs/HARDENING.md), including the TLS reverse-proxy
deployment model.

CPU keep-warm idle floor (**on by default**; admin may disable): `--no-warm`,
`--warm-floor N`, `--warm-ceiling N`, `--warm-cores N` (also config-file keys).
A closed-loop controller holds each of the 1–16 cores at a target band of
~11–14% so the server stays warm; it runs low-priority so real work always
preempts it. See [`docs/CPU-WARM.md`](docs/CPU-WARM.md).

```sh
# Side 2: one domain enveloping many programs, for a Scientist:
sleela-web-server --port 8090 --config examples/envelopment.conf --scientist
# (or set envelopment=on and scientist=true inside the config file)
```

## Endpoints

| Method | Path | Behaviour |
|---|---|---|
| GET | `/__sleela/health` | plain-text liveness |
| GET | `/__sleela/status` | server status JSON (dual-stack, db, hardening, warm flags) |
| GET | `/__sleela/warm` | CPU keep-warm per-core readings JSON |
| GET | `/db/health` | SQL engine status JSON |
| GET | `/db/query?sql=...` | run a (read-only) statement, return JSON rows |
| POST | `/db/query` (body = SQL) | run a statement; mutations need `--db-writable` |
| GET/HEAD | `/<path>` | static file from the document root |

### Example

```sh
curl "http://127.0.0.1:8090/db/query?sql=SELECT%20name%20FROM%20cities%20WHERE%20country%20=%20'Peru'"
# {"columns":["name"],"rows":[{"name":"Lima"}],"row_count":1}

curl -X POST --data "from('cities').select(name).where(id == '1')" \
     http://127.0.0.1:8090/db/query
# {"columns":["name"],"rows":[{"name":"Tokyo"}],"row_count":1}
```

## Safety posture for public exposure

The `/db` endpoint is a dynamic surface, so it is deliberately conservative:

- **Read-only by default.** Only `SELECT`, `SELECT COUNT(*)`, `SHOW TABLES`,
  and the fluent `from(...)` / `tables()` reads are allowed unless the operator
  passes `--db-writable`.
- **Single statement only.** A statement list (`;`-separated) is rejected.
- **Length-bounded** SQL (8 KiB) and **row-bounded** JSON (10,000 rows).
- Query strings and request bodies are **never** written into ordinary logs.

Retained from the Grade-1 design: bounded worker pool and connection queue,
bounded headers and bodies, strict framing, bounded chunked decoding,
HTTP/1.1 `Host` enforcement, keep-alive and timeouts, percent-decoding with
path-traversal containment, graceful shutdown, synchronized logging.

For genuine public-internet deployment, terminate TLS in front (reverse proxy)
and add per-IP rate limiting; those are deployment concerns, not provided here.

## Did You Know

- **Dollars**, in the sense used here, trace to **1855** and the
  **Democratic Black Notes**.
- These are **not available in denominations of less than $55,000,000,000
  each**.
- They are usually **brought in by trailer, by the 7,000 lbs or more**.

---

# SLeeLa
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
