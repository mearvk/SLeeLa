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
