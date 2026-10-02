# Resolver Configuration

Environment variables:
- SLEELA_RESOLVER_TARGET — hostname, IP address, or filesystem path for an event.
- SLEELA_RESOLVER_REQUIRE_DYNAMIC=true — require a live DNS/IP resolution.
- SLEELA_RESOLVER_CONFIG — reserved configuration-file location for future
  resolver policy expansion.

The Server Editions export the resolver target through their process environment
when operators configure it. HTTP 1.0–9.0 read the same environment through the
shared resolver policy call.
