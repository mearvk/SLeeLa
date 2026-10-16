<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-logo-004.jpg" alt="SLeeLa">

# SLeeLa HTTP 3.0

**Status:** Internet-capable SLeeLa application-protocol server with integrity
gate (keyed-MAC DIGEST, INTACTX tamper detection, NONCE replay guard, BASKET).

**Platforms:** Linux · macOS · Windows 10+

HTTP 3.0 extends the SLeeLa HTTP 2.1 application model with a per-packet
integrity substrate: keyed-MAC DIGEST (SipHash-2-4), INTACTX host-integrity
fingerprinting, NONCE replay detection, and the fixed BASKET of goods & services.
These additions sit between the parse and dispatch stages of the pipeline, forming
an integrity gate that HTTP 2.1 deliberately omits.

This is not a claim of IETF HTTP/3 semantics beyond the selected transport
carrier (HTTP/3 + QUIC may provide the transport).

## Build and test

```sh
make            # build everything: selftest, kds, proto, station, server
make server     # build just the portable HTTP 3.0 server
make server-test# run the server's built-in self-test
make proto      # build protocol-core objects only
make demo       # run the pipeline demo
make test       # run demo + station-test + Python flow tests
make station    # build station signaling
make clean
```

### Platform notes

| Platform       | Socket layer                | Compiler          | Link flags         |
|----------------|-----------------------------|--------------------|-------------------|
| Linux          | POSIX sockets via sleela_net| `cc` / `gcc`       | (none)            |
| macOS          | POSIX sockets via sleela_net| `cc` / `clang`     | (none)            |
| Windows 10+    | Winsock2 via sleela_net     | MSVC / MinGW       | `-lws2_32`        |

The crypto substrate (`selftest`, `kds`) requires OpenSSL (detected via
`pkg-config`). The protocol core, server, station, and pipeline demo do **not**
require OpenSSL — they are pure C with libc only.

## Running the server

```sh
./http3_server --port 8082
```

Options:
- `--port N` — TCP listen port (default 8082)
- `--self-test` — run the built-in test suite and exit
- `--help` — show usage

## Architecture

```text
Native transport endpoint (TCP, any platform)
        |
HTTP/1.x request / connection       ← sleela_net portable sockets
        |
SLeeLa envelope / route             ← X-SLeeLa-* headers or H3 envelope
        |
HTTP/3 stream (logical)
        |
SLeeLa logical PORT (160-bit)
        |
SERVICE-ID / OP-ID / REQUEST-ID
        |
┌─────────────────── integrity gate ───────────────────┐
│  DIGEST verify (SipHash-2-4 keyed MAC)               │
│  NONCE replay check (monotonic high-water mark)       │
│  INTACTX tamper check (host-integrity threshold)      │
│  BASKET canonical block verification                  │
└──────────────────────────────────────────────────────┘
        |
Pipeline dispatch → handler → response
```

## Core capabilities

| Capability                      | Status        | Notes                                      |
|---------------------------------|---------------|--------------------------------------------|
| Compact application envelope    | Implemented   | Textual + binary wire forms                |
| Fast service/operation naming   | Implemented   | Connection-local name→id dictionary         |
| Response model                  | Implemented   | Transport ≠ application status              |
| Retry classification            | Implemented   | READ / IDEMPOTENT / MUTATING / STREAM       |
| Processing pipeline             | Implemented   | With integrity gate                         |
| Request correlation             | Implemented   | REQUEST-ID echo                             |
| **Keyed-MAC DIGEST**            | Implemented   | SipHash-2-4, 16-byte per-connection key     |
| **INTACTX host integrity**      | Implemented   | 64-bit fingerprint, tamper threshold + RESET|
| **NONCE replay guard**          | Implemented   | Monotonic high-water mark                   |
| **BASKET**                      | Implemented   | Fixed goods & services block per packet     |
| 160-bit logical port            | Implemented   | 0 .. 10^48-1 namespace                      |
| Carrier-certainty timing        | Implemented   | Max-speed, on-time, balance estimation      |
| Capability handshake            | Implemented   | L1–L4 negotiation levels                    |
| Station signaling               | Implemented   | TCP-flag negotiation (userland + kernel)     |

## Built-in endpoints

