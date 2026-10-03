<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-logo-004.jpg" alt="SLeeLa">






# SLeeLa HTTP 5.0

**Status:** Experimental SLeeLa application-protocol generation; not an IETF HTTP/5 standard.

HTTP 5.0 extends the HTTP 4.0 frame/session model with an application-level **Friends' Packs** capability. A Friends' Pack is ordinary user-controlled application data containing a friends list, optional point values, document references, and optional bonus-offer references.

## Purpose

Friends' Packs provide a structured way to package application relationships and optional offerings. They do not change the underlying HTTP transport semantics.

An FP value is an application quota or accounting value. It is not an authentication credential, a measure of trust, or an authorization to control another system.

## Friends List

A friend entry may contain:

- **friend name**;
- optional **point amount**;
- optional **assigned document reference**.

The initial payload form is:

```text
friend-name|points|document-reference
```

In the implementation, `FriendsPack::addFriend()` adds an entry and `FriendsPack::friendPayload()` serializes the corresponding payload. An invalid friend index returns an empty payload.

## Architecture

```text
Application
    |
SLeeLa HTTP 5.0 semantic API
    |
Friends' Packs
    |
HTTP 5.0 frame + session model
    |
Capability / flow control / integrity
    |
Authenticated carrier
    +-- HTTP/3 / QUIC
    +-- HTTP/4-compatible application carrier
    +-- Test / in-memory carrier
```

## Friends' Pack Model

A pack may contain:

- opaque pack identifier;
- application relationship identifier;
- FP balance;
- optional bonus-offer references;
- optional expiration;
- negotiated application policy.

Bonus offers are references. They do not force downloads, redirects, or access.

## Zero-FP Behavior

When FP reaches zero:

1. Normal HTTP 5.0 operation continues.
2. New optional Friends' Pack bonuses are declined.
3. Ordinary requests remain unaffected.
4. FP exhaustion does not authorize blocking, degrading, rerouting, or interfering with unrelated traffic.
5. Replenishment occurs only through explicit application policy.

## Defensive Audit Kits

The repository's audit-kit concept is limited to authorized conformance and testing. Test bundles may validate parser boundaries, replay handling, rate limits, capability negotiation, and router-facing interoperability.

They are not network-attack mechanisms and do not authorize interference with infrastructure or communications.

## Extensions

- `FRIENDS_PACK` — pack metadata and optional offer references.
- `BONUS_OFFER` — optional offering reference.
- `FP_UPDATE` — application-level balance update.
- `AUDIT` — defensive conformance/audit event.

## Shared Download Contract

Files larger than 50 MB use:

```text
SESSION-ID | DATETIME | FILE-ID | FILE-NAME | INDEX | OFFSET | TOTAL-SIZE
```

## Build

```sh
make -C http-5.0
make -C http-5.0 test
```

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
