# SLeeLa HTTP 7.0 — Status

HTTP 7.0 is an experimental SLeeLa application-protocol generation, not an IETF HTTP/7 standard.

## Implemented

- 24 frame types: HTTP 6.0's 20 plus ASSERTION, ASSERTION_SOURCE, ASSERTION_STATUS and ASSERTION_AUTHOR.
- 32-byte big-endian frame encode/decode with payload bounds, known frame types, reserved-bit rejection and consumed-byte accounting.
- Reality Assertion serialization/parsing with exact field count, recognized statuses, size limits, embedded-NUL and delimiter rejection, required source for FACTUAL_DOCUMENTED and author for USER_AUTHORED.
- H7 endpoint checks classification flags against assertion status and validates status/source/author frames.
- Bounded HTTP request parsing, strict decimal Content-Length, duplicate framing/header rejection, Transfer-Encoding rejection, incomplete-body rejection, response-header validation and endpoint method policy.
- Loopback binding by default, explicit `--bind`, five-second per-operation socket inactivity timeout and opt-in negotiation fallback.
- Download-state overflow and leaf filename validation.
- Negative self-tests and four-platform CI workflow.

## Not production-ready

No TLS, peer authentication, application authorization, absolute request deadline, rate limiting, complete stream/flow-control/replay/resume implementation, or runtime shared logger bridge. The server is single-threaded. CI must pass on the exact proposed commit; see SPEC.md and SECURITY.md.
