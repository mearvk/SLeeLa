# HTTP 4.0 Security Boundary

HTTP 4.0 does not replace TLS, QUIC authentication, certificates, or operating-system security controls.

## Required boundaries

1. Carrier security authenticates the network connection.
2. HTTP 4.0 capabilities are explicitly negotiated.
3. Frame lengths are bounded before payload access.
4. Sequence numbers are checked by the session layer.
5. Resumption requires authenticated transfer state.
6. Application payloads are opaque to the frame parser.

## Cryptographic extension

The next security layer should authenticate the canonical frame header plus payload using a per-session key. The authenticated material should include:

VERSION | TYPE | FLAGS | STREAM-ID | REQUEST-ID | SEQUENCE | PAYLOAD-LENGTH | PAYLOAD

A future implementation can reuse the cryptographic primitives already present in SLeeLa HTTP 3.0 without coupling the frame parser to OpenSSL.

## Replay and reordering

Sequence numbers provide an application ordering boundary. A receiver should track the highest accepted sequence per logical stream and reject stale or duplicated frames according to the negotiated profile.

Sequence numbers alone are not cryptographic authentication. An attacker that can modify frames must be unable to forge a valid authenticated sequence record once the cryptographic extension is enabled.

## Privacy

HTTP 4.0 should avoid putting sensitive application information into transport-visible routing fields. Stream ID and request ID are correlation mechanisms, not authorization credentials.

## Transport security and TLS boundary

SLeeLa HTTP 4.0 does not implement TLS inside the HTTP/4 frame encoder. Security is intentionally layered.

When HTTP/4 is carried over HTTP/3/QUIC, TLS 1.3 is handled by QUIC as the transport security layer. QUIC uses TLS for authenticated key exchange and derives packet-protection keys from the TLS handshake. HTTP/3 likewise delegates confidentiality, integrity, peer authentication, reliable delivery, and stream-level transport behavior to QUIC.

Therefore the HTTP/4 application layer should **not implement a second TLS stack** merely to protect ordinary HTTP/3/QUIC carriage. Instead, the HTTP/4 implementation should consume an authenticated carrier interface and verify that the carrier/session meets the security requirements before accepting application data.

For other carriers, the security contract is explicit:

- **HTTP/3/QUIC:** use the carrier's TLS 1.3+ security; do not duplicate transport encryption in the HTTP/4 frame layer.
- **TCP or another byte-stream carrier:** use an appropriate authenticated TLS deployment before exposing HTTP/4 application data to an untrusted network.
- **In-process/test carrier:** TLS may be omitted only when the surrounding execution boundary is trusted and the test explicitly declares that condition.
- **Native future HTTP/4 carrier:** define its cryptographic handshake and authenticated-carrier contract before treating it as production-capable.

### HTTP/4 still needs application-layer security

Transport encryption is not a substitute for protocol validation. HTTP/4 continues to validate version, frame type, payload length, stream/request identity, sequence values, segmentation metadata, and resumability state. Future authenticated application profiles may additionally bind the logical message, segment set, and context metadata to an application signature or MAC.

### 0-RTT and replay

If the underlying QUIC carrier permits 0-RTT, HTTP/4 operations that create externally visible or otherwise non-idempotent effects must not assume that early data is non-replayable. QUIC/TLS documentation explicitly identifies replay exposure for 0-RTT application data. Applications should either restrict 0-RTT to replay-safe operations or require a confirmed handshake before processing state-changing operations.

### Segmentation security

MTU segmentation is an application-layer framing mechanism, not IP fragmentation. Every segment must be authenticated by the carrier and validated before reassembly. A receiver must reject impossible indexes, counts, offsets, lengths, inconsistent FIRST/LAST markers, and reassembly sizes that exceed configured limits. Reassembly buffers should be bounded to prevent memory-exhaustion attacks.

### Key management

Certificate/private-key handling belongs to the selected TLS/QUIC implementation and deployment layer. HTTP/4 should receive an authenticated carrier rather than embedding private keys in packet-processing code.
