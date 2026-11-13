<img align="right" src="../../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# http-4.0: Explanation

| | |
|---|---|
| Nature | protocol (`bin/sleelactl help http-4.0`: legal verbs `negotiate grade simulate validate info`) |
| Registry summary | HTTP/4.0 protocol metadata generation |
| Lives in | `http-4.0/` (frame codec `http4_protocol.c`, `http4_segmentation.c`, `http4_reassembly.c`, `http4_session.c`, `http4_resume.c`, `http4_packet_*.c`, server `http4_server.c`/`http4_main.c`, specs `SPEC.md` etc.) |
| Writeup type | Explanation, because the module is a specification plus a frame library whose only executable evidence is its own self-tests (it has no demo, and the server was not started), so there is no hands-on path beyond running those tests. |
| Verified | 2026-11-13 against `master` e048707fd: `bin/sleelactl validate http-4.0` (runs `make test`) passed all five self-tests. |

## What it is

SLeeLa HTTP 4.0 is **this project's own experimental design, not an IETF standard**. `README.md`, `SPEC.md` ("Experimental SLeeLa protocol draft, not an IETF HTTP/4 standard") and `STATUS.md` all say so. It replaces the envelope of `http-3.0` with an explicit **frame and stream lifecycle**: a fixed 32-byte big-endian header, nine frame types, stream/session rules, segmentation of large payloads and a resume policy. The README says it "can be carried by HTTP/3 or another supported transport", but `STATUS.md` and `CAPABILITY-MATRIX.md` are explicit that **no HTTP/3 or QUIC adapter exists**: the reference server carries frames inside HTTP/1.x over TCP, binds to `127.0.0.1` by default and provides no TLS. `UPDATE.md` records the standards boundary: the IETF defines HTTP/1.1, HTTP/2 and HTTP/3 as the core mappings and RFC 9110 defines common semantics; HTTP/4 "remains an experimental SLeeLa protocol generation".

## How it works

Frame header (SPEC.md section 2, big-endian):

| Offset | Width | Field |
|---:|---:|---|
| 0 | 1 | VERSION (must be 4) |
| 1 | 1 | TYPE (1..9: OPEN, DATA, END, RESET, WINDOW, PING, PONG, RESUME, CAPSULE) |
| 2 | 2 | FLAGS (FIN 0x01, ACK 0x02, URGENT 0x04, INCREMENTAL 0x08, RESUMABLE 0x10; other bits rejected) |
| 4 | 8 | STREAM-ID |
| 12 | 8 | REQUEST-ID |
| 20 | 8 | SEQUENCE (per stream and direction) |
| 28 | 4 | PAYLOAD-LENGTH (default maximum 16 MiB) |
| 32 | N | PAYLOAD |

Layers (README "Architecture"): application API -> frame model -> capability and session control -> segmentation/reassembly -> packet context (education + basket metadata, reusing the 14-item basket from `http-3.0`) -> carrier adapter (today only `sleela_net` TCP). Components:

* **Codec** (`http4_protocol.c`): encode/decode with validation of version, type, flags, length.
* **Segmentation / reassembly**: large payloads split over MTU-sized frames; out-of-order reassembly tracked with a bitmap.
* **Session model** (`http4_session.c`): bounded per-stream state, enforcing per-stream sequence, byte limits, duplicate OPEN and non-reused stream ids. `STATUS.md`: it is **not wired into the server's dispatch path**.
* **Resume gate** (`http4_resume.c`): validates principal/file-version/size/offset/expiry bindings and requires external chunk and final-digest verifier callbacks; it is not a cryptographic verifier or durable store.
* **Server** (`http4_server.c`): endpoints `GET /__sleela/health`, `/__sleela/status`, `/__sleela/negotiate`, `POST /__sleela/h4` (one binary frame per HTTP request), strict HTTP request framing and DOWNLOAD-metadata parsing.
* **Negotiation** (`http_negotiation.c`): a per-module copy of `sleela_http_negotiate()` (not read in detail here; `HTTP.NEGOTIATION.md` describes the shared contract: explicit acceptance, optional fallback to HTTP/1.1 then 1.0, no silent downgrade).

## What `sleelactl` does for this module

