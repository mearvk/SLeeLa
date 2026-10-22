# SLeeLa HTTP 2.0 / 2.1 library contract

This folder contains SLeeLa-language counterparts to the native implementation
in ../../http-2.0/. The models were integrated from the established
`lib/http-1.0/` source family and adapted to the HTTP 2.1 generation.

## Source inventory

- `SLPackage.sleela` — package facade and transport-boundary declaration.
- `Http21Request.sleela`, `Http21Response.sleela` — request/response state.
- `Http21Negotiation.sleela` — generation negotiation.
- `Http21Route.sleela` — logical port/service/operation routing model.
- `Http21Validation.sleela`, `Http21Limits.sleela` — validation and resource limits.
- `Http21Authorization.sleela`, `Http21SecurityPolicy.sleela` — authorization and deployment policy models.
- `Http21ConnectionPolicy.sleela` — connection lifecycle policy.
- `Http21DownloadState.sleela`, `Http21DownloadPolicy.sleela` — resumable-transfer metadata and policy.

These are SLeeLa-level models, not replacements for native byte parsing,
socket lifecycle enforcement, authentication, authorization, or filesystem
access controls. The native server currently uses HTTP/1.x transport; it does
not implement IETF HTTP/2 framing, HPACK, HTTP/2 streams, or TLS. See
`../../http-2.0/HTTP21.SECURITY.md` for deployment constraints.
