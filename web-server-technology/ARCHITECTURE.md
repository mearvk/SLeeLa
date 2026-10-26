# SLeeLa Web Server Technology — Architecture

This note explains how the dynamic SQL-backed server processes a request, how
it reaches world IP addresses, and where the SQL endpoint boundary sits.

## 1. Connection lifecycle

```
accept() ──> bounded queue ──> worker thread ──> serve(fd)
                 (QMAX=128)        (16 default)
```

- A single acceptor thread `poll()`s the listen socket with a 500 ms tick so it
  can observe `SIGINT`/`SIGTERM` and shut down gracefully.
- Accepted descriptors are handed to a **bounded work queue** drained by a fixed
  **worker pool**. When the queue is full the connection is closed immediately
  and logged as `worker-queue-full` — back-pressure instead of unbounded growth.
- Each connection has 15 s send/receive socket timeouts and honours HTTP/1.1
  keep-alive until either side signals close.

## 2. Request parsing (hardened)

The parser is the Grade-1 parser, carried over unchanged in behaviour:

- Request line split into method / target / version; only `HTTP/1.0` and
  `HTTP/1.1` accepted.
- Header block bounded (`HMAX = 64 KiB`, `NMAX = 100` lines); each name is a
  valid token; duplicate `Content-Length` rejected.
- `HTTP/1.1` requires a `Host` header.
- Bodies bounded (`BMAX = 8 MiB`). `Transfer-Encoding: chunked` is decoded with
  per-chunk size validation and a total-size cap; `Transfer-Encoding` is
  rejected for `HTTP/1.0`.

## 3. Dispatch

```
/__sleela/health           -> liveness text
/__sleela/status           -> server status JSON
/db, /db/...               -> dynamic SQL endpoint (section 5)
everything else            -> static file from document root (section 4)
```

## 4. Static files

The request path is treated as **root-relative**: leading slashes are stripped
so `/index.html` maps to `<root>/index.html`, never to the filesystem root. The
decoded path is normalized and rejected if it is absolute or contains any `..`
segment, and a canonical-prefix check confirms the resolved file still lives
under the document root. This contains path traversal (verified by
`trav-test.sh`: `..%2f..%2fetc%2fpasswd` is `403 Forbidden`).

## 5. The dynamic `/db` endpoint

```
GET  /db/health                 engine status JSON
GET  /db/query?sql=<encoded>    read-only statement -> JSON rows
POST /db/query   (body = SQL)   statement -> JSON rows (writes gated)
```

Flow:

1. Extract SQL from the `sql=` query parameter (GET) or the request body (POST).
2. Enforce policy: trim; reject empty; reject SQL longer than 8 KiB; reject a
   multi-statement list; classify the leading verb. If the verb is not a
   read verb (`SELECT`/`SHOW`/`from(`/`tables(`) and the server was not started
   with `--db-writable`, respond `403`.
3. `ssql_open` the table directory, then `ssql_exec` the statement. Engine CSV
   output is captured via `open_memstream` and converted to a JSON object of
   the form `{"columns":[...],"rows":[{...}],"row_count":N}`. JSON strings are
   escaped per RFC 8259. A successful mutation returns `{"ok":true,"mutated":true}`.
4. Engine errors map to `422 Unprocessable Entity` with
   `{"error":"<ssql_strerror>"}`.

The endpoint opens a fresh engine handle per request; the engine itself is a
file-per-table CSV store, so there is no long-lived shared connection to pool
yet. (A pooled handle is a natural future optimisation.)

## 6. Dual-stack / world IP reach

`listen_dual_stack()`:

1. Create an `AF_INET6` socket and clear `IPV6_V6ONLY`. Bound to `in6addr_any`,
   it accepts native IPv6 peers **and** IPv4-mapped (`::ffff:a.b.c.d`) peers on
   the same socket — i.e. the full range of world IP addresses.
2. If IPv6 is unavailable, or the operator passed an explicit IPv4 bind
   address, fall back to a plain `AF_INET` socket.

The server reports which mode it is in at startup
(`dual_stack=yes|no`) and in `/__sleela/status` (`"dual_stack":true`).

## 7. Observability

One synchronized log line per request:

```
request method=GET path=/db/query version=HTTP/1.1 status=200 bytes=191 \
        duration_us=95 note=db-query ok readonly=1
```

Query strings and request bodies are deliberately **not** logged.

## 8. Relationship to the SLeeLa model

`lib/web-server-technology/` carries the SLeeLa-side facade of this server:

| SLeeLa class | Models |
|---|---|
| `SLWebServer` | the server configuration + lifecycle |
| `SLSqlEndpoint` | the `/db` route policy (read-only / writable, bounds, verb classification) |
| `SLDualStackBinding` | the world-IP-reach binding descriptor |

The facade's policy decisions (e.g. which verbs are read-only) are kept
faithful to the native `statement_is_readonly` / `db_endpoint` logic, and the
runnable `demo-web-server-technology.sleela` asserts that correspondence.
