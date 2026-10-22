# HTTP 6.0 Security and Robustness

HTTP 6.0 is experimental SLeeLa application protocol code, not an IETF protocol and not production-certified. Read SPEC.md for the frame layout and limits.

## Implemented in this hardening branch

- Bounded CRLF scanning instead of unbounded strstr on socket input.
- Request-line, header-name, header-value, and HTTP-version validation.
- Strict decimal Content-Length; duplicate content-length and all transfer-encoding headers are rejected because chunked transfer is not implemented.
- Incomplete request bodies fail rather than being dispatched to application handlers.
- The H6 HTTP endpoint accepts exactly one frame, checks the decoded byte count, and bounds handler response length and sequence increment.
- Reserved frame flag bits are rejected.
- Response headers are validated and serialized through a bounded append helper to prevent response-header injection and buffer-offset overflow.
- Consolidated Friends' Bet JSON serialization enforces byte limits, validates UTF-8, and escapes JSON special/control characters.
- Synthetic tests cover key parser/serializer boundaries.

## Deployment boundary

The built-in server uses cleartext TCP. It does not implement TLS, client/server identity, authentication, per-recipient authorization, rate limiting, request deadlines, or a fully multiplexed stream state machine. Do not expose it directly to untrusted networks or transmit sensitive data until a reviewed TLS/authentication boundary and deployment controls are in place. Preferred-router and resolver hooks are not security credentials.

## Application-data rules

- Friends, points, IQ, debate fields, recipient labels, and document references are ordinary user data.
- Labels do not prove identity or consent. Points do not grant privileges. References do not authorize dereferencing or file access.
- Never treat successful parsing or protocol negotiation as authorization.
- Do not log full payloads by default; treat user-controlled labels as untrusted log content.

## Required follow-up before production

- Run exact-commit CI on Linux x86-64, Windows x86-64, macOS x86-64, and macOS arm64.
- Add fuzzing for HTTP request parsing, frame decode, and download-state parsing.
- Add socket read deadlines, bounded connection/concurrency controls, rate limiting, and graceful cancellation.
- Complete strict validation of download-state fields and numeric parsing.
- Add an authenticated TLS deployment path, authorization hooks, and end-to-end tests.
- Add protocol state validation for sequence/replay, stream lifecycle, window accounting, and per-frame semantics.
- Perform independent security review and interoperability tests.

A passing build or self-test is not a security certification.
