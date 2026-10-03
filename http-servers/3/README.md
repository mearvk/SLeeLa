<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-logo-004.jpg" alt="SLeeLa">






# SLeeLa HTTP Server Grade 3

Grade 3 is the SLeeLa HTTP/3 server entry point. It serves HTTP semantics
over QUIC/UDP and uses a QUIC-capable backend for the wire protocol.

Default endpoint: UDP 8082.

The Grade 3 adapter requires a QUIC TLS private key and certificate and
defaults to the ngtcp2 `wsslserver` backend. See `http3/README.md`.

HTTP/3 is defined by RFC 9114 and uses QUIC v1, TLS 1.3, ALPN `h3`, QUIC
streams for request/response multiplexing, and QPACK for field compression.

## Unified Route Data

This server consumes the SLeeLa unified route-data contract in
route/ROUTE.DATA.json and route/ROUTE.DATA.md. Route records carry protocol,
server surface, HTTP generation, VM generation/formal VM name, canonical
configuration root, route identifier, target/resolution mode, capability,
transport, port, and status. VM names are architectural metadata only and do
not grant capabilities. Dynamic targets must use the shared resolver before
acceptance. The VM identity is: /impl Core, /1 Foundation, /2 Operator,
/3 Specialist, /4 Supervisor, /5 Manager, /6 Director, /7 Administrator,
/8 Executive, /9 Authority, /10 Principal, /11 Sovereign.

