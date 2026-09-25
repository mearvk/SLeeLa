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
