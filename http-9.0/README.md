# SLeeLa HTTP 9.0

Status: Experimental SLeeLa application/protocol generation; not an IETF HTTP/9 standard.

HTTP 9.0 defines the current SLeeLa identity/metadata generation represented by `http90.c/.cpp/.h/.hpp`, its specification, configuration, and negotiation layer.

## Core documents

- `HTTP90.SPEC.md`
- `HTTP90.conf`
- `HTTP.NEGOTIATION.md`
- `http90.h/.hpp`
- `http90.c/.cpp`

## Compatibility

A peer must explicitly accept HTTP 9.0 before the generation is selected. If fallback is allowed, negotiation can return to HTTP/1.1 and then HTTP/1.0. Security requirements must not be weakened merely to obtain connectivity.

## Logical ports and download

Logical ports are application identifiers, not native sockets. Files larger than 50 MB use the common resume metadata:

`SESSION-ID | DATETIME | FILE-ID | FILE-NAME | INDEX | OFFSET | TOTAL-SIZE`

## Verification

The build checks the C/C++ implementation and the negotiation layer. Protocol conformance should additionally validate configuration parsing, bounded metadata, malformed inputs, explicit fallback, and clean shutdown.
