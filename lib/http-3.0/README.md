# SLeeLa HTTP 3.0 library sources

This directory exposes SLeeLa-level models for the HTTP 3.0 (H3) application
protocol, alongside the native implementation in `../../http-3.0/`.

## Source inventory

- `SLPackage.sleela` — package facade and transport boundary.
- `Http30Envelope.sleela`, `Http30Request.sleela`, `Http30Response.sleela` — envelope and request/response models.
- `Http30Route.sleela`, `Http30Validation.sleela`, `Http30Negotiation.sleela` — routing, validation, generation negotiation.
- `Http30Authorization.sleela`, `Http30SecurityPolicy.sleela`, `Http30Integrity.sleela` — authorization and integrity-gate models.
- `Http30Handshake.sleela` — L1–L4 project capability negotiation model.
- `Http30RetryPolicy.sleela` — READ / IDEMPOTENT / MUTATING / STREAM retry policy.
- `Http30TimingPolicy.sleela` — advisory timing and carrier telemetry.
- `Http30BasketPolicy.sleela` — fixed 14-item basket schema metadata.
- `Http30Limits.sleela` — documented envelope, dictionary, MAC and basket limits.
- `Http30ConnectionPolicy.sleela` — connection lifecycle policy.
- `Http30DownloadState.sleela`, `Http30DownloadPolicy.sleela` — resumable-download metadata and policy.

## Important implementation boundaries

These files are SLeeLa-level models and policies. They do not replace the
native parser, cryptographic code, replay-state storage, authorization provider,
or transport implementation. The native H3 core documents a SipHash-2-4 keyed
64-bit digest, nonce replay guard, INTACTX check and canonical basket. A 64-bit
tag is not a substitute for modern transport security or a full peer identity
system; deploy behind authenticated TLS/QUIC or another protected transport,
and validate the cryptographic design independently before public exposure.

The native reference server architecture currently describes an HTTP/1.x
carrier. The SLeeLa generation name “HTTP 3.0” must not be confused with the
IETF HTTP/3 protocol: this library does not claim QUIC, HTTP/3 framing, QPACK,
or standards interoperability. Capability levels are project-specific.

The native specification/data-flow and tests remain authoritative:
`../../http-3.0/README.md`, `../../http-3.0/FLOW.md`,
`../../http-3.0/STATUS.md`, and `../../http-3.0/Makefile`.