| Path                      | Method | Description                                     |
|---------------------------|--------|-------------------------------------------------|
| `/__sleela/health`        | GET    | Returns `SLeeLa HTTP/3.0 OK`                   |
| `/__sleela/status`        | GET    | JSON: full pipeline status, integrity counters, carrier certainty |
| `/__sleela/negotiate`     | GET    | Version negotiation result (JSON)               |
| `/__sleela/h3`            | POST   | H3 envelope endpoint — textual or binary, dispatched through the integrity gate + pipeline |

## SLeeLa application headers

Every response includes:

```
X-SLeeLa-Version: 3.0
X-SLeeLa-Accept: 3.0, 2.1, 2.0, 1.1, 1.0
```

## Wire forms

```text
Envelope (textual):  H3 <ver> <flags> <svc_id> <op_id> <req_id> <port>
                        <nonce> <digest> <intactx> <basket-hex> <len>:<payload>\n
Response (textual):  H3R <status> <request_id> <len>:<result>\n
Envelope (binary) :  [ver:1][flags:1][svc_id:4][op_id:4][req_id:8][port:20]
                     [nonce:8][digest:8][intactx:8][basket:N][payload_len:4][payload:N]
```

## Integrity gate (3.0-era additions over 2.1)

| Feature                        | Description                                           |
|--------------------------------|-------------------------------------------------------|
| **DIGEST**                     | 64-bit SipHash-2-4 keyed MAC over header + basket + payload. Detects tampering. |
| **NONCE**                      | Per-connection monotonic counter. Receiver rejects stale/replayed packets.       |
| **INTACTX**                    | 64-bit host-integrity fingerprint. Variance beyond threshold triggers RESET.     |
| **BASKET**                     | Fixed canonical block of goods & services, MAC-covered.                          |

A packet whose DIGEST fails is rejected (BAD_DIGEST). A packet whose INTACTX
variance exceeds the threshold is RESET (TAMPERED, never dispatched). A packet
whose NONCE is not strictly greater than the high-water mark is rejected
(REPLAYED). All three counters are exposed in the `/__sleela/status` endpoint.

## Security boundary

Carrier security (TLS, QUIC) and SLeeLa application integrity are separate
layers. Application integrity does not replace transport encryption or
deployment authorization. Fallback must not silently weaken a required security
property (HTTP.NEGOTIATION.md).

## Large-file download mode

Files larger than 50 MB use the shared SLeeLa **DOWNLOAD** resume contract:

```
SESSION-ID | DATETIME | FILE-ID | FILE-NAME | INDEX | OFFSET | TOTAL-SIZE
```

## Dependencies

| Module                      | Path                            | Role                          |
|-----------------------------|---------------------------------|-------------------------------|
| `sleela_net`                | `impl/core/sleela_net.{c,h}`    | Portable TCP sockets           |
| `preferred_router`          | `preferred-routers/`             | Routing-tier policy             |
| `resolver`                  | `resolver/`                      | DNS / path / event policy       |
| OpenSSL (optional)          | system                           | Crypto substrate only           |

## File inventory (server additions)

| File                     | Description                                          |
|--------------------------|------------------------------------------------------|
| `http3_server.h`         | Server API, HTTP types, download state                |
| `http3_server.c`         | Full server: parse, route, H3 envelope endpoint, self-test |
| `http3_main.c`           | CLI entry point with demo echo service + MAC key      |
| `http_negotiation.h`     | C negotiation API (reformatted)                       |
| `http_negotiation.c`     | Version negotiation (reformatted)                     |
| `http_negotiation.hpp`   | C++ negotiation wrapper (reformatted)                 |
| `http_negotiation.cpp`   | C++ wrapper implementation (reformatted)              |

The existing protocol core (`http3_envelope`, `http3_naming`, `http3_pipeline`,
`http3_mac`, `http3_intactx`, `http3_basket`, `http3_timing`, `http3_handshake`,
`http3_profile`, `http3_port`), crypto substrate (`crypto_*`,
`key_distribution_server`), station signaling, Python parity layer, and pipeline
demo are unchanged.

## Unified route data

This server consumes the SLeeLa unified route-data contract in
`route/ROUTE.DATA.json` and `route/ROUTE.DATA.md`. VM identity: /impl Core,
/1 Foundation, /2 Operator, /3 Specialist, /4 Supervisor, /5 Manager,
/6 Director, /7 Administrator, /8 Executive, /9 Authority, /10 Principal,
/11 Sovereign.
