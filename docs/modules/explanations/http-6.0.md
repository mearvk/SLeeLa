<img align="right" src="../../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# http-6.0: Explanation

| | |
|---|---|
| Nature | protocol (`bin/sleelactl help http-6.0`: legal verbs `negotiate grade simulate validate info`) |
| Registry summary | HTTP/6.0 protocol metadata generation |
| Lives in | `http-6.0/` (`http6_protocol.{c,h}`, `http6_server.{c,h}`, `http6_main.c`, `http6_consolidated_pack.hpp`, negotiation files, `README.md`, `STATUS.md`, `FLOW.md`) |
| Writeup type | Explanation, because the module is an experimental application-data model with a codec and server whose only executable evidence is its own self-test; there is no demo and no spec beyond README/STATUS/FLOW, and the server was not started. |
| Verified | 2026-11-13 against `master` e048707fd: `make -C http-6.0 test` builds and prints `h6s_self_test: all tests passed`; `sleelactl validate http-6.0` (`make syntax`) passes. |

## What it is

SLeeLa HTTP 6.0 is **this project's own experimental generation, not an IETF HTTP/6 standard** (`README.md`, `STATUS.md`). It consolidates the `http-5.0` Friends' Packs into a **"Consolidated Friends' Bet"** record: friends list, point values, document references, a team/area label, a user-supplied "consolidated IQ" value, debate topic and position, and an optional recipient label. The README states the word "bet" only names the record: "It is not a gambling service or financial instrument", and that packet contents are ordinary application data that do not authorize interference with anything. `FLOW.md` frames the debate option as "a structured communication format, not an instruction to pressure, threaten, attack, or interfere with a recipient"; recipients may accept, reject, ignore or filter.

It keeps the 32-byte big-endian frame header and extends the frame vocabulary to **20 types**: the 13 of `http-5.0` plus `CONSOLIDATED_FRIENDS_BET` (14), `TEAMSTER_DEBATE` (15), `CONSOLIDATE_IQ` (16), `TEAM_AREA` (17), `DEBATE_TOPIC` (18), `DEBATE_POSITION` (19), `RECIPIENT_LABEL` (20). The enum in `http6_protocol.h` (lines 11-18) matches the README table. A new flag `USER_AUTHORED` (0x40) is documented.

## How it works

`FLOW.md` gives the order: establish an authenticated carrier, negotiate HTTP 6.0, open a logical stream, import or build the HTTP 5.0 pack, add consolidated metadata and optional debate fields, serialize as ordinary content, frame it, send only to an explicitly selected recipient, receiver validates and may accept/reject/ignore/filter, `AUDIT` records conformance events without altering network behaviour.

The server (`http6_server.c`) exposes `GET /__sleela/health`, `/__sleela/status`, `/__sleela/negotiate` and `POST /__sleela/h6` (lines 202-205) and advertises `X-SLeeLa-Version: 6.0` with `X-SLeeLa-Accept: 6.0, 5.0, 4.0, 3.0, 2.1, 1.1, 1.0` (README). Like the neighbouring generations it carries frames in HTTP/1.x over `sleela_net` sockets and implements the shared DOWNLOAD resume record. `http6_consolidated_pack.hpp` is the C++ data model for the record.

## What `sleelactl` does for this module

| Command | Real behaviour | Exit |
|---|---|---|
| `bin/sleelactl info http-6.0` | summary, location, README pointer, docs `FLOW.md`, `HTTP.NEGOTIATION.md`, `README.md` | 0 |
| `bin/sleelactl validate http-6.0` | runs `make -C http-6.0 syntax` (syntax-only compile of `http6_protocol.c`, `http6_server.c`, `http6_main.c`, `http_negotiation.c` and `http_negotiation.cpp`); ends `✓ http-6.0 validated (syntax)`, with many `-Wmisleading-indentation` warnings in `http6_server.c` | 0 |
| `bin/sleelactl negotiate http-6.0` / `grade http-6.0` | message only, "no standalone ... CLI is built" | 3 |
| `bin/sleelactl simulate http-6.0` | "no simulator registered (sim= unset)" | 3 |

## Where it lives

| File | Role |
|---|---|
| `README.md`, `STATUS.md`, `FLOW.md`, `HTTP.NEGOTIATION.md` | The only documentation (there is no `SPEC.md`, `SECURITY.md` or vector file in this directory) |
| `http6_protocol.{c,h}` | Frame codec for 20 types |
| `http6_server.{c,h}`, `http6_main.c` | Reference server, binary `http6_server` |
| `http6_consolidated_pack.hpp` | C++ model of the consolidated pack |
| `build/Makefile`, `build/negotiation.mk` | Local build fragments |

## Status and limits

Run here (`make -C http-6.0 test`, exit 0):

```text
./http6_server --self-test
h6s_self_test: all tests passed
```

* `STATUS.md` is short and lists only "Verification targets: add conformance tests for bounded field lengths, malformed packs, negotiation, explicit fallback, and serialization round trips." It does not state a test result. It also describes the Makefile as a "C/C++ syntax-check build", while the Makefile in fact has a `test` target that links and runs the server self-test (this is why the self-test result above exists).
* The README header calls the server "Internet-capable"; the module has no TLS or authentication code that this run could find evidence of, and `FLOW.md` step 1 simply assumes "an authenticated carrier". Treat that wording as unverified.
* Not run: the HTTP listener and `POST /__sleela/h6`, the C++ pack model (`http6_consolidated_pack.hpp` is not included by any `.c`/`.cpp` file in `http-6.0/` and no Makefile target compiles it, so nothing in this checkout builds or tests it), any conformance beyond the self-test, CI (there is no `http6-*` workflow under `.github/workflows/`).
* Build products created (untracked): `http6_server`; ignored: `http6_protocol.o`. `make -C http-6.0 clean` removes them.

## Related

* [`http-5.0`](http-5.0.md) - Friends' Packs, 13 frame types
* [`http-7.0`](../tutorials/http-7.0.md) - adds four assertion frames on top of the 20 types
* `http-6.0/README.md`, `FLOW.md`

## Sources

Files read: `bin/sleelactl.d/registry.conf`, `bin/sleelactl` (`protocol_cmd`), `http-6.0/{README.md,STATUS.md,FLOW.md,Makefile,http6_protocol.h}`, `http6_server.c` lines 200-206, `.github/workflows` directory listing.
Commands run: `bin/sleelactl info|negotiate|grade|simulate|validate http-6.0`, `make -C http-6.0 test`.
