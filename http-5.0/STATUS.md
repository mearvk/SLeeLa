# SLeeLa HTTP 5.0 Status

**Overall status: Experimental; not production-ready.** HTTP 5.0 is a SLeeLa application protocol, not an IETF HTTP/5 standard.

## Present in source

- Portable HTTP/1.x-over-TCP reference server using SLeeLa's socket layer.
- 32-byte big-endian frame header and 13 enumerated frame types.
- Encode/decode/validate functions with a 16 MiB frame payload cap.
- Reserved flag-bit rejection in the frame codec.
- Friends' Pack quota helpers and C++ user-data model.
- Built-in health, status, negotiation, and frame endpoints.
- DOWNLOAD metadata serialization/parser with strict decimal parsing, bounded fields, leaf-filename checks, and offset/total-size validation.
- HTTP request framing checks for duplicate Content-Length, unsupported Transfer-Encoding, invalid Content-Length, truncated bodies, bodies without Content-Length, and bytes beyond declared length.
- HTTP frame endpoint rejects trailing bytes after a single decoded frame.
- Response-header helper rejects CR/LF injection and overlong header fields.
- Added self-test cases for reserved flags and unknown frame types.

## Important limitations

- The latest changes have not yet been compiled or run in CI; no new test result is claimed.
- The native HTTP request-body ceiling is `HTTP5_MAX_PAYLOAD + HTTP5_FRAME_HEADER_SIZE` (16 MiB + 32 bytes), matching one maximum-size frame envelope.
- The server's HTTP/1.x carrier does not provide TLS or QUIC and must not be advertised as native HTTP/3.
- No complete type-specific payload grammar or shared canonical vectors for all 13 frame types.
- No integrated persistent per-stream state machine, bidirectional sequence handling, or enforced WINDOW credit.
- Capability negotiation is not bound to an authenticated transcript; compatibility fallback must be explicitly reviewed and disabled when not intended.
- Resume parsing is not a durable, authenticated resume store and does not itself verify chunk/final object digests.
- No independent security review is recorded.

## Release gates

1. Run `make -C http-5.0 clean all syntax test` on Linux, macOS, and Windows/MinGW and fix all failures.
2. Add golden wire vectors and adversarial tests for each type, flag, parser, and boundary.
3. Add boundary tests for the 16 MiB payload and 16 MiB + 32-byte HTTP-body limits, including overflow and allocation-failure paths.
4. Integrate bounded stream lifecycle and actual send/receive flow-control accounting.
5. Define all frame payload schemas and reject unsupported semantic operations.
6. Bind negotiation to an authenticated carrier/session; no implicit downgrade.
7. Implement durable, authenticated resume with crash-safe publication and full-object digest verification.
8. Review source changes before updating the repository SHA-256 manifest; never bypass integrity verification.
9. Pass cross-platform CI, sustained fuzzing, and independent security review.

A source file or successful envelope round-trip alone is not evidence of production conformance.
