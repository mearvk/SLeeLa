# SLeeLa HTTP 4.0 — Normative Core Specification (Draft 0.2)

**Status:** Experimental SLeeLa protocol draft, not an IETF HTTP/4 standard. The key words MUST, MUST NOT, REQUIRED, SHOULD, and MAY are normative within this project.

## 1. Layering and security boundary

HTTP 4.0 defines a 32-byte frame header and stream/session semantics. It does not replace TLS, QUIC, TCP, congestion control, or peer certificates. An untrusted-network deployment MUST use an authenticated, integrity-protected carrier. The carrier adapter MUST expose whether authentication and handshake completion succeeded. State-changing operations MUST NOT be processed as replayable early data unless protected by a durable idempotency mechanism.

The current native server uses HTTP/1.x over TCP. It is not an HTTP/3/QUIC adapter. The carrier capability MUST be reported accurately.

## 2. Wire format

All multibyte integers are unsigned and big-endian. A frame consists of:

| Offset | Width | Field | Rule |
|---:|---:|---|---|
| 0 | 1 | VERSION | MUST equal 4 |
| 1 | 1 | TYPE | 1..9 |
| 2 | 2 | FLAGS | Only bits 0x0001..0x0010 are defined |
| 4 | 8 | STREAM-ID | Logical stream identifier |
| 12 | 8 | REQUEST-ID | Correlation identifier |
| 20 | 8 | SEQUENCE | Monotonically increasing per stream and direction |
| 28 | 4 | PAYLOAD-LENGTH | Payload bytes, excluding the 32-byte header |
| 32 | N | PAYLOAD | Exactly PAYLOAD-LENGTH bytes |

A decoder MUST reject unknown versions, unknown types, undefined flag bits, payloads over the configured maximum, truncated frames, invalid type-specific payloads, and arithmetic overflow. It MUST validate lengths before allocation or copying. A stream-oriented carrier may deliver partial or multiple frames; the adapter MUST retain partial bytes and consume exactly one frame at a time.

The native default maximum frame payload is 16 MiB. Deployments SHOULD configure a lower bound appropriate to their memory budget. HTTP body and frame payload limits are separate.

## 3. Frame types

| Code | Type | Semantics |
|---:|---|---|
| 1 | OPEN | Creates stream/request context; duplicate active stream IDs are rejected |
| 2 | DATA | Carries bytes only on an open, non-reset stream |
| 3 | END | Half-closes the sender direction; later DATA in that direction is rejected |
| 4 | RESET | Terminates the stream; subsequent frames are rejected unless a future profile permits acknowledgements |
| 5 | WINDOW | Advertises bounded receive credit; updates MUST be overflow-checked |
| 6 | PING | Liveness probe |
| 7 | PONG | Response to PING |
| 8 | RESUME | Continues an authenticated transfer at a verified checkpoint |
| 9 | CAPSULE | Carries bounded control/capability metadata |

Defined flags: FIN=0x01, ACK=0x02, URGENT=0x04, INCREMENTAL=0x08, RESUMABLE=0x10. Unknown bits MUST be rejected. Undefined type/flag combinations MUST be rejected.

## 4. Sequence and stream invariants

Sequence numbers are scoped to (authenticated session, stream ID, direction). A receiver MUST verify the exact next sequence for an ordered stream profile before state changes or dispatch. It MUST NOT use a single connection-wide high-water mark for multiplexed streams. Sequence state resets only for a new authenticated stream/session; identifiers MUST NOT be reused while old state can still be accepted. The current bounded native reference profile additionally requires stream IDs to increase monotonically within a session, allowing closed slots to be reclaimed without retaining an unbounded tombstone set.

Implementations MUST enforce active-stream, per-stream byte, global queued-byte, idle-timeout, and cleanup limits. Counters and additions MUST be checked for overflow. Flow-control credit MUST be consumed before accepting DATA and returned only when the application releases buffer capacity.

## 5. Capability negotiation and downgrade resistance

Capabilities are the intersection of local and peer-supported bits. A peer MUST NOT infer HTTP 4.0 features from HTTP/3 or another carrier. The handshake MUST be bound to the authenticated session and include both peers' offers, protocol generation, selected profile, and a transcript integrity check. If the required security profile is unavailable, negotiation MUST fail closed; legacy fallback is allowed only when explicitly configured and policy permits it. Fallback MUST be observable.

Unknown mandatory bits MUST be rejected. A profile MUST NOT be enabled when dependencies are absent.

## 6. Integrity and identity

The frame codec does not authenticate frames. Production deployments MUST rely on an authenticated carrier and MUST authorize the authenticated principal for each service/operation. If an application-layer MAC profile is added, its specification MUST define algorithm, canonical bytes, key derivation, key identifiers, rotation, constant-time verification, replay state, and test vectors. SipHash's 64-bit tag MUST NOT be presented as encryption or a substitute for TLS/QUIC peer authentication.

Host fingerprint/INTACTX data is not identity authentication. A field merely present or marked “verified” by a caller is not proof of cryptographic verification.

## 7. Segmentation and reassembly

Each segment MUST bind to a single (session, stream, request, transfer ID) and agree on total length and segment count. Receivers MUST reject duplicate indexes, overlapping byte ranges, out-of-range offsets, inconsistent FIRST/LAST flags, impossible counts, and reassembly beyond configured per-transfer and global memory budgets. Reassembly MUST be bounded by bytes and active-transfer counts, with expiry and cleanup. Out-of-order arrival MAY be supported, but application data MUST NOT be released as complete until every byte is present and integrity verification succeeds.

## 8. Resumable transfers

A resume token/checkpoint MUST be authenticated and bound to principal, transfer ID, immutable file/version identity, expected total size, accepted offset, and integrity digest. The receiver MUST reject unknown, stale, or inconsistent checkpoints and MUST NOT silently skip bytes. Chunk boundaries and checksums MUST be verified, and the final object digest MUST be checked before atomic publication. Resume MUST be idempotent or protected against duplicate side effects. SESSION-ID, DATETIME, FILE-ID, FILE-NAME, INDEX, OFFSET, and TOTAL-SIZE are metadata, not authorization credentials.

The 50 MiB threshold selects resumable mode; it is not a maximum transfer size. Deployment quotas MUST cap file size and temporary storage.

## 9. Errors and observability

Errors MUST distinguish malformed frame, unsupported version/type/flags, sequence violation, flow-control violation, authorization failure, integrity failure, resource exhaustion, expired resume state, and internal failure. Diagnostics MUST NOT expose keys, tokens, private payloads, or sensitive identity data. Metrics and logs MUST be bounded and avoid attacker-controlled unbounded labels.

## 10. Conformance

Implementations MUST pass the vectors and adversarial tests in CONFORMANCE.md, plus fuzzing and sanitizer runs for native parsers. Cross-language implementations MUST produce identical bytes for the same frame. Production status additionally requires cross-platform CI, carrier security verification, persistent-resume tests, resource exhaustion tests, and independent security review.
