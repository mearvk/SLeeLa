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
  native/
    dyn_sql_server.h            public C entry point
    dyn_sql_server.cpp          the server (parsing, pool, static + /db)
    main.cpp                    command-line entry point
    Makefile                    builds and links the lib/sleela-sql engine
    smoke-test.sh               end-to-end static + dynamic + policy test
    trav-test.sh                path-traversal containment test
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

## Endpoints

| Method | Path | Behaviour |
|---|---|---|
| GET | `/__sleela/health` | plain-text liveness |
| GET | `/__sleela/status` | server status JSON (dual-stack, db flags) |
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

---

# SLeeLa
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
