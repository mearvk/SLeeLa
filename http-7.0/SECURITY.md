# SLeeLa HTTP 7.0 — Security Notes

## Implemented defenses

- Bounded request header and body buffers.
- Bounded HTTP parsing without unbounded string scanning over socket input.
- Token validation, control-byte rejection, one nonempty Host for HTTP/1.1, duplicate-header rejection, duplicate Content-Length rejection, and Transfer-Encoding rejection.
- Strict decimal Content-Length parsing with overflow checks; incomplete bodies and unexpected trailing bytes are rejected.
- Bounded response serialization and response-header validation; handlers cannot override managed framing or SLeeLa headers.
- Exact-one-frame enforcement at the H7 endpoint, payload bounds, reserved flag-bit rejection and sequence-overflow guard.
- Assertion field-count, status, size, delimiter, source and authorship validation.
- Download-state numeric overflow checks and path-like filename rejection.
- Loopback binding by default and five-second per-operation socket inactivity timeouts.
- Negotiation limited to known SLeeLa versions; fallback requires explicit opt-in.

## Not a production security boundary yet

- TCP is cleartext; no TLS or peer authentication.
- No application-level authorization model.
- Inactivity timeout is not a total request deadline; slow-trickle behavior remains a concern.
- Server is single-threaded, so one slow client can delay all other clients.
- No configurable rate limits, graceful cancellation protocol, complete stream/replay/flow-control state machine, or durable resume.
- Shared logger has not yet been wired into HTTP 7.0 ingress/egress; packet metadata observability is an outstanding task.
- Add end-to-end logger tests, fuzz/property-based tests, sanitizer runs, interoperability tests and independent security review before production use.

Assertion status and provenance are metadata supplied by applications. A status label is not a cryptographic signature, source validation, identity proof, or a guarantee that a statement is true.