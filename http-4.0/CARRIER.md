# HTTP 4.0 Carrier Model

SLeeLa HTTP 4.0 is a protocol layer, not a replacement for an operating-system transport.

## Initial carrier: HTTP/3

The first interoperable carrier is HTTP/3. An HTTP/3 request/response body carries a sequence of HTTP 4.0 frames as opaque application bytes.

The carrier is responsible for:
- network connection establishment;
- TLS/QUIC security;
- HTTP/3 stream transport;
- congestion control;
- packet loss recovery.

HTTP 4.0 is responsible for:
- logical stream identity;
- request identity;
- frame sequence;
- resumability;
- application flow control;
- capability state.

This separation prevents HTTP 4.0 from pretending that application-level frames replace QUIC transport semantics.

## Future carriers

A future carrier may provide a native SLeeLa socket or another transport. Such a carrier MUST preserve the HTTP 4.0 frame semantics or explicitly negotiate a different profile.

## Carrier invariants

A carrier MUST NOT rewrite an HTTP 4.0 payload after validation. In particular, it MUST preserve:
- stream ID;
- request ID;
- sequence;
- frame type;
- payload bytes.

A carrier MAY split or coalesce transport reads, because the HTTP 4.0 decoder is length-bounded.
