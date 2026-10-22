<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-logo-004.jpg" alt="SLeeLa">

# SLeeLa HTTP 4.0

**Status:** Experimental SLeeLa frame-based protocol foundation. Not production-ready; see STATUS.md and CAPABILITY-MATRIX.md.

**Platforms:** Linux · macOS · Windows 10+

HTTP 4.0 moves the SLeeLa model toward an explicit message and frame lifecycle.
It is not an IETF HTTP/4 standard — it is a SLeeLa experimental protocol layer
that can be carried by HTTP/3 or another supported transport.

## Build and test

```sh
make            # build selftests + server
make test       # run protocol, segmentation, session-state, and server self-tests
make session-test # run bounded per-stream state self-test
make fuzz        # optional Clang libFuzzer + ASan/UBSan parser harness
make server     # build just the portable server
make server-test# run the server self-test
make protocol-test     # run the frame protocol self-test
make segmentation-test # run the segmentation/reassembly self-test
make clean
```

### Platform notes

| Platform       | Socket layer                | Compiler          | Link flags         |
|----------------|-----------------------------|--------------------|-------------------|
| Linux          | POSIX sockets via sleela_net| `cc` / `gcc`       | (none)            |
| macOS          | POSIX sockets via sleela_net| `cc` / `clang`     | (none)            |
| Windows 10+    | Winsock2 via sleela_net     | MSVC / MinGW       | `-lws2_32`        |

No external dependencies. Pure C11 with libc only.

## Running the server

```sh
./http4_server_bin --port 8083
```

Options:
- `--port N` — TCP listen port (default 8083)
- `--self-test` — run the built-in test suite and exit
- `--help` — show usage

## Architecture

```text
Application
    |
SLeeLa HTTP 4.0 semantic API
    |
HTTP 4.0 frame model                ← OPEN/DATA/END/RESET/WINDOW/PING/PONG/RESUME/CAPSULE
    |
Capability + session control
    |
Segmentation / reassembly           ← large payloads split across MTU-sized frames
    |
Packet context (education + basket)
    |
Carrier adapter                     ← sleela_net portable sockets (Linux/macOS/Windows)
    +-- HTTP/3 / QUIC
    +-- Future native carrier
    +-- Test / in-memory carrier
```

## Engineering specification and verification\n\n- [Normative core specification](SPEC.md)\n- [Conformance and adversarial test plan](CONFORMANCE.md)
- [Canonical binary test vectors](TEST-VECTORS.md)\n- [Evidence-based capability matrix](CAPABILITY-MATRIX.md)\n- [Current implementation status](STATUS.md)\n\nThe reference server currently carries frames in HTTP/1.x over TCP. HTTP/3/QUIC support is not implemented by this directory. The added session-state model is not yet wired into the server dispatch path. Tests added in source must be run in CI before being described as passing.\n\n## Frame model

Fixed-width 32-byte header, big-endian:

```text
VERSION | TYPE | FLAGS | STREAM-ID | REQUEST-ID | SEQUENCE | PAYLOAD-LENGTH | PAYLOAD
  1B      1B     2B       8B          8B           8B          4B             N bytes
```

Frame types:

| Type      | Code | Description                          |
|-----------|:----:|--------------------------------------|
| OPEN      | 1    | Establish a logical request or stream|
| DATA      | 2    | Carry application payload            |
| END       | 3    | Complete a request or stream          |
| RESET     | 4    | Terminate a stream with error         |
| WINDOW    | 5    | Advertise receive capacity            |
| PING      | 6    | Liveness probe                        |
| PONG      | 7    | Liveness response                     |
| RESUME    | 8    | Continue an interrupted transfer      |
| CAPSULE   | 9    | Carry negotiated/session metadata     |

Flags: `FIN` (0x01), `ACK` (0x02), `URGENT` (0x04), `INCREMENTAL` (0x08),
`RESUMABLE` (0x10).

## Built-in endpoints

| Path                      | Method | Description                              |
|---------------------------|--------|------------------------------------------|
| `/__sleela/health`        | GET    | Returns `SLeeLa HTTP/4.0 OK`            |
| `/__sleela/status`        | GET    | JSON: server, generation, VM, platform, frame types, capabilities |
| `/__sleela/negotiate`     | GET    | Version negotiation result (JSON)        |
| `/__sleela/h4`            | POST   | H4 frame endpoint — accepts binary frames (32-byte header + payload), dispatches to frame handler, returns response frame or JSON metadata |

