# SLeeLa HTTP 5.0 — Core Specification (Draft 0.2)

**Status:** Experimental SLeeLa application protocol. Not an IETF HTTP/5 standard. Normative terms apply only to this project draft.

## 1. Scope and current implementation

HTTP 5.0 is a SLeeLa-specific application protocol carried by an HTTP/1.x server in the current reference implementation. Its frame codec is not HTTP/3 or QUIC. The current code includes a portable HTTP server, a frame envelope, Friends' Packs application helpers, and a basic negotiation path. It is not production-ready and does not provide transport encryption or peer authentication.

## 2. Wire format

Every frame has a 32-byte header followed by exactly PAYLOAD-LENGTH bytes. Multibyte values are unsigned big-endian.

| Offset | Width | Field |
|---:|---:|---|
| 0 | 1 | VERSION (must equal 5) |
| 1 | 1 | TYPE (1–13) |
| 2 | 2 | FLAGS |
| 4 | 8 | STREAM-ID |
| 12 | 8 | REQUEST-ID |
| 20 | 8 | SEQUENCE |
| 28 | 4 | PAYLOAD-LENGTH |
| 32 | N | PAYLOAD |

The maximum frame payload in the current codec is 16 MiB. The HTTP server request-body ceiling is `HTTP5_MAX_PAYLOAD + HTTP5_FRAME_HEADER_SIZE` (16 MiB + 32 bytes), allowing one maximum-size frame envelope per request. This limit does not imply support for multi-frame request bodies; the endpoint rejects trailing bytes.

The codec rejects an unsupported version, unknown type, undefined flag bits, payloads above the codec limit, and truncated payloads. Decode consumes one frame and reports the number of bytes consumed. The HTTP endpoint MUST reject trailing bytes because it accepts one frame per HTTP request.

## 3. Frame types

| Code | Type | Current intent |
|---:|---|---|
| 1 | OPEN | Open logical stream |
| 2 | DATA | Application payload |
| 3 | END | End stream or message |
| 4 | RESET | Terminate stream |
| 5 | WINDOW | Flow-control advertisement |
| 6 | PING | Liveness probe |
| 7 | PONG | Liveness response |
| 8 | RESUME | Resume-transfer metadata |
| 9 | CAPSULE | Session/capability metadata |
| 10 | FRIENDS_PACK | Friends' Pack application data |
| 11 | BONUS_OFFER | Optional offer reference |
| 12 | FP_UPDATE | Application quota update |
| 13 | AUDIT | Defensive conformance/audit event |

The envelope does not define every frame's complete payload grammar. Until the type-specific schemas and canonical vectors exist, the presence of a type code does not mean that the associated semantic behavior is complete. State-mutating handlers must explicitly reject unsupported or malformed payloads.

## 4. Flags

The currently defined mask is FIN (0x01), ACK (0x02), URGENT (0x04), INCREMENTAL (0x08), RESUMABLE (0x10), and OPTIONAL (0x20). All other bits are reserved and are rejected by the C codec. Type-specific flag legality still needs a complete table and tests.

## 5. Friends' Packs

Friends' Packs are ordinary user-controlled application data. Friend entries and document references are not authorization credentials. FP is an application quota only; it is not a trust score, identity proof, or permission to access a system. Bonus-offer references must be independently authorized by application policy.

## 6. Session, flow control, and negotiation

STREAM-ID, REQUEST-ID, and SEQUENCE are correlation/order fields, not authentication. The current server does not integrate a complete per-stream state machine or enforce WINDOW credit in a persistent multiplexed transport. Negotiation fallback is configurable in code; production deployment must explicitly disable fallback unless a reviewed compatibility policy permits it. Offers and selection are not yet cryptographically bound to an authenticated handshake transcript.

## 7. DOWNLOAD and resumability

For transfers over the project threshold (currently greater than 50 MiB), the metadata contract is:

`SESSION-ID | DATETIME | FILE-ID | FILE-NAME | INDEX | OFFSET | TOTAL-SIZE`

These fields are metadata, not authorization. The parser must reject malformed numbers, overflow, extra fields, path-like filenames, and offsets above total size. A production resume implementation additionally requires authenticated tokens, durable atomic checkpoints, chunk integrity checks, final full-object digest verification, and atomic publication. Those guarantees are not supplied by the metadata parser.

## 8. Security boundary

The current server carries HTTP 5.0 frames inside HTTP/1.x over TCP. It does not implement TLS, QUIC, HTTP/3, frame MACs, certificate verification, or a complete authorization engine. Untrusted-network deployments require an independently maintained authenticated transport and explicit application authorization. Do not describe the server as Internet-safe merely because it can accept TCP connections.

## 9. Conformance and release criteria

Required tests include golden-byte vectors; all frame types; reserved flags; unsupported version/type; truncation and trailing bytes; maximum-length boundaries; strict Content-Length; duplicate Content-Length and Transfer-Encoding rejection; malformed DOWNLOAD metadata; response-header injection; and sanitizer/fuzz runs. Production status additionally requires complete payload schemas, integrated stream lifecycle and flow control, authenticated transport, durable resume, cross-platform CI, and independent security review.
