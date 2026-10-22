<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-logo-004.jpg" alt="SLeeLa">

# SLeeLa HTTP 6.0

**Status:** Experimental SLeeLa application-protocol server; not production-certified and not an IETF HTTP/6 standard. The reference server uses cleartext TCP and does not itself provide TLS, peer authentication, or application authorization.

HTTP 6.0 extends the HTTP 5.0 frame model with Consolidated Friends' Bet and Teamster Debate application data. Do not expose the reference server directly to untrusted networks or send sensitive data without a reviewed TLS/authentication boundary.

**Platforms:** Linux · macOS · Windows 10+

HTTP 6.0 consolidates the HTTP 5.0 Friends' Packs model into a portable
**Consolidated Friends' Bet** record — friends list, point values, document
references, team/area label, consolidated IQ value, debate material, and
optional recipient label. It is not an IETF HTTP/6 standard.

## Build and test

```sh
make            # build protocol objects + server
make test       # run server self-test
make server     # build just the portable server
make syntax     # syntax-only check of all sources
make clean
```

### Platform notes

| Platform       | Socket layer                | Compiler          | Link flags         |
|----------------|-----------------------------|--------------------|-------------------|
| Linux          | POSIX sockets via sleela_net| `cc` / `gcc`       | (none)            |
| macOS          | POSIX sockets via sleela_net| `cc` / `clang`     | (none)            |
| Windows 10+    | Winsock2 via sleela_net     | MSVC / MinGW       | `-lws2_32`        |

## Running the server

```sh
./http6_server --port 8085
# To intentionally expose the service on all IPv4 interfaces, pass: --bind 0.0.0.0
```

The server binds to `127.0.0.1` by default. Use `--bind HOST` only when you intentionally choose another interface; `0.0.0.0` exposes all IPv4 interfaces and should only be used with an appropriate network/TLS boundary.

Options: `--port N` (default 8085), `--bind HOST` (default `127.0.0.1`), `--allow-fallback` (opt in to HTTP/1.1 or HTTP/1.0 fallback), `--self-test`, `--help`. Version fallback is disabled by default.

## Frame model — 20 types

32-byte big-endian header. HTTP 5.0's 13 types plus 7 new:

| Type                        | Code | Description                          |
|-----------------------------|:----:|--------------------------------------|
| OPEN                        | 1    | Establish logical request/stream      |
| DATA                        | 2    | Carry application payload             |
| END                         | 3    | Complete request/stream               |
| RESET                       | 4    | Terminate stream with error           |
| WINDOW                      | 5    | Advertise receive capacity            |
| PING                        | 6    | Liveness probe                        |
| PONG                        | 7    | Liveness response                     |
| RESUME                      | 8    | Continue interrupted transfer         |
| CAPSULE                     | 9    | Session/capability metadata           |
| FRIENDS_PACK                | 10   | Pack metadata + offer references      |
| BONUS_OFFER                 | 11   | Optional offering reference           |
| FP_UPDATE                   | 12   | FP balance update                     |
| AUDIT                       | 13   | Conformance/audit event               |
| **CONSOLIDATED_FRIENDS_BET**| 14   | Consolidated bet record               |
| **TEAMSTER_DEBATE**         | 15   | Structured debate package             |
| **CONSOLIDATE_IQ**          | 16   | User-supplied IQ value                |
| **TEAM_AREA**               | 17   | Team/area label                       |
| **DEBATE_TOPIC**            | 18   | Debate topic                          |
| **DEBATE_POSITION**         | 19   | User-authored position/argument       |
| **RECIPIENT_LABEL**         | 20   | Optional recipient/audience label     |

Flags: `FIN`, `ACK`, `URGENT`, `INCREMENTAL`, `RESUMABLE`, `OPTIONAL`,
`USER_AUTHORED` (0x40).

## Built-in endpoints

| Path                      | Method | Description                              |
|---------------------------|--------|------------------------------------------|
| `/__sleela/health`        | GET    | Returns `SLeeLa HTTP/6.0 OK`            |
| `/__sleela/status`        | GET    | JSON: all 20 frame types, bet/debate status |
| `/__sleela/negotiate`     | GET    | Version negotiation result (JSON)        |
| `/__sleela/h6`            | POST   | H6 frame endpoint — binary frames       |

## SLeeLa application headers

```
X-SLeeLa-Version: 6.0
X-SLeeLa-Accept: 6.0, 5.0, 4.0, 3.0, 2.1, 1.1, 1.0
```

## Consolidated Friends' Bet

A user-created, non-binding application record:
- Friends list + points + document references
- Team/area label
- Consolidated IQ (user-supplied application data)
- Debate topic + position
- Recipient label

The term "bet" identifies the record. It is not a gambling service or financial
instrument.

## Teamster Debate

Structured communication package: team area → IQ → topic → position → recipient.
Recipients may accept, reject, ignore, or filter.

## Safety and authorization

All packet contents are ordinary application data. They do not authorize
interference with networks, systems, persons, property, or communications. Recipient labels are not identity claims; points and IQ fields do not grant privileges; document references do not authorize dereferencing.

See [SPEC.md](SPEC.md) for the normative project draft and [SECURITY.md](SECURITY.md) for implemented controls, deployment limits, and required release gates. The hardening workflow runs syntax/build tests across Linux, Windows, macOS Intel, and macOS ARM64; only a successful run on the exact commit counts as validation.

## File inventory

| File                          | Description                                    |
|-------------------------------|------------------------------------------------|
| `http6_server.h`             | Server API, HTTP types, frame handler           |
| `http6_server.c`             | Full server with H6 frame endpoint + self-test  |
| `http6_main.c`               | CLI entry point with echo handler               |
| `http6_protocol.h`           | 20 frame types, header struct                   |
| `http6_protocol.c`           | Frame encode/decode/validate (new)              |
| `http6_consolidated_pack.hpp`| Bounded C++ ConsolidatedFriendsBet model         |
| `http6_consolidated_pack.cpp`| UTF-8 validation + deterministic JSON serialization |
| `http6_pack_test.cpp`        | C++ model/serialization regression tests         |
| `http_negotiation.*`         | Negotiation (C + C++, reformatted)              |
| `Makefile`                   | `all`, `server`, `proto`, `syntax`, `test`      |

## Dependencies

| Module              | Path                           | Role                 |
|---------------------|--------------------------------|----------------------|
| `sleela_net`        | `impl/core/sleela_net.{c,h}`   | Portable TCP sockets  |
| `preferred_router`  | `preferred-routers/`            | Routing-tier policy    |
| `resolver`          | `resolver/`                     | DNS / event policy     |

## Unified route data

VM identity: /impl Core, /1 Foundation, /2 Operator, /3 Specialist,
/4 Supervisor, /5 Manager, /6 Director, /7 Administrator, /8 Executive,
/9 Authority, /10 Principal, /11 Sovereign.
