# SLeeLa HTTP 4.0 — Experimental Next-Generation Protocol

Status: Experimental SLeeLa protocol generation; not an IETF-standard HTTP/4 implementation.

There is no published IETF HTTP/4 specification as of September 2026. The IETF HTTP working group continues to develop HTTP extensions and HTTP semantics, including incremental forwarding and resumable upload work. SLeeLa HTTP 4.0 therefore defines an experimental next-generation SLeeLa application protocol while remaining explicit about which behavior is standard HTTP behavior and which behavior is SLeeLa-specific.

## Purpose

HTTP 4.0 builds on the SLeeLa HTTP 3.0 application model while moving the core toward explicit message/frame lifecycle, independent request and stream identity, resumable and incremental delivery, capability negotiation, flow-control and backpressure signals, connection migration, authenticated sequence numbers, structured error/retry classes, and observability.

The first implementation is a SLeeLa protocol that can be carried by HTTP/3 or another supported transport. It does not claim to replace HTTP/3 on the public Internet.

## Architectural layers

Application
  |
SLeeLa HTTP 4.0 semantic API
  |
HTTP/4 message/frame model
  |
Capability + stream/session control
  |
Integrity / replay / resumability
  |
Carrier adapter
  +-- HTTP/3 / QUIC
  +-- future native carrier
  +-- test/in-memory carrier

## Initial frame model

VERSION | TYPE | FLAGS | STREAM-ID | REQUEST-ID | SEQUENCE | PAYLOAD-LENGTH | PAYLOAD

Frame types:
- OPEN — establish a logical request/stream.
- DATA — application payload.
- END — complete a request/stream.
- RESET — terminate a stream.
- WINDOW — advertise receive capacity.
- PING / PONG — liveness.
- RESUME — continue an interrupted transfer.
- CAPSULE — carry negotiated/session metadata.

## What is new relative to HTTP 3.0

HTTP 3.0 in this repository centers on an application envelope and processing pipeline. HTTP 4.0 moves one layer downward and makes the stream/frame lifecycle itself explicit.

| Concern | HTTP 3.0 | HTTP 4.0 |
|---|---|---|
| Application envelope | central | retained as semantic payload |
| Stream identity | carrier-oriented | explicit protocol field |
| Sequence | primarily request/nonce | authenticated frame sequence |
| Resume | application convention | protocol frame |
| Flow control | advisory | explicit WINDOW frames |
| Incremental delivery | application/carrier behavior | explicit DATA lifecycle |
| Capability negotiation | handshake | session capabilities |
| Carrier | primarily HTTP/3 embedding | pluggable carrier |
| Migration | carrier-dependent | protocol session model |
| Errors | response/pipeline | typed stream/session reset |

## Security boundary

HTTP 4.0 does not invent a replacement for TLS. A deployment should use an authenticated transport and may additionally use SLeeLa application integrity. The reference implementation provides deterministic frame validation and sequence checks; cryptographic keying remains a separate layer.

## Compatibility

HTTP 4.0 can initially be carried as an opaque application body over HTTP/3. This gives existing HTTP/3 infrastructure a migration path without claiming that the carrier has become an HTTP/4 standard.

## Build

    make -C http-4.0
    make -C http-4.0 test

The implementation is dependency-light and uses C11.

## Next implementation layers

1. HTTP/3 carrier adapter.
2. Resumable upload/download state store.
3. Incremental forwarding adapter.
4. Strong authenticated session keys.
5. Path migration policy.
6. C++/Python API parity.
7. Cross-platform Windows 10+, macOS, and Linux builds.
8. Interoperability fixtures and conformance corpus.
