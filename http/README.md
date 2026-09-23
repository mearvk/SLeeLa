# SLeeLa HTTP

This directory contains the HTTP transport and web-integration layer for SLeeLa.

The HTTP subsystem is intended to provide a conventional Internet-facing transport while keeping SLeeLa business logic behind a defined connector boundary.

## Structure

- [`spec/`](spec/) — HTTP integration specifications, including the HTTP 3.0
  protocol spec ([`spec/HTTP-3.0-SPEC.md`](spec/HTTP-3.0-SPEC.md)) and guiding
  principles ([`spec/HTTP-3.0-GUIDING-PRINCIPLES.md`](spec/HTTP-3.0-GUIDING-PRINCIPLES.md)).

The HTTP 3.0 implementation source (crypto capsules, key distribution, route
navigation, and the build/self-test tooling) lives in the top-level
[`http-3.0/`](../http-3.0/) directory.

The implementation should remain compatible with ordinary HTTP infrastructure, including routers, proxies, TLS termination, fiber, coax, Ethernet, Wi-Fi, and other underlying physical/network media.


## HTTP Version Port-Multiplexing Model

SLeeLa documents one common application-level port model across its HTTP generations:

- **HTTP 1.0:** logical requests are multiplexed by a SLeeLa request/connection dispatcher; the logical port is an application identifier and does not imply a separate OS socket.
- **HTTP 2.0 / 2.1:** HTTP/2 streams provide the transport-level multiplexing. Each stream may carry a SLeeLa logical port, service, operation, request, and payload.
- **HTTP 3.0:** HTTP/3 streams provide the transport-level multiplexing, while the SLeeLa envelope carries the expanded 160-bit logical port namespace.

The layers remain distinct:

`native transport endpoint → HTTP stream → SLeeLa logical port → service/operation`

A logical port identifies a service, channel, endpoint, virtual listener, or application route. It is not a promise that the host has opened a corresponding TCP/UDP socket for every logical value.
