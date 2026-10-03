<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-logo-004.jpg" alt="SLeeLa">






# SLeeLa HTTP 3.0

**Status:** Experimental SLeeLa application-protocol generation; not a claim of IETF HTTP/3 semantics beyond the selected carrier.

HTTP 3.0 extends the SLeeLa application model with request correlation, service/operation naming, retry classes, processing stages, and an application integrity/security layer. HTTP/3 and QUIC may provide the transport carrier.

## Architecture

```text
QUIC connection
      |
HTTP/3 stream
      |
SLeeLa logical PORT
      |
SERVICE-ID / OP-ID
      |
REQUEST-ID
      |
SLeeLa application envelope
```

Logical ports remain application identifiers rather than native sockets.

## Large-File Download Mode

Files larger than 50 MB use the shared SLeeLa resume contract:

```text
SESSION-ID | DATETIME | FILE-ID | FILE-NAME | INDEX | OFFSET | TOTAL-SIZE
```

## Security Boundary

Carrier security and SLeeLa application integrity are separate layers.

- HTTP/3 / QUIC can provide authenticated transport and encryption according to the deployment.
- The SLeeLa integrity layer validates application-level data and protocol conditions.
- Application integrity does not replace TLS, QUIC security, or deployment authorization.

## Verification

The directory contains implementation and self-test material for the SLeeLa application layer.

See:

- `STATUS.md`
- `FLOW.md`
- the root `Makefile`

The build system should delegate to the protocol source tree rather than replacing the repository's implementation with an unrelated framework.

## Unified Route Data

This implementation consumes the SLeeLa unified route-data contract in
route/ROUTE.DATA.json and route/ROUTE.DATA.md. Route records carry protocol,
server surface, HTTP generation, VM generation/formal VM name, canonical
configuration root, route identifier, target/resolution mode, capability,
transport, port, and status. VM names are architectural metadata only and do
not grant capabilities. Dynamic targets must use the shared resolver before
acceptance. The VM identity is: /impl Core, /1 Foundation, /2 Operator,
/3 Specialist, /4 Supervisor, /5 Manager, /6 Director, /7 Administrator,
/8 Executive, /9 Authority, /10 Principal, /11 Sovereign.
