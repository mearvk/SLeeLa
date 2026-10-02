<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa Resolver

The shared resolver provides runtime DNS/IP and filesystem-path resolution for
Server Editions and HTTP 1.0–9.0.

Capabilities:
- hostname → numeric IPv4/IPv6 address using the host resolver;
- IP → hostname reverse lookup when PTR resolution is available;
- canonical filesystem-path resolution;
- event-scoped dynamic resolution policy;
- C and C++ interfaces;
- standalone self-test/diagnostic executable.

The resolver is deliberately opt-in at the HTTP packet-policy boundary. If no
resolver target is configured, existing protocol behavior is unchanged. Set
SLEELA_RESOLVER_TARGET to a hostname, IP address, or path when an event requires
dynamic resolution. Set SLEELA_RESOLVER_REQUIRE_DYNAMIC=true when an event must
produce a live DNS/IP resolution rather than a static path result.

The resolver does not rewrite operating-system routes. It supplies a current
resolution result to the HTTP/server decision layer, which can then combine it
with the preferred-router subsystem.