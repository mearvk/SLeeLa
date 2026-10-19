<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-logo-004.jpg" alt="SLeeLa">

# SLeeLa HTTP 2.0 / 2.1

**Status:** Internet-capable SLeeLa application-protocol server with compact
envelope, fast naming, and processing pipeline.

**Platforms:** Linux · macOS · Windows 10+

HTTP 2.0/2.1 extends the SLeeLa HTTP 1.0 model with compact application
envelopes, fast service/operation naming, request correlation, retry
classification, and a traceable processing pipeline. It shares the basic
design goals of HTTP 3.0 but is the earlier generation — it carries the clean
core only, without the 3.0-era per-packet integrity substrate (no keyed-MAC
DIGEST, INTACTX, NONCE, or BASKET).

## Build and test

```sh
make            # build the h21_server binary, protocol objects, negotiation objects
make test       # run the built-in self-test suite
make syntax     # syntax-only check of all sources
make proto      # build protocol-core objects only
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
./h21_server --port 8081
```

Options:
- `--port N` — TCP listen port (default 8081)
- `--self-test` — run the built-in test suite and exit
- `--help` — show usage

The server logs to stderr and runs until interrupted (SIGINT / SIGTERM on
POSIX; Ctrl-C on Windows).

## Architecture

```text
Native transport endpoint (TCP, any platform)
        |
HTTP/1.x request / connection       ← sleela_net portable sockets
        |
SLeeLa envelope / route             ← X-SLeeLa-* application headers
        |                               or H2.1 compact envelope in body
HTTP/2 stream (logical)
        |
SLeeLa logical PORT
        |
SERVICE-ID / OP-ID
        |
REQUEST-ID
        |
Pipeline dispatch → handler → response
```

Logical ports are application identifiers and are independent of native
TCP/UDP socket numbering.

## Core capabilities

| Capability                  | Status        | Notes                                |
|-----------------------------|---------------|--------------------------------------|
| Compact application envelope| Implemented   | Textual + binary wire forms          |
| Fast service/operation naming| Implemented  | Connection-local name→id dictionary   |
| Response model              | Implemented   | Transport ≠ application status        |
| Retry classification        | Implemented   | READ / IDEMPOTENT / MUTATING          |
| Processing pipeline         | Implemented   | Register, dispatch, wire handling     |
| Request correlation         | Implemented   | REQUEST-ID echo                       |
| Stream-aware multiplexing   | By design     | Logical ports above HTTP/2 streams    |
| Per-packet integrity gate   | Not in 2.1    | Added by HTTP 3.0                     |

## Wire forms

```text
Envelope (textual):  H21 <ver> <flags> <service_id> <op_id> <request_id> <len>:<payload>\n
Response (textual):  H21R <status> <request_id> <len>:<result>\n
Envelope (binary) :  [ver:1][flags:1][service_id:4][op_id:4][request_id:8][payload_len:4][payload:N]
```

The binary header is 22 bytes. The textual and binary forms are auto-detected
on the wire by checking the leading bytes for the `H21 ` ASCII tag.

## Built-in endpoints

| Path                      | Method | Description                              |
|---------------------------|--------|------------------------------------------|
| `/__sleela/health`        | GET    | Returns `SLeeLa HTTP/2.1 OK`            |
| `/__sleela/status`        | GET    | JSON: server, generation, VM, platform, pipeline status |
| `/__sleela/negotiate`     | GET    | Version negotiation result (JSON)        |
| `/__sleela/h21`           | POST   | H2.1 envelope endpoint — accepts textual or binary envelope, dispatches through the pipeline, returns textual response |

## SLeeLa application headers

Every response includes version advertisement:

```
X-SLeeLa-Version: 2.1
X-SLeeLa-Accept: 2.1, 2.0, 1.1, 1.0
```

Clients may set the following request headers for SLeeLa logical routing:

| Header                   | Purpose                                |
|--------------------------|----------------------------------------|
| `X-SLeeLa-Version`      | Requested SLeeLa protocol generation   |
| `X-SLeeLa-Accept`       | Comma-separated acceptable generations |
| `X-SLeeLa-Port`         | Logical port (application route)       |
| `X-SLeeLa-Service-ID`   | Service identifier                     |
| `X-SLeeLa-Op-ID`        | Operation identifier                   |

## Envelope endpoint usage

POST a textual or binary H2.1 envelope to `/__sleela/h21`:

