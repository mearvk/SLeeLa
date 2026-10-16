<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-logo-004.jpg" alt="SLeeLa">

# SLeeLa HTTP 5.0

**Status:** Internet-capable SLeeLa application-protocol server with Friends'
Packs extension.

**Platforms:** Linux · macOS · Windows 10+

HTTP 5.0 extends the HTTP 4.0 frame/session model with an application-level
**Friends' Packs** capability — structured packaging of application
relationships, optional point values, document references, and bonus-offer
references. It is not an IETF HTTP/5 standard.

## Build and test

```sh
make            # build protocol objects + server
make test       # run server self-test (frame, FP, negotiation, download)
make server     # build just the portable server
make server-test# run the server self-test
make proto      # build protocol + FriendsPack objects only
make syntax     # syntax-only check of all sources
make clean
```

### Platform notes

| Platform       | Socket layer                | Compiler          | Link flags         |
|----------------|-----------------------------|--------------------|-------------------|
| Linux          | POSIX sockets via sleela_net| `cc` / `gcc`       | (none)            |
| macOS          | POSIX sockets via sleela_net| `cc` / `clang`     | (none)            |
| Windows 10+    | Winsock2 via sleela_net     | MSVC / MinGW       | `-lws2_32`        |

No external dependencies. C11 protocol core, C++17 FriendsPack class.

## Running the server

```sh
./http5_server --port 8084
```

Options: `--port N` (default 8084), `--self-test`, `--help`.

## Architecture

```text
Application
    |
SLeeLa HTTP 5.0 semantic API
    |
Friends' Packs                       ← FRIENDS_PACK / BONUS_OFFER / FP_UPDATE / AUDIT
    |
HTTP 5.0 frame + session model       ← 13 frame types (32-byte header)
    |
Capability / flow control / integrity
    |
Carrier adapter                      ← sleela_net portable sockets
    +-- HTTP/3 / QUIC
    +-- HTTP/4-compatible carrier
    +-- Test / in-memory carrier
```

## Frame model

32-byte big-endian header, same wire format as HTTP 4.0 with 4 additional
frame types:

| Type           | Code | Description                              |
|----------------|:----:|------------------------------------------|
| OPEN           | 1    | Establish logical request/stream          |
| DATA           | 2    | Carry application payload                 |
| END            | 3    | Complete request/stream                   |
| RESET          | 4    | Terminate stream with error               |
| WINDOW         | 5    | Advertise receive capacity                |
| PING           | 6    | Liveness probe                            |
| PONG           | 7    | Liveness response                         |
| RESUME         | 8    | Continue interrupted transfer             |
| CAPSULE        | 9    | Session/capability metadata               |
| FRIENDS_PACK   | 10   | Pack metadata + offer references          |
| BONUS_OFFER    | 11   | Optional offering reference               |
| FP_UPDATE      | 12   | Application-level FP balance update       |
| AUDIT          | 13   | Defensive conformance/audit event         |

Flags: `FIN`, `ACK`, `URGENT`, `INCREMENTAL`, `RESUMABLE`, `OPTIONAL` (0x20).

## Friends' Packs

A Friends' Pack is ordinary user-controlled application data:

- **Friend entries:** `friend-name|points|document-reference`
- **FP balance:** application quota — not an auth credential or trust score
- **Bonus offers:** references only, evaluated under local policy
- **Zero-FP behavior:** normal operation continues; new optional bonuses
  declined; ordinary requests unaffected

The C API provides `http5_fp_can_offer()` and `http5_fp_consume()`. The C++
`FriendsPack` class provides friend-list management, payload serialisation,
and file I/O.

## Built-in endpoints

| Path                      | Method | Description                              |
|---------------------------|--------|------------------------------------------|
| `/__sleela/health`        | GET    | Returns `SLeeLa HTTP/5.0 OK`            |
| `/__sleela/status`        | GET    | JSON: server, generation, all 13 frame types, FP status |
| `/__sleela/negotiate`     | GET    | Version negotiation result (JSON)        |
| `/__sleela/h5`            | POST   | H5 frame endpoint — accepts binary frames, dispatches to handler, returns response frame |

## SLeeLa application headers

```
X-SLeeLa-Version: 5.0
X-SLeeLa-Accept: 5.0, 4.0, 3.0, 2.1, 1.1, 1.0
```

## Large-file download mode

Files >50 MB: `SESSION-ID | DATETIME | FILE-ID | FILE-NAME | INDEX | OFFSET | TOTAL-SIZE`

## Security boundary

HTTP 5.0 retains the HTTP 4.0 security boundary. FP is an application quota,
not an authentication token. Bonus references do not grant access. Audit kits
are bounded conformance tools, not attack mechanisms.

## File inventory

| File                     | Description                                          |
|--------------------------|------------------------------------------------------|
| `http5_server.h`         | Server API, HTTP types, frame handler, download state|
| `http5_server.c`         | Full server: parse, route, H5 frame endpoint, self-test |
| `http5_main.c`           | CLI entry point with echo frame handler               |
| `http5_protocol.h`       | Frame types, header struct, FP helpers (reformatted)  |
| `http5_protocol.c`       | Frame encode/decode/validate, FP ops (reformatted)    |
| `http5_friends_pack.hpp` | C++ FriendsPack class                                 |
| `http5_friends_pack.cpp` | FriendsPack implementation (friend list, I/O)         |
| `http_negotiation.*`     | Negotiation API (C + C++, reformatted)                |
| `Makefile`               | `all`, `server`, `proto`, `syntax`, `test`, `clean`   |

## Dependencies

| Module              | Path                           | Role                 |
|---------------------|--------------------------------|----------------------|
| `sleela_net`        | `impl/core/sleela_net.{c,h}`   | Portable TCP sockets  |
| `preferred_router`  | `preferred-routers/`            | Routing-tier policy    |
| `resolver`          | `resolver/`                     | DNS / event policy     |

## Unified route data

This server consumes the SLeeLa unified route-data contract. VM identity:
/impl Core, /1 Foundation, /2 Operator, /3 Specialist, /4 Supervisor,
/5 Manager, /6 Director, /7 Administrator, /8 Executive, /9 Authority,
/10 Principal, /11 Sovereign.
