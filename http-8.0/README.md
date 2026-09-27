# SLeeLa HTTP 8.0

Status: Experimental SLeeLa cryptographic/session generation; not an IETF HTTP/8 standard.

HTTP 8.0 extends the SLeeLa HTTP lineage with an explicit cryptographic/session boundary. The implementation must keep cryptographic configuration separate from ordinary application payloads and must fail closed when required security policy cannot be satisfied.

## Files

- `CRYPTO.SUPPORT.md` — supported cryptographic boundary;
- `HTTP80.EARLY.SECURITY.conf` — early security configuration;
- `http80.c/.cpp` — protocol implementation;
- `http80_crypto.hpp` — C++ cryptographic interface;
- `HTTP.NEGOTIATION.md` — peer-generation negotiation.

## Negotiation

HTTP 8.0 is selected only after explicit peer acceptance. Where fallback is permitted, the negotiation layer may fall back to HTTP/1.1 and then HTTP/1.0. Required security properties must not be silently weakened by fallback.

## Logical ports and download

Logical ports remain application identifiers rather than native sockets. Files larger than 50 MB use the common resume contract:

`SESSION-ID | DATETIME | FILE-ID | FILE-NAME | INDEX | OFFSET | TOTAL-SIZE`

## Verification

The build must syntax-check both C and C++ sources and validate the configured cryptographic policy before deployment.