```sh
# Register service "echo" op "echo" (done by default in the demo server)
# Send a textual envelope: service_id=1, op_id=1, request_id=42, payload="ping"
curl -X POST -d 'H21 2 0 1 1 42 4:ping' http://localhost:8081/__sleela/h21
# Response: H21R 0 42 4:ping
```

## Port and stream multiplexing

HTTP 2.1 defines three independent addressing layers:

| Layer                       | Purpose                                    |
|-----------------------------|--------------------------------------------|
| Native transport endpoint   | Physical/network connection endpoint       |
| HTTP/2 stream               | Multiplexed request/response channel       |
| SLeeLa logical port         | Application service/channel/route identifier|

Multiple logical ports can share the same HTTP/2 connection and its stream
pool. Stream identifiers and SLeeLa logical ports are different namespaces
and must not be conflated.

## Large-file download mode

Files larger than 50 MB use SLeeLa **DOWNLOAD** mode with resume metadata:

```
SESSION-ID | DATETIME | FILE-ID | FILE-NAME | INDEX | OFFSET | TOTAL-SIZE
```

A resumed request selects FILE-ID and its transfer INDEX/OFFSET rather than
depending on the original connection remaining open.

## What HTTP 2.1 deliberately omits (3.0-era additions)

| Feature                                    | Present? |
|--------------------------------------------|:--------:|
| Keyed-MAC DIGEST (SipHash-2-4)             | ❌        |
| INTACTX host identity / tamper RESET       | ❌        |
| Replay NONCE                               | ❌        |
| Goods & services BASKET                    | ❌        |
| Per-packet integrity gate in the pipeline  | ❌        |

## Negotiation

Generation selection is explicit. A peer must accept the proposed generation
before it is selected. Where fallback is permitted, the negotiation layer may
return to HTTP/1.1 and then HTTP/1.0. Fallback must not silently remove a
security property. See `HTTP.NEGOTIATION.md`.

## File inventory

| File                     | Description                                          |
|--------------------------|------------------------------------------------------|
| `h21_server.h`           | Server API, HTTP request/response types, download state |
| `h21_server.c`           | Full server: parse, route, envelope endpoint, self-test |
| `h21_main.c`             | CLI entry point with demo echo service                |
| `h21_protocol.h`         | Aggregation header (envelope + naming + pipeline)     |
| `h21_envelope.h`         | Envelope + response types, pack/unpack signatures     |
| `h21_envelope.c`         | Envelope init, textual/binary pack/unpack, response   |
| `h21_naming.h`           | Fast-naming dictionary types                          |
| `h21_naming.c`           | Name→id interning, lookup, reverse lookup             |
| `h21_pipeline.h`         | Pipeline types, retry classes, handler binding        |
| `h21_pipeline.c`         | Service/op registration, dispatch, wire handling      |
| `h21_flow.py`            | Python parity implementation (envelope/response)      |
| `http_negotiation.h`     | C negotiation API                                     |
| `http_negotiation.c`     | Version negotiation with policy checks                |
| `http_negotiation.hpp`   | C++ negotiation wrapper                               |
| `http_negotiation.cpp`   | C++ wrapper implementation                            |
| `Makefile`               | Build: `all`, `server`, `proto`, `syntax`, `test`, `clean` |
| `FLOW.md`                | Data-flow diagram and wire-form specification         |
| `HTTP.NEGOTIATION.md`    | Negotiation protocol for SLeeLa HTTP 1–9              |
| `STATUS.md`              | Design-goal parity table and implementation status    |

## Dependencies

| Module                      | Path                           | Role                          |
|-----------------------------|--------------------------------|-------------------------------|
| `sleela_net`                | `impl/core/sleela_net.{c,h}`   | Portable TCP sockets           |
| `preferred_router`          | `preferred-routers/`            | Routing-tier policy             |
| `resolver`                  | `resolver/`                     | DNS / path / event policy       |

## Unified route data

This server consumes the SLeeLa unified route-data contract in
`route/ROUTE.DATA.json` and `route/ROUTE.DATA.md`. Route records carry
protocol, server surface, HTTP generation, VM generation/formal VM name,
canonical configuration root, route identifier, target/resolution mode,
capability, transport, port, and status. VM names are architectural metadata
only and do not grant capabilities. Dynamic targets must use the shared
resolver before acceptance. The VM identity is: /impl Core, /1 Foundation,
/2 Operator, /3 Specialist, /4 Supervisor, /5 Manager, /6 Director,
/7 Administrator, /8 Executive, /9 Authority, /10 Principal, /11 Sovereign.
