# Production hardening (Section 3)

The dynamic web server ships four in-process hardening controls plus a
documented TLS boundary. All are off/open by default (so Side-1 and Side-2
behaviour is unchanged) and configurable both as **startup flags** and in the
**configuration file**; the command line overrides the file.

## 1. Rate limiting (admission control)

A sliding-window limiter beyond the connection-queue back-pressure. Two
independent windows:

- **per-peer**: at most N requests per window per client IP;
- **global**: at most N requests per window across all peers.

A limit of `0` disables that dimension. Over-limit requests get `503` with
`Connection: close` and are **not** counted (a flood does not keep the window
pinned full). Aged-out per-peer windows are swept periodically to bound memory.

```sh
--rate-per-peer N        # per-client cap   (0 = off)
--rate-global N          # global cap       (0 = off)
--rate-window-ms N       # window, default 1000
```
```text
# or in the config file:
rate-per-peer  = 60
rate-global    = 5000
rate-window-ms = 1000
```

## 2. Write authorization (bearer token)

When a token is set, a **mutating** `/db` statement must present
`Authorization: Bearer <token>`. Reads are unaffected. The comparison is
**constant time** over the longer of the two strings, so a token is not leaked
by response timing. A missing/incorrect token yields `401`.

```sh
--write-token TOKEN
```
```text
write-token = s3cr3t
```

This composes with `--db-writable`: writes require **both** that mutations are
permitted at all *and* a valid token.

## 3. Trusted-proxy client identity (for a TLS terminator)

When the server runs behind a reverse proxy that terminates TLS, the immediate
TCP peer is the proxy, not the client. Listing the proxy as trusted makes the
server honour the proxy's `Forwarded` / `X-Forwarded-For` header to recover the
real client IP (used for rate-limiting and logging). A `Forwarded` header from a
peer that is **not** an allowlisted proxy is ignored, so clients cannot spoof
their address.

```sh
--trusted-proxy 127.0.0.1    # repeatable
```
```text
trusted-proxy = 127.0.0.1
trusted-proxy = 10.0.0.1
```

The left-most entry of an `X-Forwarded-For` list is taken as the originating
client (what the trusted terminator recorded first).

## 4. TLS 1.3 — deployment model

The server does **not** terminate TLS in-process. Hand-rolling a TLS stack
would be the wrong call for a server that aims to be honest about its security
posture. Instead, terminate TLS at a dedicated, well-audited front end and run
this server as the trusted upstream:

```
Internet ──TLS 1.3──> reverse proxy (nginx / Caddy / Envoy) ──HTTP──> sleela-web-server
                        │  terminates TLS, sets X-Forwarded-For       │  --trusted-proxy <proxy-ip>
                        │  (and ideally Forwarded)                     │  --rate-per-peer / --write-token
```

Minimal nginx example:

```nginx
server {
    listen 443 ssl http2;
    server_name science.example;
    ssl_certificate     /etc/ssl/science.example.crt;
    ssl_certificate_key /etc/ssl/science.example.key;
    ssl_protocols       TLSv1.3;

    location / {
        proxy_pass         http://127.0.0.1:8090;
        proxy_set_header   Host              $host;
        proxy_set_header   X-Forwarded-For   $remote_addr;
        proxy_set_header   X-Forwarded-Proto $scheme;
    }
}
```

Then start the server trusting the terminator:

```sh
sleela-web-server --port 8090 --addr 127.0.0.1 \
  --root ./public --db ./data --db-writable \
  --trusted-proxy 127.0.0.1 --rate-per-peer 60 --write-token "$WRITE_TOKEN"
```

Binding the server to `127.0.0.1` (or a private interface) ensures it is only
reachable through the terminator. The server's dual-stack world-IP reach is then
provided at the TLS edge, which is where it belongs for a public deployment.

For HTTP/3, the repository's Grade-3 server already delegates QUIC/TLS to an
external backend (`http-servers/3`); the same reverse-proxy pattern applies.

## Status and logging

`/__sleela/status` reports `rate_limit`, `write_auth`, and `trusted_proxies`.
Every request log line now carries `client=<ip>` (the effective client, after
trusted-proxy resolution). Query strings, bodies, and tokens are never logged.

## Still outstanding (tracked, not delivered)

- The `lib/sleela-sql` engine's own ROADMAP items — ACID transactions,
  concurrent-writer isolation, JOINs/richer predicates — remain a separate
  effort and are *not* claimed here.
- CI wiring and large-scale stress/fuzz tests.
- Real IPv6 validation on a dual-stack host (only IPv4 was exercisable in the
  build environment).

## Tests

- `native/hardening-test.cpp` — unit tests for the limiter, constant-time
  compare, bearer-token parse, and trusted-proxy resolution (`make check`).
- `native/hardening-live-test.sh` — end-to-end rate limit, write-token, and
  trusted-proxy anti-spoofing against the running server.
- `native/config-hardening-test.sh` — the same features configured from a file.
