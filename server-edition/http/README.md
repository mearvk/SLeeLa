# SLeeLa Server Edition HTTP Capability

The three Server Editions use the shared SLeeLa HTTP API surface in
`api/http-multiplexing/`.

## Common capability

- Logical PORT namespace: exactly `10^48` values, represented in 20-byte
  big-endian form.
- HTTP 1.0: request/connection multiplexing model.
- HTTP 2.0 and 2.1: HTTP/2 stream multiplexing.
- HTTP 3.0: HTTP/3/QUIC stream multiplexing.
- Files larger than 50,000,000 bytes use resumable DOWNLOAD mode.
- Resume identity includes SESSION-ID, DATETIME, FILE-ID, FILE-NAME, INDEX,
  OFFSET, and TOTAL-SIZE.

The logical PORT is an application routing identifier. It is distinct from the
native TCP/UDP firewall port and from an HTTP/2 or HTTP/3 stream identifier.

## Build

From the repository root:

```sh
make -C server-edition/http
```

The resulting `httpctl` is a small native validator/descriptor built from
the authoritative API source.

## Server lifecycle

Each Discord Server Edition validates its configured logical PORT, HTTP version,
and transfer size before starting Server.sleela. Firewall lifecycle remains
owned by `server-edition/port-awareness/`; the logical PORT does not require a
new native socket.

## Edition mapping

- Discord-1™: native launcher in `api/server/sleelas.cpp`.
- Discord-2™: `server-edition/moral/2/serverd` and `src/Server.sleela`.
- Discord-3™: `server-edition/moral/3/serverd` and `src/Server.sleela`.
