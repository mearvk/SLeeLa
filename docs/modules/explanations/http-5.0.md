<img align="right" src="../../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# http-5.0: Explanation

| | |
|---|---|
| Nature | protocol (`bin/sleelactl help http-5.0`: legal verbs `negotiate grade simulate validate info`) |
| Registry summary | HTTP/5.0 protocol metadata generation |
| Lives in | `http-5.0/` (`http5_protocol.{c,h}`, `http5_server.{c,h}`, `http5_main.c`, `http5_friends_pack.{cpp,hpp}`, negotiation files, `SPEC.md`, `STATUS.md`, `SECURITY.md`, `TEST-VECTORS.md`) |
| Writeup type | Explanation, because the module is an experimental frame specification with a codec and server whose only executable evidence is its own self-test; there is no demo and the server was not started. |
| Verified | 2026-11-13 against `master` e048707fd: `make -C http-5.0 test` builds and prints `h5s_self_test: all tests passed`; `sleelactl validate http-5.0` (`make syntax`) passes. |

## What it is

SLeeLa HTTP 5.0 is **this project's own experimental application protocol, not an IETF HTTP/5 standard** (stated in `README.md`, `SPEC.md` and `STATUS.md`). It keeps the 32-byte big-endian frame header of `http-4.0` and widens the frame vocabulary from nine to **13 types** by adding `FRIENDS_PACK` (10), `BONUS_OFFER` (11), `FP_UPDATE` (12) and `AUDIT` (13). "Friends' Packs" are described as ordinary user-managed application data: friend entries, points, document references and optional bonus-offer references. The documents repeat that "FP" is an application quota only, "not an identity, trust score, authentication token, or authorization grant".

Carrier: the reference server carries HTTP 5.0 frames inside HTTP/1.x over TCP. `README.md`: it "does **not** implement HTTP/3 or QUIC and does not provide TLS or peer authentication."

## How it works

* **Frame codec** (`http5_protocol.c`): header `VERSION(1) TYPE(1) FLAGS(2) STREAM-ID(8) REQUEST-ID(8) SEQUENCE(8) PAYLOAD-LENGTH(4) PAYLOAD`; version must be 5, type 1-13, payload up to 16 MiB. Defined flags (in `http5_protocol.h`): FIN 0x01, ACK 0x02, URGENT 0x04, INCREMENTAL 0x08, RESUMABLE 0x10, OPTIONAL 0x20; any other bit is rejected. Decode consumes exactly one frame and returns the byte count; the HTTP endpoint rejects trailing bytes.
* **Friends' Pack model** (`http5_friends_pack.{hpp,cpp}`): C++ user-data model and quota helpers (`make proto` builds it as `http5_friends_pack.o`).
* **Server** (`http5_server.c`, `http5_main.c`): endpoints `GET /__sleela/health`, `/__sleela/status`, `/__sleela/negotiate` and `POST /__sleela/h5` (routes in `http5_server.c` lines 389-393). Request body ceiling is 16 MiB + 32 bytes (`HTTP5_MAX_PAYLOAD + HTTP5_FRAME_HEADER_SIZE`). It rejects duplicate or malformed Content-Length, Transfer-Encoding, truncated bodies and response-header CR/LF injection, and parses the DOWNLOAD record `SESSION-ID | DATETIME | FILE-ID | FILE-NAME | INDEX | OFFSET | TOTAL-SIZE` strictly.
* **Negotiation** (`http_negotiation.c`): per-module `sleela_http_negotiate()`; shared contract in `HTTP.NEGOTIATION.md` (explicit acceptance, optional fallback to HTTP/1.1 then 1.0, never silently weakening security).

## What `sleelactl` does for this module

| Command | Real behaviour | Exit |
|---|---|---|
| `bin/sleelactl info http-5.0` | summary, location, README pointer, three docs (`FLOW.md`, `HTTP.NEGOTIATION.md`, `README.md`) | 0 |
| `bin/sleelactl validate http-5.0` | runs `make -C http-5.0 syntax` (first existing of `validate`, `syntax`, `test`): syntax-only compile of the C sources and the C++ files; ends `✓ http-5.0 validated (syntax)` with warnings (`-Wtype-limits` at `http5_protocol.c:78`, misleading indentation in `http5_server.c:415,421`) | 0 |
| `bin/sleelactl negotiate http-5.0` / `grade http-5.0` | message only, "no standalone ... CLI is built" | 3 |
| `bin/sleelactl simulate http-5.0` | "no simulator registered (sim= unset)" | 3 |

## Where it lives

| File | Role |
|---|---|
| `SPEC.md` | Core specification, draft 0.2 (wire format, 13 types, flags, security boundary, release criteria) |
| `STATUS.md`, `SECURITY.md`, `TEST-VECTORS.md`, `FLOW.md` | Status and release gates, security boundary, golden frame vectors, flow |
| `http5_protocol.{c,h}` | Codec |
| `http5_server.{c,h}`, `http5_main.c` | Reference server, binary `http5_server` |
| `http5_friends_pack.{cpp,hpp}` | Friends' Pack model |
| `.github/workflows/http5-conformance.yml` | CI workflow for this module (not run here) |

## Status and limits

Run here (`make -C http-5.0 test`, exit 0): compiles `http5_protocol.o`, `http5_friends_pack.o`, links `http5_server` against `sleela_net`, router and resolver, then

```text
./http5_server --self-test
h5s_self_test: all tests passed
```

* `README.md` says the build targets "still require execution in CI before being called verified" and `STATUS.md` says "no new test result is claimed". This writeup is such a local result at e048707fd: `make syntax` and `make test` pass on Linux x86_64 with GCC 11.
* Gaps the module itself lists (`STATUS.md`): no per-type payload grammar or shared vectors for all 13 types, no integrated stream state machine or WINDOW credit, capability negotiation not bound to an authenticated transcript, resume parsing is not a durable authenticated store, no independent security review, nine release gates open.
* Not run: the HTTP listener and `POST /__sleela/h5`, any use of the Friends' Pack C++ model beyond compiling it, CI.
* Build products created (untracked): `http5_server`; ignored: `http5_protocol.o`, `http5_friends_pack.o`. `make -C http-5.0 clean` removes them.

## Related

* [`http-4.0`](http-4.0.md) - the nine-type predecessor, same header
* [`http-6.0`](http-6.0.md) - adds seven more frame types on top of the Friends' Pack model
* `http-5.0/SPEC.md`, `STATUS.md`, `TEST-VECTORS.md`

## Sources

Files read: `bin/sleelactl.d/registry.conf`, `bin/sleelactl` (`protocol_cmd`), `http-5.0/{README.md,STATUS.md,SPEC.md,SECURITY.md,TEST-VECTORS.md (head),Makefile,http5_protocol.h}`.
Commands run: `bin/sleelactl info|negotiate|grade|simulate|validate http-5.0`, `make -C http-5.0 test`.
