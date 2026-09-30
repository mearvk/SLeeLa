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