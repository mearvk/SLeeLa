<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-logo-004.jpg" alt="SLeeLa">

# SLeeLa HTTP 7.0

**Status:** Internet-capable SLeeLa application-protocol server with Reality
Assertion layer.

**Platforms:** Linux · macOS · Windows 10+

HTTP 7.0 introduces a **Reality Assertion** layer for carrying statements as
explicitly classified application claims. The protocol separates the fact that
a statement is transmitted from the separate question of whether that statement
is established by evidence. An HTTP 7.0 packet does not make a claim true
merely because it carries that claim. It is not an IETF HTTP/7 standard.

## Build and test

```sh
make            # build protocol objects + server
make test       # run server self-test
make server     # build just the portable server
make demo       # build the original http70.c / http70.cpp demos
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
./http7_server --port 8086
```

Options: `--port N` (default 8086), `--self-test`, `--help`.

## Frame model — 24 types

32-byte big-endian header. HTTP 6.0's 20 types plus 4 assertion frames:

| Type                        | Code | Description                          |
|-----------------------------|:----:|--------------------------------------|
| OPEN – RECIPIENT_LABEL      | 1–20 | Inherited from HTTP 6.0              |
| **ASSERTION**                | 21   | Reality assertion with classified payload |
| **ASSERTION_SOURCE**         | 22   | Source/provenance for an assertion    |
| **ASSERTION_STATUS**         | 23   | Classification status update          |
| **ASSERTION_AUTHOR**         | 24   | Authorship metadata                   |

Flags include `FACTUAL` (0x80) for documented assertions and `USER_AUTHORED`
(0x40) inherited from HTTP 6.0.

## Assertion model

Each assertion carries:

| Field                | Description                                          |
|----------------------|------------------------------------------------------|
| `status`             | Classification (see below)                           |
| `statement`          | The assertion text                                   |
| `source`             | Source used to classify it                            |
| `source_date`        | Date of the source                                   |
| `author`             | Who authored the assertion                            |
| `document_reference` | Optional supporting document                          |

### Assertion status values

| Status                | Meaning                                               |
|-----------------------|-------------------------------------------------------|
| FACTUAL_DOCUMENTED    | Established by documented evidence                     |
| USER_AUTHORED         | Authored by a user, not independently verified          |
| FICTIONAL             | Known to be fictional                                   |
| COUNTERFACTUAL        | Contradicts established evidence                        |
| DISPUTED              | Subject of active dispute                               |
| UNVERIFIED            | Not yet verified                                        |

### Serialised form (tab-delimited)

```
STATUS\tSTATEMENT\tSOURCE\tSOURCE_DATE\tAUTHOR\tDOCUMENT_REFERENCE\n
```

## Built-in endpoints

| Path                      | Method | Description                              |
|---------------------------|--------|------------------------------------------|
| `/__sleela/health`        | GET    | Returns `SLeeLa HTTP/7.0 OK`            |
| `/__sleela/status`        | GET    | JSON: 24 frame types, assertion statuses  |
| `/__sleela/negotiate`     | GET    | Version negotiation result (JSON)        |
| `/__sleela/h7`            | POST   | H7 frame endpoint — binary frames        |

## SLeeLa application headers

```
X-SLeeLa-Version: 7.0
X-SLeeLa-Accept: 7.0, 6.0, 5.0, 4.0, 3.0, 2.1, 1.1, 1.0
```

## Provenance

Applications should preserve who authored an assertion, when, the source used
to classify it, and whether it is documented, disputed, fictional,
counterfactual, or unverified.

## Security boundary

Reality assertions are information-exchange data. They do not grant authority,
change physical reality, authorize action against people or systems, establish
legal status, or provide instructions for constructing weapons. The protocol
treats assertions as data with provenance and classification.

## File inventory

| File                     | Description                                          |
|--------------------------|------------------------------------------------------|
| `http7_server.h`         | Server API, HTTP types, frame handler                 |
| `http7_server.c`         | Full server with H7 frame endpoint + self-test        |
| `http7_main.c`           | CLI entry point with echo handler                     |
| `http7_protocol.h`       | 24 frame types, assertion struct, serialise/parse API |
| `http7_protocol.c`       | Frame encode/decode, assertion serialise/parse (new)  |
| `http70.c`               | Original C demo (assertions via printf)               |
| `http70.cpp`             | Original C++ demo                                     |
| `http_negotiation.*`     | Negotiation (C + C++, reformatted)                    |
| `Makefile`               | `all`, `server`, `proto`, `demo`, `syntax`, `test`    |

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
