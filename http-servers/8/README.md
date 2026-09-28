# SLeeLa HTTP 8.0 Server

Native HTTP 8.0 handshake/session server using the common SLeeLa HTTP 4–9 packet envelope.

HTTP 8.0 remains an experimental SLeeLa application protocol, not an IETF HTTP/8 standard.

Wire contract:
- 32-byte network-byte-order packet header.
- VERSION must be 8.
- First packet must be OPEN with sequence 0.
- Sequence numbers may not regress.
- Payload is generation-specific HTTP/8.0 handshake/session data.
- A HANDSHAKE marker is required before exchange.
- Packet identity is logged without opaque payload contents.

Build: `make`
Run: `./http-server-8 --addr 127.0.0.1 --port 8408 --once`
