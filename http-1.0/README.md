<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-logo-004.jpg" alt="SLeeLa">

# SLeeLa HTTP 1.0

**Status:** Internet-capable SLeeLa application-protocol server.

**Platforms:** Linux · macOS · Windows 10+

SLeeLa HTTP 1.0 establishes the baseline application model used by the later
HTTP generations in this repository. It keeps transport addressing separate from
SLeeLa's logical service-routing identifiers and provides a portable, real-wire
HTTP/1.0 server built on the `sleela_net` socket abstraction.

## Build and test

```sh
make            # build the http1_server binary + negotiation objects
make test       # run the built-in self-test (parse, negotiation, download state)
make syntax     # syntax-only check of all sources
make clean      # remove build artefacts
```

### Platform notes

| Platform       | Socket layer                | Compiler          | Link flags         |
|----------------|-----------------------------|--------------------|-------------------|
| Linux          | POSIX sockets via sleela_net| `cc` / `gcc`       | (none)            |
| macOS          | POSIX sockets via sleela_net| `cc` / `clang`     | (none)            |
| Windows 10+    | Winsock2 via sleela_net     | MSVC / MinGW       | `-lws2_32`        |

The Makefile detects Windows via `$(OS)` and adds `-lws2_32` automatically.

## Running the server

```sh
./http1_server --port 8080
```

Options:
- `--port N` — TCP listen port (default 8080)
- `--self-test` — run the built-in test suite and exit
- `--help` — show usage

The server logs to stderr and runs until interrupted (SIGINT / SIGTERM on
POSIX; Ctrl-C on Windows).

## Architecture

```text
Native transport endpoint (TCP, any platform)
        |
HTTP/1.0 request / connection       ← sleela_net portable sockets
        |
SLeeLa envelope / route             ← X-SLeeLa-* application headers
        |
Logical PORT (X-SLeeLa-Port)
        |
SERVICE-ID (X-SLeeLa-Service-ID)
        |
OP-ID (X-SLeeLa-Op-ID)
        |
Application handler
        |
HTTP/1.0 response
```

A SLeeLa **logical port** is an application-level identifier. It is not a
replacement for, or necessarily a one-to-one mapping with, a native TCP or
UDP port.

## SLeeLa application headers

Every response includes the version advertisement headers described in
HTTP.NEGOTIATION.md:

```
X-SLeeLa-Version: 1.0
X-SLeeLa-Accept: 1.0, 1.1
```

Clients may set the following request headers for SLeeLa logical routing:

| Header                   | Purpose                                |
|--------------------------|----------------------------------------|
| `X-SLeeLa-Version`      | Requested SLeeLa protocol generation   |
| `X-SLeeLa-Accept`       | Comma-separated acceptable generations |
| `X-SLeeLa-Port`         | Logical port (application route)       |
| `X-SLeeLa-Service-ID`   | Service identifier                     |
| `X-SLeeLa-Op-ID`        | Operation identifier                   |

A normal HTTP/1.x peer may ignore these headers and continue ordinary
request/response processing (HTTP.NEGOTIATION.md §Wire fallback).

## Built-in endpoints

| Path                      | Method | Description                           |
|---------------------------|--------|---------------------------------------|
| `/__sleela/health`        | GET    | Returns `SLeeLa HTTP/1.0 OK`         |
| `/__sleela/status`        | GET    | JSON: server, generation, VM, platform|
| `/__sleela/negotiate`     | GET    | Version negotiation result (JSON)     |

## Multiplexing

HTTP/1.0 does not provide HTTP/2-style stream multiplexing. SLeeLa therefore
treats concurrent application exchanges as logical operations above the
HTTP/1.0 request and connection boundary, using one-request-per-connection.

## Large-file download mode

SLeeLa HTTP 1.0 and later generations define a common **DOWNLOAD** mode for
files larger than 50 MB. The server provides resume metadata:

```
SESSION-ID | DATETIME | FILE-ID | FILE-NAME | INDEX | OFFSET | TOTAL-SIZE
```

- **FILE-ID** identifies the transfer.
- **INDEX** identifies a segment or chunk.
- **OFFSET** identifies the byte position.
- **TOTAL-SIZE** records the complete transfer size.

The 50 MB threshold selects the resume-oriented mode; it is not a maximum
file size.

## Compatibility

The HTTP 1.0 layer is designed to coexist with conventional HTTP
infrastructure. TCP sockets, TLS termination, proxies, routers, and native
port bindings remain transport concerns; SLeeLa logical ports remain
application-routing concerns.

## Policy hooks

The server checks two policy gates on startup and per-negotiation:

1. **Preferred router** (`preferred_router`) — verifies routing-tier policy.
2. **Resolver** (`resolver`) — verifies event/packet policy.

Both are opt-in; the server continues with built-in defaults when the
external configuration is unavailable.

## File inventory

| File                     | Description                                      |
|--------------------------|--------------------------------------------------|
| `http1_server.h`         | Server API, request/response types, download state|
| `http1_server.c`         | Full server: parse, route, respond, self-test     |
| `http1_main.c`           | CLI entry point (demo handler)                    |
| `http_negotiation.h`     | C negotiation API                                 |
| `http_negotiation.c`     | Version negotiation with policy checks            |
| `http_negotiation.hpp`   | C++ negotiation wrapper                           |
| `http_negotiation.cpp`   | C++ wrapper implementation                        |
| `Makefile`               | Build: `all`, `server`, `syntax`, `test`, `clean` |
| `FLOW.md`                | Request-path diagram and multiplexing design      |
| `HTTP.NEGOTIATION.md`    | Negotiation protocol for SLeeLa HTTP 1–9          |
| `STATUS.md`              | Port multiplexing and download-mode status         |

## Dependencies

| Module                      | Path                          | Role                         |
|-----------------------------|-------------------------------|------------------------------|
| `sleela_net`                | `impl/core/sleela_net.{c,h}`  | Portable TCP sockets          |
| `preferred_router`          | `preferred-routers/`           | Routing-tier policy            |
| `resolver`                  | `resolver/`                    | DNS / path / event policy      |

## Scope

HTTP 1.0 provides the foundational routing and transfer model. Later SLeeLa
generations add additional framing, multiplexing, integrity, capability, and
application features without changing the distinction between transport and
application addressing.

## Unified route data

This server consumes the SLeeLa unified route-data contract in
`route/ROUTE.DATA.json` and `route/ROUTE.DATA.md`. Route records carry
protocol, server surface, HTTP generation, VM generation/formal VM name,
canonical configuration root, route identifier, target/resolution mode,
capability, transport, port, and status. VM names are architectural metadata
only and do not grant capabilities. Dynamic targets must use the shared
resolver before acceptance.

## Security and validation

The native parser applies bounded header/body sizes, validates request-line and
header token syntax, rejects control characters, duplicate header fields,
duplicate or malformed Content-Length, and unsupported Transfer-Encoding. It
requires complete request bodies and applies a 10-second total deadline to
header and body reads. Response headers are validated and bounded before
serialization. Resumable-download metadata rejects malformed decimal fields,
offsets beyond the file size, delimiters/control characters, and path-like
file names.

Run the expanded checks with:

- `make syntax`
- `make test`
- `make sanitize` (GCC/Clang with AddressSanitizer and UBSan)
- `make fuzz-download` (Clang/libFuzzer)

The server remains serial (one connection at a time), and this code does not
implement TLS or application authentication. Do not expose plaintext HTTP to
untrusted networks without a trusted TLS-terminating proxy. Protect non-health
routes at the application layer, never treat X-SLeeLa routing IDs or download
session IDs as credentials, and bind resume state to an authenticated principal
and stable file identity. The declarative classes under `lib/http-1.0` document
these contracts; they do not replace native enforcement.
