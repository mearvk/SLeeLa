<img align="right" src="../../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# http-9.0: Explanation

| | |
|---|---|
| Nature | protocol (`bin/sleelactl help http-9.0`: legal verbs `negotiate grade simulate validate info`) |
| Registry summary | HTTP/9.0 experimental protocol generation (not IETF) |
| Lives in | `http-9.0/` (`http90.{c,h}`, `http90.{cpp,hpp}`, `HTTP90.SPEC.md`, `HTTP90.conf`, `http_negotiation.{c,h,cpp,hpp}`, `README.md`, `STATUS.md`, `TESTING.md`) |
| Writeup type | Explanation, because the module is a metadata model and specification (a C struct API plus syntax-only build), and its documented `make demo` target fails to link, so no operable end-to-end path exists. |
| Verified | 2026-11-13 against `master` e048707fd: `bin/sleelactl validate http-9.0` and `make -C http-9.0 test` pass (syntax only); `make -C http-9.0 demo` fails (`undefined reference to 'main'`); scratch programs exercised `http90_*` and `sleela_http_negotiate`. |

## What it is

SLeeLa HTTP 9.0 is **an experimental project design that is explicitly not an IETF standard**: "not an IETF HTTP/9 standard and does not replace HTTP/1.1 or HTTP/2/3" (`HTTP90.SPEC.md` line 3; same in `README.md`, `STATUS.md`, the registry summary and `HTTP.NEGOTIATION.md`). It is not a wire protocol: `TESTING.md` says "an explicitly documented packet serialization format" is still needed, and `STATUS.md` says the syntax target "does not claim full link, runtime, wire-level, or interoperability conformance". It extends the HTTP 8.0 metadata model with structured records: protocol grade, sequence, prior-packet metadata (4096-byte text), police and international identifiers, optional monitoring frequencies, national signal/frequency provenance, international security/safety/police-category data, and a "Dark Band" record. The specification repeats that these are descriptive fields and "not commands or grants of authority"; real police frequencies are deliberately not embedded.

## How it works

The authoritative layout is the C header `http90.h` (the spec says so). Main types: `http90_packet_metadata` (grade `char[32]`, `prior_packet_metadata[4096]`, `identity`, `monitoring`, `national_signal_frequency`, `international_data`, `dark_band`, `uint64_t sequence`), with fixed buffer sizes (`HTTP90_MAX_ID` 128, `HTTP90_MAX_TEXT` 256, `HTTP90_MAX_FREQUENCY` 64). Status codes: `HTTP90_OK` 0, `INVALID_ARGUMENT` 400, `UNAUTHORIZED` 401, `FORBIDDEN` 403, `INVALID_METADATA` 422, `NOT_CONFIGURED` 428. Functions: `http90_init`, `_set_identity`, `_set_frequency`, `_set_national_signal_frequency`, `_set_international_data`, `_set_dark_band`, `_validate`, `_can_transmit`, `_can_receive`. `http90.hpp` wraps them in a `sleeLa::http90::PacketMetadata` class.

Defaults (`HTTP90.conf`, spec section 3): identifiers are `POLICE-ID-UNASSIGNED` / `INTERNATIONAL-ID-UNASSIGNED`, monitoring frequencies are `DISABLED` and not enabled, and an enabled frequency whose value is `DISABLED` must fail with `HTTP90_NOT_CONFIGURED`. The default Dark Band content is `0x4441524B42414E44` (the ASCII bytes of "DARKBAND"), described as "a public identifier/content representation, not a credential, cryptographic key, authorization token, or hidden channel".

**Negotiation** (`http_negotiation.c`, 7 lines): `sleela_http_negotiate(requested, peer_accept, allow_fallback)` first consults the resolver (`"HTTP/9.0"`) and router (`"HTTP/9.0"`, 4) policy hooks, then returns `requested` if the peer's list contains it, else (only if fallback is allowed) `"1.1"`, then `"1.0"`, else NULL.

### Observed behaviour (scratch programs, not committed)

Compiled against `http90.c` in a temporary directory:

```text
init: grade='HTTP/9.0' police_id='POLICE-ID-UNASSIGNED' dark_band='Dark Band' '0x4441524B42414E44'
scanner freq='DISABLED' enabled=0
validate(default)=422 can_transmit=0 can_receive=0
set_frequency(enabled, DISABLED)=0
validate after=428
set_identity(NULL,..)=400
set_identity(overlong)=0
validate(null)=400
after filling international + national-signal data: validate=0 can_transmit=1 can_receive=1
```

So a default record is invalid (422) until the international and national-signal data are set; enabling a monitoring frequency whose value is `DISABLED` fails closed with 428 at `validate`; null arguments give 400. An overlong identifier (399 characters into a 128-byte field) was **accepted**: `http90_set_identity` returned 0 and stored 127 characters, i.e. silent truncation. `STATUS.md` lists "reject truncated identifiers where truncation could change meaning" as still required before deployment, so this matches the module's own to-do list.

