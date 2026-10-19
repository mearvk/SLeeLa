<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-logo-004.jpg" alt="SLeeLa">

# SLeeLa HTTP 8.0

**Status:** Internet-capable SLeeLa cryptographic/session-security protocol
server with National Emblems, Signals, and Frequency metadata.

**Platforms:** Linux · macOS · Windows 10+

HTTP 8.0 adds an explicit cryptographic and session-security boundary to the
SLeeLa HTTP lineage — two-way handshaking with subscription/radio state,
National Signal Frequency (NSF) metadata, binary-checker agreements,
international context, and a DarkPower session descriptor. It is not an IETF
HTTP/8 standard.

## Build and test

```sh
make            # build protocol objects + server
make test       # run server self-test (frames, session handshake, negotiation)
make server     # build just the portable server
make demo       # build original http80.c / http80.cpp + DarkPower demos
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
./http8_server --port 8087
```

Options: `--port N` (default 8087), `--self-test`, `--help`.

## Frame model — 28 types

HTTP 7.0's 24 types plus 4 session/crypto/NSF frames:

| Type                | Code | Description                              |
|---------------------|:----:|------------------------------------------|
| OPEN – ASSERTION_AUTHOR | 1–24 | Inherited from HTTP 7.0              |
| **SESSION_BEGIN**    | 25   | Initiate two-way handshake                |
| **SESSION_ACCEPT**   | 26   | Accept/reject party in handshake          |
| **NATIONAL_SIGNAL**  | 27   | NSF descriptive metadata                  |
| **CRYPTO_POLICY**    | 28   | Cryptographic policy declaration          |

Flags include `CRYPTO_REQUIRED` (0x100).

## Session model

- **Subscription state**: UNDECLARED → INACTIVE → ACTIVE → SUSPENDED
- **Radio state**: UNDECLARED → READY → NOT_READY
- **International context**: jurisdiction + process + duty
- **Binary-checker agreement**: metric + legal + social + agreement hash
- **National Signal Frequency**: emblem, signal, frequency unit/range, jurisdiction, source, timestamp
- **Two-way handshake**: begin → offer → accept → complete → can_exchange
- **Parties**: initiator + responder with safety acceptance

The session API (`http8_session_*`) is reusable C, extracted from the original
`http80.c` demo.

## DarkPower

Typed session descriptor with a fixed 24-character SCHEDULE TERM and integer
DARK POWER value (0x18ae). `contact_request_for_iss()` produces an
application-level contact-request string for domain ISS. This is a data-format
helper, not an access grant.

## Built-in endpoints

| Path                      | Method | Description                              |
|---------------------------|--------|------------------------------------------|
| `/__sleela/health`        | GET    | Returns `SLeeLa HTTP/8.0 OK`            |
| `/__sleela/status`        | GET    | JSON: 28 frame types, session/NSF/crypto status |
| `/__sleela/negotiate`     | GET    | Version negotiation result (JSON)        |
| `/__sleela/h8`            | POST   | H8 frame endpoint — binary frames        |

## SLeeLa application headers

```
X-SLeeLa-Version: 8.0
X-SLeeLa-Accept: 8.0, 7.0, 6.0, 5.0, 4.0, 3.0, 2.1, 1.1, 1.0
```

## Security boundary

Cryptographic configuration is separate from application payloads.
Implementations should fail closed when required security policy cannot be
satisfied. NSF records are descriptive metadata — they do not grant authority,
establish legal status, or provide instructions for interception/interference.

## File inventory

| File                          | Description                                    |
|-------------------------------|------------------------------------------------|
| `http8_server.h`             | Server API, HTTP types, frame handler           |
| `http8_server.c`             | Full server + session handshake self-test        |
| `http8_main.c`               | CLI entry point                                  |
| `http8_protocol.h`           | 28 frame types + session/NSF/checker types (new) |
| `http8_protocol.c`           | Frame encode/decode + session API (new)          |
| `http80.c` / `http80.cpp`   | Original demos (unchanged)                       |
| `DarkPower.hpp/.cpp`         | DarkPower session descriptor (unchanged)         |
| `http80_crypto.hpp`          | C++ crypto interface (unchanged)                 |
| `http_negotiation.*`         | Negotiation (C + C++, reformatted)               |
| `Makefile`                   | `all`, `server`, `proto`, `demo`, `syntax`, `test` |

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
