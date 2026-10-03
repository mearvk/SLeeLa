<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-logo-004.jpg" alt="SLeeLa">






# SLeeLa HTTP 8.0

**Status:** Experimental SLeeLa cryptographic/session generation; not an IETF HTTP/8 standard.

HTTP 8.0 adds an explicit cryptographic and session-security boundary to the SLeeLa HTTP lineage. Cryptographic configuration remains separate from ordinary application payloads, and implementations should fail closed when required security policy cannot be satisfied.

## National Emblems, Signals, and Frequency

HTTP 8.0 also establishes an application-level area of interest for the structured representation and exchange of **National Emblems, Signals, and Frequency**.

This area is intended for descriptive, interoperable metadata such as:

- national emblem names, identifiers, provenance, and display references;
- public or authorized signal identifiers and their semantic descriptions;
- frequency-related metadata, including units, bands, ranges, measurement context, and source references;
- jurisdiction, organization, or service context where explicitly supplied by the application;
- timestamps, versioning, and provenance needed to distinguish current data from historical or user-authored records.

These records are **data models, not authority grants**. An emblem, signal, frequency, or jurisdictional label carried by HTTP 8.0 does not itself establish legal status, authenticity, ownership, authorization, or operational control.

Frequency information should remain descriptive and bounded by the applicable configuration and authorization policy. HTTP 8.0 does not define instructions for unauthorized interception, interference, jamming, evasion, or disruption of communications.

## DarkPower

HTTP 8.0 includes the project-level **DarkPower** C++ model:

- `DarkPower.hpp` / `DarkPower.cpp` — typed DarkPower session descriptor;
- **SCHEDULE TERM** — fixed 24-character value;
- **DARK POWER** — integer value `0x18ae`;
- `contact_request_for_iss()` — application-level contact-request string helper for domain `ISS`.

The contact-request method is a data-format helper. It does not authenticate a peer, authorize access, bypass security controls, or establish a network connection.

## Repository Files

- `CRYPTO.SUPPORT.md` — cryptographic support boundary.
- `HTTP80.EARLY.SECURITY.conf` — early security configuration.
- `http80.c` / `http80.cpp` — protocol implementation.
- `http80_crypto.hpp` — C++ cryptographic interface.
- `DarkPower.hpp` / `DarkPower.cpp` — DarkPower session descriptor.
- `HTTP.NEGOTIATION.md` — generation negotiation.

## Negotiation

HTTP 8.0 is selected only after explicit peer acceptance. Where fallback is permitted, negotiation may return to HTTP/1.1 and then HTTP/1.0.

Fallback must not silently weaken a security property required by the configured policy.

## Logical Ports and Download

Logical ports remain application identifiers rather than native sockets.

Files larger than 50 MB use the shared resume contract:

```text
SESSION-ID | DATETIME | FILE-ID | FILE-NAME | INDEX | OFFSET | TOTAL-SIZE
```

## Verification

Before deployment, the build and verification process should:

1. Syntax-check the C and C++ sources.
2. Validate cryptographic configuration.
3. Validate National Emblems, Signals, and Frequency metadata for schema, provenance, units, and bounded values.
4. Validate negotiation behavior.
5. Reject configurations that violate required security policy.

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