Negotiation, compiled with the router and resolver sources:

```text
requested=9.0 accept='9.0, 1.1' fallback=0 -> 9.0
requested=9.0 accept='1.1, 1.0' fallback=0 -> (null)
requested=9.0 accept='1.1, 1.0' fallback=1 -> 1.1
requested=9.0 accept='1.0' fallback=1 -> 1.0
requested=9.0 accept='8.0' fallback=1 -> (null)
requested=9.0 accept='19.0' fallback=0 -> (null)
```

## What `sleelactl` does for this module

| Command | Real behaviour | Exit |
|---|---|---|
| `bin/sleelactl info http-9.0` | summary "HTTP/9.0 experimental protocol generation (not IETF)", location, README pointer, docs `HTTP90.SPEC.md`, `HTTP.NEGOTIATION.md`, `README.md` | 0 |
| `bin/sleelactl validate http-9.0` | runs `make -C http-9.0 syntax` (4 syntax-only compiles: `http90.c`, `http_negotiation.c`, `http90.cpp`, `http_negotiation.cpp`); ends `✓ http-9.0 validated (syntax)` | 0 |
| `bin/sleelactl negotiate http-9.0` / `grade http-9.0` | informational text only; no code runs | 3 |
| `bin/sleelactl simulate http-9.0` | "no simulator registered (sim= unset)" | 3 |

`bin/SLEELACTL.md` uses `sleelactl validate http-9.0` as its example for protocol validation; it checks syntax only.

## Where it lives

| File | Role |
|---|---|
| `HTTP90.SPEC.md` | Application-level spec (7 sections) |
| `HTTP90.conf` | Safe placeholder configuration (`DISABLED` frequencies, `UNASSIGNED` ids) |
| `http90.h`, `http90.c` | C metadata API and validation |
| `http90.hpp`, `http90.cpp` | C++ wrapper `PacketMetadata` |
| `http_negotiation.{c,h,cpp,hpp}` | Version negotiation adapters |
| `TESTING.md`, `STATUS.md` | Required test matrix and open items |
| `Makefile` | `all` (= `syntax`), `syntax`, `demo`, `test`, `clean` |

## Status and limits

* **Works:** `make syntax` and `make test` pass. `make test` only repeats the syntax checks and prints "Runtime/conformance tests require a configured test harness; see TESTING.md." There are no runtime tests in the module.
* **Broken: `make demo`.**

  ```text
  $ make -C http-9.0 demo
  cc -std=c11 -O2 -Wall -Wextra -Wpedantic -DHTTP90_STANDALONE_DEMO -I. http90.c -o http90_demo
  /usr/bin/ld: .../crt1.o: in function `_start': (.text+0x1b): undefined reference to `main'
  collect2: error: ld returned 1 exit status
  make: *** [Makefile:21: http90_demo] Error 1
  ```

  The Makefile defines `HTTP90_STANDALONE_DEMO`, but neither `http90.c` nor `http90.cpp` contains `main` or that macro, so no demo exists. The C++ variant (`c++ -std=c++17 -I. http90.cpp http90.c`) fails with the same missing `main`. `make demo` is also not mentioned in `README.md` or `STATUS.md`, which list only `make syntax` and `make test`.
* **Not done (module's own list, `STATUS.md`):** runtime tests for every setter and validation branch, truncation rejection, router/resolver integration check per platform, fuzzing, verified fallback/downgrade behaviour, review of authentication, authorization, integrity, privacy, replay, logging and secret handling.
* No CI workflow for HTTP 9.0 exists under `.github/workflows/`.
* Build products: none remain (the failed demo build left no binary; syntax checks write nothing).

## Related

* [`http-8.0`](../tutorials/http-8.0.md) - the session/crypto/NSF generation it extends
* `http-9.0/HTTP90.SPEC.md`, `TESTING.md`, `HTTP.NEGOTIATION.md`
* [`../../../bin/SLEELACTL.md`](../../../bin/SLEELACTL.md)

## Sources

Files read: `bin/sleelactl.d/registry.conf` (line 55), `bin/sleelactl` (`protocol_cmd`), `bin/SLEELACTL.md`, `http-9.0/{README.md,STATUS.md,TESTING.md,HTTP90.SPEC.md,HTTP90.conf,http90.h,http90.hpp,http_negotiation.c,Makefile}`.
Commands run: `bin/sleelactl info|negotiate|grade|simulate|validate http-9.0`, `make -C http-9.0 test`, `make -C http-9.0 demo` (fails), scratch C programs for `http90_*` and `sleela_http_negotiate` (built in temporary directories and deleted).
