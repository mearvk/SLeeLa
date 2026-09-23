# SLeeLa HTTP/3 Transport

Grade 3 uses HTTP/3 semantics over QUIC rather than treating HTTP/3 as
HTTP/2 over another socket.

## Wire architecture

- HTTP/3: RFC 9114.
- QUIC v1: RFC 9000.
- TLS 1.3 is part of QUIC security.
- ALPN: `h3`.
- Request/response multiplexing uses QUIC bidirectional streams.
- Field compression uses QPACK (RFC 9204), not HPACK.
- HTTP/3 uses UDP and does not use HTTP/1.1 transfer codings.

The SLeeLa adapter in this directory intentionally delegates QUIC packet
processing, TLS handshake, congestion control, loss recovery, stream flow
control, and QPACK to a maintained ngtcp2/nghttp3 backend. The default
backend name is `wsslserver`, the HTTP/3 server shipped by ngtcp2.

This is a deliberate boundary: SLeeLa owns the command contract and service
integration; a mature QUIC implementation owns the wire protocol.

## Build

Build the adapter with:

    make

A QUIC-capable backend must also be installed. With ngtcp2, the example
server is built as `wsslserver` and accepts:

    wsslserver [OPTIONS] <ADDR> <PORT> <PRIVATE_KEY_FILE> <CERTIFICATE_FILE>

SLeeLa invokes it as:

    http-server-3 --addr 0.0.0.0 --port 8082 \
      --key server.key --cert server.crt

or:

    http-server-3 --backend /path/to/wsslserver \
      --addr 0.0.0.0 --port 8082 \
      --key server.key --cert server.crt

The Grade 3 service is therefore UDP, with 8082 as the SLeeLa default.

## Security

Do not place private keys in the repository. Use filesystem permissions,
a protected deployment directory, and certificates appropriate to the
hostname being served. HTTP/3 requires the QUIC TLS handshake and ALPN
negotiation; a plain UDP listener is not an HTTP/3 implementation.
