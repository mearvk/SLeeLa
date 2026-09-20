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
| §5 | Compact envelope `VERSION\|FLAGS\|SERVICE-ID\|OP-ID\|REQUEST-ID\|NONCE\|DIGEST\|INTACTX\|BASKET\|PAYLOAD` | **Implemented** — textual **and** binary wire; per-packet keyed-MAC DIGEST (SipHash-2-4) + INTACTX host id + monotonic NONCE replay guard + fixed 14-item goods/services BASKET (ISO USD/g) |
| §6 | Request IDs (correlation without ordering) | **Implemented** — carried and echoed |
| §7 | Response model `STATUS\|REQUEST-ID\|RESULT` | **Implemented** — `http3_response_*` |
| §9 | Retry classes (READ/IDEMPOTENT/MUTATING/STREAM) | **Implemented** — per-operation; mutating-retry decision point marked |
| §13 | Resource limits (payload/envelope bounds) | **Partial** — payload capped; per-op quotas future |
| §14 | Capability negotiation (text/binary, dictionaries, compression) | **Partial** — text/binary present; negotiation handshake future |
| §17 | Administration / observability | **Partial** — request counter; full admin surface future |
| §19 | Reference processing pipeline | **Implemented** — receive→parse→svc-id→op-id→unpack→dispatch→logic→pack→response |
| §19 | Connection-level timing (max speed / on time / balance / carrier certainty) | **Implemented (advisory)** — `http3_timing.{h,c}`; observes arrivals, tallies late/over-rate/unbalanced, tracks carrier certainty; no wire change, no rejects |
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
wire output is byte-identical to the C reference (envelope form now
`H3 <ver> <flags> <svc> <op> <req> <digest> <intactx> <len>:<payload>`). The
per-packet DIGEST is a keyed MAC (SipHash-2-4) over a canonical big-endian field
layout, so C and Python compute the same 64-bit tag for the same key + logical
packet (verified by direct cross-check, and both against the SipHash reference
vector). This is the branch that proves the spec's governing principle (§20): a
C, C++, Java, or other client can implement the connector without becoming a
SLeeLa runtime.

**Per-packet integrity: DIGEST + INTACTX (added after 2026-09-14).** Every
HTTP 3.0 packet now carries two integrity values ahead of its payload:

- **DIGEST** — a 64-bit **keyed MAC** (SipHash-2-4, `http3_mac.{c,h}`) over the
  envelope header (excluding the transport BINARY marker) plus payload, under a
  16-byte per-connection key, sealed by `http3_envelope_init`. Because it is
  keyed, it detects not only accidental corruption but **deliberate forgery**: a
  party without the shared secret cannot compute a valid tag for a rewritten
  packet. The pipeline holds the key (`http3_pipeline_set_mac_key`, from the
  crypto substrate's key agreement in a deployment); a packet that fails
  verification is answered `BAD_DIGEST` and never dispatched. C and Python
  compute identical tags, and the implementation matches the SipHash-2-4
  reference test vector.
- **INTACTX** — a system-specific 64-bit host-integrity identity
  (`http3_intactx.{h,c}`). It is derived from a stable OS/identity baseline
  (OS name/release/arch, hostname, user), **persisted** to a baseline file so it
  survives restarts and reboots, folded with a per-emit "use-normality" sample
  (shell, cwd, term, locale). The value packs a 16-bit variance in its high bits
  over a 48-bit identity, so a larger departure from the baseline yields a
  statically larger number; a healthy, unchanged host reports variance 0. When a
  received packet's variance exceeds the pipeline threshold
  (`HTTP3_INTACTX_TAMPER_THRESHOLD`, tunable), the exchange is **RESET**
  (`TAMPERED` + a `RESET` body) and the packet is not dispatched — the "reset
  packets if the computer has been tampered with" requirement.
- **NONCE** — a per-connection monotonically increasing counter carried in the
  header and **covered by the MAC**. The pipeline keeps a high-water mark
  (`nonce_high_water`, resettable via `http3_pipeline_reset_replay_window`) and
  admits a packet only when its NONCE is strictly greater; otherwise it answers
  `REPLAYED` and does not dispatch. This defeats replay of a previously valid,
  validly MAC'd packet. Because the NONCE is inside the MAC, an attacker cannot
  bump it to evade the check without invalidating the DIGEST. (The
  single-connection reference keeps one high-water mark; a multi-sender
  deployment keys it per sender identity.)

- **BASKET** — a fixed, carefully-selected set of **14 goods and services**
  (`http3_basket.{h,c}`), atomic-bound to the United States capitalism system.
  Each item has an indivisible atomic number and an ISO value **for a Gram**,
  denominated in USD (ISO 4217 USD / 840) as integer micro-USD per gram. The
  full basket is serialized into a 172-byte canonical big-endian block that
  **travels in every HTTP 3.0 packet** and is **covered by the MAC**, so it is
  authenticated end to end and cannot be altered in transit without detection.
  The same basket appears in the human-readable `BASKET.docx` (repo root, a
  standard Office Open XML document) and in the Python reference; the C and
  Python blocks are byte-identical.

The pipeline records all three integrity events for observability (§17):
`digest_rejects`, `tamper_resets`, and `replays_rejected`. New files:
`http3_intactx.{h,c}`, `http3_mac.{h,c}`, `http3_basket.{h,c}`, and root
`BASKET.docx`. Updated: `http3_envelope.{h,c}`, `http3_pipeline.{h,c}`,
`http3_protocol.h`, `http3_pipeline_demo.c`, `http3_flow.py`,
`test_http3_flow.py`, `Makefile`, `.gitignore`, `FLOW.md`.

The DIGEST began as a fast non-cryptographic FNV-1a hash (corruption detection
only), was upgraded to the SipHash-2-4 keyed MAC described above so it also
resists deliberate tampering, and finally gained a MAC-covered NONCE so the
receiver can reject replays — the per-packet integrity method is now
authenticity **and** freshness, not just an error check.

---

*Verification note: every "Implemented" item above was exercised by
`http3_pipeline_demo` (C) and `test_http3_flow.py` (Python) on the "as of" date;
the crypto substrate by `crypto_selftest`. See `FLOW.md` for the spec-section →
code map.*
