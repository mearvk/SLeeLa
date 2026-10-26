# lib/web-server-technology — SLeeLa facade for the dynamic SQL web server

This package is the SLeeLa-side model of the dynamic, SQL-backed HTTP/1.1
server whose native implementation lives in
[`web-server-technology/native`](../../web-server-technology). The facade
classes describe the server's configuration, the dynamic `/db` endpoint policy,
and the world-reachable dual-stack binding; they do not open sockets
themselves — the native server does that.

## Classes

| Class | Role |
|---|---|
| `SLPackage` | package manifest (executive-tier metadata record) |
| `SLWebServer` | server configuration + lifecycle (`configure`/`open`/`shutdownServer`/`active`), document root, port, threads, bind address, and the `/db` enable + writable flags |
| `SLSqlEndpoint` | the dynamic `/db` route policy: route prefix, read-only vs writable, statement/row bounds, and read-verb classification that mirrors the native `statement_is_readonly` logic |
| `SLDualStackBinding` | the binding descriptor; `reachesWorld()` is true only when bound to all interfaces as dual-stack IPv6+IPv4 |

## Validate

Library classes have no `main()`, so validate through the sibling demo (which
`sleela run` auto-links against every `.sleela` in this directory):

```sh
export SLEELA_SHA256_MANIFEST="$PWD/security/sha256-manifest.json"   # from repo root
impl/build/sleela check lib/web-server-technology/demo-web-server-technology.sleela
impl/build/sleela run   lib/web-server-technology/demo-web-server-technology.sleela
```

The demo asserts the policy correspondence with the native server: `SELECT` is
permitted read-only, `INSERT` is denied until writes are allowed, and a
single-literal bind drops world reach.

## Metadata

Every class carries the Descriptive-metadata block required by
`.kiro/steering/metadata-class-standard.md`. The package and facade classes are
executive-tier and cite real governing instruments in `metaNationalSchedule`
(RFC 9112 for HTTP/1.1 framing, RFC 8259 for JSON, RFC 4291 for IPv6
addressing).