## SLeeLa application headers

Every response includes:

```
X-SLeeLa-Version: 4.0
X-SLeeLa-Accept: 4.0, 3.0, 2.1, 1.1, 1.0
```

## Core capabilities

| Capability                      | Status        | Notes                                     |
|---------------------------------|---------------|-------------------------------------------|
| Fixed-width frame header        | Implemented   | 32 bytes, big-endian                      |
| 9 frame types                   | Implemented   | OPEN through CAPSULE                      |
| Stream/request/sequence IDs     | Implemented   | Explicit protocol fields                   |
| Deterministic encode/decode     | Implemented   | Round-trip tested                          |
| Segmentation                    | Implemented   | Large payloads → MTU-sized frames          |
| Reassembly                      | Implemented   | Bitmap-tracked, out-of-order tolerant      |
| Packet context                  | Implemented   | Education token + basket                   |
| Payload wrapping                | Implemented   | Context-prefixed wire payloads             |
| Resumable delivery              | Implemented   | RESUME frame type                          |
| Incremental delivery            | Implemented   | INCREMENTAL flag                           |
| Flow control                    | Frame-level   | WINDOW frame type                          |
| Capability negotiation          | Frame-level   | CAPSULE frame type                         |

## HTTP 3.0 → HTTP 4.0

| Concern                  | HTTP 3.0                  | HTTP 4.0                          |
|--------------------------|---------------------------|-----------------------------------|
| Application envelope     | Central                   | Retained as semantic payload      |
| Stream identity          | Carrier-oriented          | Explicit protocol field           |
| Sequence handling        | Request/integrity layer   | Authenticated frame sequence      |
| Resume                   | Application convention    | Explicit RESUME frame             |
| Flow control             | Advisory/application      | Explicit WINDOW frames            |
| Incremental delivery     | Application/carrier       | Explicit DATA lifecycle           |
| Capability negotiation   | Handshake                 | Session capability model          |
| Carrier                  | Primarily HTTP/3          | Pluggable carrier                 |
| Migration                | Carrier-dependent         | Protocol session model            |
| Errors                   | Response/pipeline         | Typed stream/session RESET        |

## Large-file download mode

Files larger than 50 MB use the shared SLeeLa **DOWNLOAD** resume contract:

```
SESSION-ID | DATETIME | FILE-ID | FILE-NAME | INDEX | OFFSET | TOTAL-SIZE
```

## Security boundary

HTTP 4.0 does not replace TLS or invent a substitute for authenticated
transport. Frame validation and sequence checks are deterministic. Cryptographic
key management remains a separate concern.

## File inventory (server additions)

| File                     | Description                                          |
|--------------------------|------------------------------------------------------|
| `http4_server.h`         | Server API, HTTP types, frame handler, download state|
| `http4_server.c`         | Full server: parse, route, H4 frame endpoint, self-test |
| `http4_main.c`           | CLI entry point with echo frame handler               |
| `http_negotiation.h`     | C negotiation API (reformatted)                       |
| `http_negotiation.c`     | Version negotiation (reformatted)                     |
| `http_negotiation.hpp`   | C++ negotiation wrapper (reformatted)                 |
| `http_negotiation.cpp`   | C++ wrapper implementation (reformatted)              |

The existing protocol core (`http4_protocol`), packet context, packet payload,
segmentation, reassembly, and their self-tests are unchanged.

## Dependencies

| Module                      | Path                           | Role                          |
|-----------------------------|--------------------------------|-------------------------------|
| `sleela_net`                | `impl/core/sleela_net.{c,h}`   | Portable TCP sockets           |
| `preferred_router`          | `preferred-routers/`            | Routing-tier policy             |
| `resolver`                  | `resolver/`                     | DNS / path / event policy       |

## Unified route data

This server consumes the SLeeLa unified route-data contract in
`route/ROUTE.DATA.json` and `route/ROUTE.DATA.md`. VM identity: /impl Core,
/1 Foundation, /2 Operator, /3 Specialist, /4 Supervisor, /5 Manager,
/6 Director, /7 Administrator, /8 Executive, /9 Authority, /10 Principal,
/11 Sovereign.
