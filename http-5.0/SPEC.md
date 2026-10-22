# SLeeLa HTTP 5.0 — Core Specification (Draft 0.1)

**Protocol status:** Experimental SLeeLa project specification; not an IETF standard. Normative terms MUST, MUST NOT, SHOULD, and MAY apply only to this draft.

## 1. Scope

This draft specifies the initial fixed frame envelope and invariants. It does not define a complete web server, TLS, QUIC, browser compatibility, HTTP/1.1 syntax, or HTTP/2/3 interoperability. The current implementation is an envelope codec only.

## 2. Wire header

Every frame begins with a 40-byte header. All multibyte integers are unsigned, big-endian.

| Offset | Width | Field | Rule |
|---:|---:|---|---|
| 0 | 1 | VERSION | MUST equal 5 |
| 1 | 1 | TYPE | 1 through 11 |
| 2 | 2 | FLAGS | Only defined bits accepted |
| 4 | 4 | PAYLOAD-LENGTH | Payload bytes, excluding header |
| 8 | 8 | SESSION-ID | Session correlation; not an authentication credential |
| 16 | 8 | STREAM-ID | Logical stream identifier |
| 24 | 8 | REQUEST-ID | Request correlation identifier |
| 32 | 8 | SEQUENCE | Per-stream, per-direction sequence number |

The maximum payload in the initial C codec is 16 MiB. Deployments SHOULD choose a lower cap based on their memory budget. A decoder MUST reject unsupported versions, unknown types, reserved flags, over-limit payloads, truncated frames, and size arithmetic overflow before reading payload bytes.

The frame decoder is a single-frame primitive. A stream carrier must implement a bounded incremental parser that retains partial input and consumes exactly one frame at a time; that adapter is not yet supplied.

## 3. Initial frame types

| Code | Name | Intended semantics |
|---:|---|---|
| 1 | OPEN | Establish stream context |
| 2 | DATA | Application bytes on an open stream |
| 3 | END | Close a direction or stream according to the negotiated profile |
| 4 | RESET | Terminate a stream with a defined error |
| 5 | WINDOW | Advertise receive credit |
| 6 | PING | Liveness probe |
| 7 | PONG | PING response |
| 8 | META | Typed metadata/capability record |
| 9 | RESUME | Continue from an authenticated durable checkpoint |
| 10 | COMMIT | Confirm transfer completion after integrity validation and durable publication |
| 11 | ERROR | Structured error code and bounded diagnostic metadata |

This table is semantic intent, not a complete control-payload schema. Until each payload is formally defined and covered by vectors, a server MUST reject unsupported frame types rather than process guessed payload layouts.

## 4. Flags

Initial flags: FIN = 0x0001, ACK = 0x0002, URGENT = 0x0004, IDEMPOTENT = 0x0008, RESUMABLE = 0x0010. All other bits are reserved and MUST be rejected. The implementation currently validates the bit mask but does not yet enforce all type-specific flag combinations; that is a conformance gap.

## 5. Session, stream, and sequence invariants

- SESSION-ID identifies protocol state and MUST NOT be treated as a secret or bearer token.
- STREAM-ID and REQUEST-ID are correlation fields, not proof of authorization.
- Sequence numbers are scoped to authenticated session, stream, and direction.
- State-changing frame processing MUST validate sequence and authorization before mutating durable or externally visible state.
- Implementations MUST cap active streams, total buffered bytes, per-stream bytes, idle lifetime, and frame rate.
- Counter additions and length arithmetic MUST be checked for overflow.
- WINDOW updates MUST be bounded and cannot grant more credit than the negotiated maximum.
- RESET and END semantics must be explicit and tested before a production server accepts them.

## 6. Negotiation

A future negotiation record MUST bind both peers' offers, selected protocol generation, required capabilities, and authenticated-session transcript. If a required capability or security property is missing, negotiation MUST fail closed. No implicit downgrade to HTTP 4.0 or other generations is permitted. Any compatibility bridge must be explicitly configured and separately reviewed.

## 7. Security boundary

The codec does not encrypt or authenticate frames. Network deployments MUST use a maintained authenticated transport and MUST authorize the authenticated principal for every operation. The codec MUST NOT implement a home-grown TLS or QUIC replacement. See SECURITY.md.

## 8. Resumability and COMMIT

RESUME metadata is not authorization. Production resumption requires a cryptographically authenticated token bound to principal, transfer ID, immutable object version, expected size, digest, offset, and expiry. Checkpoints must be durable and atomic with data writes. COMMIT may be accepted only after recomputing the full-object digest and atomically publishing the completed object. None of those storage operations is implemented by the current codec.

## 9. Error behavior

A complete implementation must distinguish malformed envelope, unsupported type/version/flags, sequence violation, flow-control violation, authentication failure, authorization failure, integrity failure, resource exhaustion, expired checkpoint, and internal failure. Error diagnostics must not reveal tokens, keys, private payloads, or sensitive identity data.

## 10. Conformance and release

The initial codec must pass deterministic round-trip, golden-byte, truncation, reserved-bit, unsupported-version/type, maximum-length, overflow, and sanitizer tests. Production status additionally requires full payload schemas, C/SLeeLa shared vectors, integrated stream dispatch and flow control, authenticated carrier tests, durable resume/recovery tests, fuzzing, and independent security review.
