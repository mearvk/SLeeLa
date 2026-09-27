# SLeeLa HTTP 5.0 — Friends' Packs

Status: Experimental SLeeLa protocol generation; not an IETF HTTP/5 standard.

HTTP 5.0 extends the repository's HTTP 1.0+ lineage and HTTP 4.0 frame/session architecture with an application-layer Friends' Packs (FP) capability. Friends' Packs carry ordinary application information plus references to optional, explicitly declared bonus offerings.

FP is a quota/capability value, not a measure of trust, nationality, geography, or Internet access. When FP reaches 0, only optional bonus-pack allowances are exhausted; ordinary protocol operation continues under the negotiated session and carrier.

## Architecture

Application
  |
SLeeLa HTTP 5.0 semantic API
  |
Friends' Packs / bonus-offering layer
  |
HTTP 5.0 frame + session model
  |
Capability / flow-control / integrity
  |
Authenticated carrier
  +-- HTTP/3 / QUIC
  +-- HTTP/4-compatible application carrier
  +-- test/in-memory carrier

## Friends' Pack model

A Friends' Pack contains an opaque pack identifier, an application relationship identifier, an FP balance, optional bonus-offer references, optional expiry, and negotiated policy. Bonus offerings are references, not forced downloads or redirects.

## Zero-FP behavior

1. Normal HTTP 5.0 traffic continues.
2. New optional Friends' Pack bonuses are declined.
3. Ordinary requests are unaffected.
4. FP exhaustion does not authorize blocking, degrading, rerouting, or interfering with unrelated network traffic.
5. Replenishment occurs only through explicit application policy.

## Defensive Assault Kits

The requested Assault Kits are represented as defensive audit kits only. They are bounded conformance/test bundles for authorized lab validation of parser boundaries, replay handling, rate limits, capability negotiation, and router-facing interoperability.

They do not contain router-stinging, packet-flooding, credential attacks, route manipulation, denial-of-service logic, or instructions for attacking infrastructure in India, Pakistan, China, Korea, or elsewhere.

## HTTP 1.0+ lineage

HTTP 5.0 retains the repository's layered principle: HTTP/1.x semantics remain the historical baseline; HTTP/2-style multiplexing informs concurrent exchanges; HTTP/3/QUIC supplies an authenticated carrier option; and SLeeLa HTTP 4.0 contributes explicit frames, stream/request identity, sequence handling, resumability, flow control, migration, and typed reset behavior.

HTTP 5.0 adds Friends' Packs as an application capability without changing the carrier security boundary.

## Security boundary

TLS/QUIC and authenticated carriers remain responsible for transport security. FP is not an authentication or authorization credential. Offer references do not grant access by themselves.

## Initial extensions

- FRIENDS_PACK — pack metadata and optional offer references.
- BONUS_OFFER — one optional offering reference.
- FP_UPDATE — application-level FP balance change.
- AUDIT — defensive conformance/audit event.

## Build

    make -C http-5.0
    make -C http-5.0 test