| Command | Real behaviour | Exit |
|---|---|---|
| `bin/sleelactl info http-4.0` | summary, location, `see: http-4.0/README.md`, first three `*.md` files (`CAPABILITY-MATRIX.md`, `CARRIER.md`, `CONFORMANCE.md`) | 0 |
| `bin/sleelactl validate http-4.0` | no `validate` or `syntax` target exists, so it runs `make -C http-4.0 test` | 0 |
| `bin/sleelactl negotiate http-4.0` / `grade http-4.0` | informational message only, "no standalone ... CLI is built" | 3 |
| `bin/sleelactl simulate http-4.0` | "no simulator registered (sim= unset)" | 3 |

## Where it lives

| File | Role |
|---|---|
| `SPEC.md`, `CONFORMANCE.md`, `TEST-VECTORS.md`, `CAPABILITY-MATRIX.md`, `PRODUCTION-GATES.md` | Normative draft 0.2, test plan, vectors, evidence table, release gates |
| `SECURITY.md`, `CARRIER.md`, `HANDSHAKE.md`, `RESUME.md`, `PROFILES.md`, `FLOW.md`, `UPDATE.md` | Boundaries and design notes |
| `http4_protocol.{c,h}`, `http4_selftest.c` | Codec and its self-test |
| `http4_segmentation.c`, `http4_reassembly.c`, `http4_segmentation_selftest.c` | Segmentation and reassembly |
| `http4_session.c`, `http4_session_selftest.c` | Stream-state reference model |
| `http4_resume.c`, `http4_resume_selftest.c` | Resume policy gate |
| `http4_server.c`, `http4_main.c` | HTTP/1.x reference server (`http4_server_bin`) |
| `http4_fuzz.c` | libFuzzer harness (`make fuzz`, needs clang; not run) |
| `.github/workflows/http4-conformance.yml` | CI job that runs `make -C http-4.0 clean test` |

## Status and limits

Evidence from the run (`make -C http-4.0 test`, exit 0):

```text
./http4_selftest
SLeeLa HTTP 4.0 frame self-test: PASS
./http4_segmentation_selftest
SLeeLa HTTP 4.0 segmentation/reassembly self-test: PASS
./http4_session_selftest
SLeeLa HTTP 4.0 session-state self-test: PASS
./http4_resume_selftest
SLeeLa HTTP 4.0 resume-policy self-test: PASS
./http4_server_bin --self-test
h4s_self_test: all tests passed
```

* Works: the codec, segmentation, session model, resume gate and server self-tests compile (with warnings from `preferred-routers/preferred_router.c`, e.g. misleading indentation and format-truncation) and pass.
* Documented gaps (module's own words, `STATUS.md`): session state not integrated into live dispatch; no WINDOW credit enforcement; control-frame payload schemas not validated for all nine types; no authenticated carrier, no durable resume store, no frame MAC; "Keep HTTP 4.0 experimental" until eight release gates are met. `STATUS.md` also records that the repository SHA-256 manifest gate failed on a past commit; the manifest was not touched here.
* `CAPABILITY-MATRIX.md` marks several rows "latest edits not run here"; this run shows the codec self-test passing at this commit, but the matrix itself was not updated.
* Doc defect: `README.md` contains literal `\n` text where line breaks were intended (the "Engineering specification and verification" bullet list is on one line with `\n\n- ` sequences).
* Not run: the HTTP server as a listener, `make fuzz`, the CI workflow, golden vectors from `TEST-VECTORS.md` against an independent implementation.
* Build products created by the run (untracked): `http4_selftest`, `http4_segmentation_selftest`, `http4_session_selftest`, `http4_resume_selftest`, `http4_server_bin` and ignored `*.o`; `make -C http-4.0 clean` removes them.

## Related

* [`http-3.0`](../tutorials/http-3.0.md) - the envelope generation it builds on
* [`http-5.0`](http-5.0.md) - adds four frame types for Friends' Packs and keeps the same header
* `http-4.0/SPEC.md`, `CAPABILITY-MATRIX.md`, `PRODUCTION-GATES.md`
* `http-servers/3/README.md` - the repository's separate native HTTP/3 (RFC 9114) server grade, not related to this module's code

## Sources

Files read: `bin/sleelactl.d/registry.conf`, `bin/sleelactl` (`protocol_cmd`), `http-4.0/{README.md,STATUS.md,SPEC.md (first 30 lines),CAPABILITY-MATRIX.md,UPDATE.md,Makefile}`, `.github/workflows/http4-conformance.yml`, `http-servers/README.md`.
Commands run: `bin/sleelactl info|negotiate|grade|simulate|validate http-4.0` (validate = `make -C http-4.0 test`).
