<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-logo-004.jpg" alt="SLeeLa">






# SLeeLa HTTP 4.0

**Status:** Experimental SLeeLa next-generation protocol; not an IETF HTTP/4 standard.

There is no published IETF HTTP/4 specification corresponding to this SLeeLa generation. HTTP 4.0 is therefore documented as a SLeeLa experimental protocol layer that can be carried by HTTP/3 or another supported transport.

## Purpose

HTTP 4.0 moves the SLeeLa model toward an explicit message and frame lifecycle. It adds protocol-level treatment of:

- stream and request identity;
- authenticated sequence handling;
- resumable delivery;
- flow control and backpressure;
- incremental delivery;
- capability negotiation;
- session migration;
- typed reset and retry behavior;
- observability.

## Architecture

```text
Application
    |
SLeeLa HTTP 4.0 semantic API
    |
HTTP 4.0 message/frame model
    |
Capability + session control
    |
Integrity / replay / resumability
    |
Carrier adapter
    +-- HTTP/3 / QUIC
    +-- Future native carrier
    +-- Test / in-memory carrier
```

## Frame Model

Initial frame representation:

```text
VERSION | TYPE | FLAGS | STREAM-ID | REQUEST-ID | SEQUENCE | PAYLOAD-LENGTH | PAYLOAD
```

Defined frame types:

- `OPEN` — establish a logical request or stream.
- `DATA` — carry application payload.
- `END` — complete a request or stream.
- `RESET` — terminate a stream.
- `WINDOW` — advertise receive capacity.
- `PING` / `PONG` — liveness.
- `RESUME` — continue an interrupted transfer.
- `CAPSULE` — carry negotiated or session metadata.

## HTTP 3.0 → HTTP 4.0

| Concern | HTTP 3.0 | HTTP 4.0 |
|---|---|---|
| Application envelope | Central | Retained as semantic payload |
| Stream identity | Carrier-oriented | Explicit protocol field |
| Sequence handling | Request/integrity layer | Authenticated frame sequence |
| Resume | Application convention | Explicit frame |
| Flow control | Advisory/application layer | Explicit WINDOW frames |
| Incremental delivery | Application/carrier behavior | Explicit DATA lifecycle |
| Capability negotiation | Handshake | Session capability model |
| Carrier | Primarily HTTP/3 | Pluggable carrier |
| Migration | Carrier-dependent | Protocol session model |
| Errors | Response/pipeline | Typed stream/session reset |

## Security Boundary

HTTP 4.0 does not replace TLS or invent a substitute for authenticated transport. Deployments should use an authenticated carrier and may add SLeeLa application integrity.

The reference implementation provides deterministic frame validation and sequence checks. Cryptographic key management remains a separate concern.

## Compatibility

HTTP 4.0 can initially be transported as an application payload over HTTP/3. This provides an implementation path without representing SLeeLa HTTP 4.0 as an Internet-standard HTTP version.

## Build

```sh
make -C http-4.0
make -C http-4.0 test
```

The implementation is dependency-light and uses C11.

## Planned Implementation Areas

1. HTTP/3 carrier adapter.
2. Resumable upload/download state.
3. Incremental forwarding.
4. Authenticated session keys.
5. Path-migration policy.
6. C++ / Python API parity.
7. Linux, Windows 10+, and macOS builds.
8. Interoperability fixtures and conformance tests.

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