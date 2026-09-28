# SLeeLa HTTP 9.0 Server

Native HTTP 9.0 metadata server using the common SLeeLa HTTP 4–9 packet envelope.

HTTP 9.0 remains an experimental SLeeLa application protocol, not an IETF HTTP/9 standard.

Wire contract:
- 32-byte network-byte-order packet header.
- VERSION must be 9.
- First packet must be OPEN with sequence 0.
- Sequence numbers may not regress.
- Payload is generation-specific HTTP/9.0 metadata.
- A HANDSHAKE marker is required before metadata exchange.
- Packet identity is logged without opaque payload contents.

The generation-specific payload corresponds to the HTTP90 metadata model: protocol grade, sequence/prior metadata, configured identity, monitoring configuration, international data and Dark Band metadata. The native server does not activate external monitoring equipment.

Build: `make`
Run: `./http-server-9 --addr 127.0.0.1 --port 8409 --once`
