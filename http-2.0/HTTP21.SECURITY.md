# HTTP 2.1 Security and Deployment Contract

## Protocol boundary

The H21 envelope is a SLeeLa application protocol. The native h21_server
currently accepts HTTP/1.0 or HTTP/1.1 over sleela_net. It does **not**
implement IETF HTTP/2 framing, HPACK, stream state, or TLS. HTTP/2 stream
multiplexing in earlier design notes is aspirational, not an implemented
transport feature.

## Deployment requirements

- Bind to loopback or a trusted private interface by default. Public exposure
  requires a separately configured TLS terminator/reverse proxy and documented
  authentication and authorization.
- Treat X-SLeeLa headers as untrusted routing hints, never identity credentials.
  Validate route IDs against a server-owned registry.
- Trust forwarding headers only from a specifically trusted proxy that overwrites them.
- H21 2.1 has no message authentication, replay protection, or packet integrity
  gate. Use an authenticated protected transport and fail closed if required
  security is unavailable. Never downgrade security requirements for connectivity.
- Do not decompress based only on COMPRESSED; enforce output and expansion-ratio limits.
- Download metadata is a resume hint, not permission to access a path. Resolve
  file IDs through an authorized server-side registry; never build filesystem
  paths from client-controlled names.
- Authorize every application operation. Retry class is advisory; do not
  automatically replay mutating operations.

## Parser and resource policy

- Bound header size/count, individual fields, and body size.
- Reject duplicate Content-Length and Transfer-Encoding to avoid ambiguous framing.
- Envelope lengths must exactly match supplied bytes; trailing data, unknown
  versions/flags, malformed numeric fields, and overlong names are rejected.
- The server handles one connection at a time. It is not a high-concurrency
  internet edge server. Deploy behind a hardened proxy with read deadlines,
  rate limits, and concurrency controls.
- Run make syntax, make test, make test-python, and sanitizer/fuzz jobs in CI.

## Remaining limitations

Parser hardening does not add TLS, authentication, concurrent serving, complete
total-request deadlines, or a filesystem-backed resumable transfer subsystem.
Do not describe this generation as production-ready until those are implemented
and independently tested.
