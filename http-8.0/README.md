# SLeeLa HTTP 8.0

**Status:** Experimental SLeeLa cryptographic/session generation; not an IETF HTTP/8 standard.

HTTP 8.0 adds an explicit cryptographic and session-security boundary to the SLeeLa HTTP lineage. Cryptographic configuration remains separate from ordinary application payloads, and implementations should fail closed when required security policy cannot be satisfied.

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
3. Validate negotiation behavior.
4. Reject configurations that violate required security policy.
