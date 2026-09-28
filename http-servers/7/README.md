# SLeeLa HTTP 7.0 Server

Native HTTP 7.0 semantic/assertion server using the common SLeeLa HTTP 4–9 packet envelope.

HTTP 7.0 remains an experimental SLeeLa application protocol, not an IETF HTTP/7 standard.

Wire contract:
- 32-byte network-byte-order packet header.
- VERSION must be 7.
- First packet must be OPEN with sequence 0.
- Sequence numbers may not regress.
- Payload is generation-specific HTTP/7.0 semantic/assertion data.
- Packet identity is logged without opaque payload contents.

The HTTP/7.0 payload must identify HTTP/7.0. The server then returns semantic/assertion metadata as packet DATA.

Build: `make`
Run: `./http-server-7 --addr 127.0.0.1 --port 8407 --once`
