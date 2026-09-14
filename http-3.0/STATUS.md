# HTTP 3.0 Source — Present State

**As of:** 2026-09-14 · **Branch of work:** `http3-spec-flow` (off `master`)
**Scope:** making the `http-3.0/` source embody the data model and flow that
drive the HTTP 3.0 spec (`http/spec/HTTP-3.0-SPEC.md`).

This document is the point-in-time status, organized 1–2–3–4. Section 4 gives a
paragraph for each branching technology in the source. Dates are stated
relative to the "as of" date above.

---

## 1. Where this stood before

Until now (i.e. up to the day before this record, 2026-09-13), `http-3.0/` was a
**cryptographic and key-distribution substrate only**: X25519/HKDF/AES-GCM
primitives, identity capsules, per-jurisdiction capsule fan-out, monotonic route
grading, per-route session keys, and a hardened TLS key-distribution server.
That substrate was solid and self-tested, but the **application protocol the
spec actually describes was not present in code**: there was no compact
envelope (§5), no fast service/operation naming (§4), no response model (§7), no
§19 processing pipeline, and no retry classes (§9). A second gap: the Python
test (`test_crypto_http3.py`) imported Python crypto modules that do not exist in
the tree (only the C versions do), so it could not run.

## 2. What was added now (present state)

As of the "as of" date, the source now contains a **runnable, spec-faithful
application-protocol core** alongside the existing crypto substrate, plus a
demo, tests, build wiring, and this status set. The core is pure data (no
OpenSSL dependency), so it builds and tests independently. All of it is
verified: `make clean all` builds cleanly, the crypto self-test passes, the C
flow demo passes end-to-end, the Python flow tests pass, and `cpp-check`
succeeds. New files: `http3_envelope.{h,c}`, `http3_naming.{h,c}`,
`http3_pipeline.{h,c}`, `http3_protocol.h`, `http3_pipeline_demo.c`,
`http3_flow.py`, `test_http3_flow.py`. Updated: `Makefile`, `test_crypto_http3.py`
(now skips cleanly instead of failing to import), and the CI workflow.

## 3. Present completeness against the spec

| Spec section | Concept | State (2026-09-14) |
|---|---|---|
| §4 | Fast naming (name ↔ compact id, caching) | **Implemented** — `http3_naming` + `http3_flow.py` |
| §5 | Compact envelope `VERSION\|FLAGS\|SERVICE-ID\|OP-ID\|REQUEST-ID\|PAYLOAD` | **Implemented** — textual **and** binary wire |
| §6 | Request IDs (correlation without ordering) | **Implemented** — carried and echoed |
| §7 | Response model `STATUS\|REQUEST-ID\|RESULT` | **Implemented** — `http3_response_*` |
| §9 | Retry classes (READ/IDEMPOTENT/MUTATING/STREAM) | **Implemented** — per-operation; mutating-retry decision point marked |
| §13 | Resource limits (payload/envelope bounds) | **Partial** — payload capped; per-op quotas future |
| §14 | Capability negotiation (text/binary, dictionaries, compression) | **Partial** — text/binary present; negotiation handshake future |
| §17 | Administration / observability | **Partial** — request counter; full admin surface future |
| §19 | Reference processing pipeline | **Implemented** — receive→parse→svc-id→op-id→unpack→dispatch→logic→pack→response |
| §10, §11 | Streaming, compression | **Not yet** — flag bits reserved, no engine |
| §15, §16 | Java connector, RMI parity | **Not yet** — Python parity done as a proof of §20 |
| crypto | Capsules, KDS, route grading | **Pre-existing** — unchanged, still self-tests |

Net: the **hot-path data model and flow the spec centers on are now real and
demonstrable**; the remaining items (streaming, compression, negotiation
handshake, Java/RMI connectors, richer admin) are scoped as future work and are
not blockers for the flow this record covers.

## 4. Branching technologies — one paragraph each

**Compact envelope (§5).** The envelope is the unit of data that drives an HTTP
3.0 exchange. `http3_envelope.{h,c}` defines it as `VERSION | FLAGS |
SERVICE-ID | OP-ID | REQUEST-ID | PAYLOAD` and packs/unpacks it in two wire
forms. This is the spine everything else hangs from: naming supplies the ids,
the pipeline consumes the envelope, and the response mirrors its request id.

**Fast naming (§4).** `http3_naming.{h,c}` is the connection-local dictionary
that turns human service/operation names into compact numeric ids and caches
the result on first use, so later requests never re-send or re-parse long
names. Ids are deliberately connection-local and not globally stable; the
dictionary also reverses id→name for dispatch and diagnostics. This is the
branch that makes repeated calls cheap.

**Pipeline and dispatch (§19).** `http3_pipeline.{h,c}` makes the spec's hot
path explicit and traceable: receive → minimal parse → service-id lookup →
operation-id lookup → unpack → dispatch → business logic → pack → response.
Dispatch is keyed on the compact ids via a service/operation binding table, and
the pipeline owns everything around the handler so business logic stays pure.
This is the branch that turns the envelope + naming into an executed request.

**Retry classes (§9).** Each registered operation declares a retry class —
READ, IDEMPOTENT, MUTATING, or STREAM — recorded next to its handler. The
pipeline exposes the class and marks the explicit decision point where a
connector would consult an idempotency-key store before replaying a MUTATING
operation. This is the branch that lets clients reason about safe retries
rather than blindly repeating.

**Textual vs. binary wire.** The same logical envelope travels either as a
human-readable line (`H3 …`, length-prefixed payload for binary-safety) or as a
fixed big-endian binary header (22 bytes) plus raw payload, selected by a flag
and auto-detected on receive. Textual serves interoperability; binary serves
negotiated high performance. This is the branch that lets one protocol meet both
"easy to implement" and "fast" without changing the logical model.

**Cryptographic substrate (pre-existing).** The X25519/HKDF/AES-GCM primitives,
identity capsules, capsule sets, route grading, route sessions, and the TLS
key-distribution server remain as they were and still pass the crypto
self-test. The new protocol core is intentionally decoupled from it (no OpenSSL
dependency) so the data-flow layer and the security layer evolve independently
and compose at deployment time. This is the branch that carries confidentiality
and key distribution beneath the application flow.

**C ↔ Python parity.** `http3_flow.py` re-implements the envelope, naming,
response, retry classes, and pipeline in dependency-free Python, and its textual
wire output is byte-identical to the C reference (verified:
`H3 3 0 1 1 1001 5:20,22`). This is the branch that proves the spec's governing
principle (§20): a C, C++, Java, or other client can implement the connector
without becoming a SLeeLa runtime.

---

*Verification note: every "Implemented" item above was exercised by
`http3_pipeline_demo` (C) and `test_http3_flow.py` (Python) on the "as of" date;
the crypto substrate by `crypto_selftest`. See `FLOW.md` for the spec-section →
code map.*
