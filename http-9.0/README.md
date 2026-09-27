# SLeeLa HTTP 9.0

**Status:** Experimental SLeeLa application/protocol generation; not an IETF HTTP/9 standard.

HTTP 9.0 is the current SLeeLa identity and metadata generation represented by the `http90` implementation, specification, configuration, and negotiation files in this directory.

## Core Files

- `HTTP90.SPEC.md` — protocol specification.
- `HTTP90.conf` — configuration.
- `HTTP.NEGOTIATION.md` — generation negotiation.
- `http90.h` / `http90.hpp` — public C/C++ interfaces.
- `http90.c` / `http90.cpp` — implementation.

## Negotiation

A peer must explicitly accept HTTP 9.0 before the generation is selected.

Where fallback is permitted, negotiation may return to HTTP/1.1 and then HTTP/1.0. Fallback must not weaken a security requirement merely to obtain connectivity.

## Logical Ports

Logical ports are SLeeLa application identifiers. They are independent of native TCP/UDP socket numbering.

## Large-File Download Mode

Files larger than 50 MB use the common SLeeLa resume metadata:

```text
SESSION-ID | DATETIME | FILE-ID | FILE-NAME | INDEX | OFFSET | TOTAL-SIZE
```

## Verification

Conformance and deployment checks should cover:

- C and C++ compilation;
- configuration parsing;
- bounded metadata;
- malformed-input handling;
- explicit generation fallback;
- clean shutdown;
- negotiation behavior;
- required security-policy enforcement.

## Relationship to Earlier Generations

HTTP 9.0 is part of the repository's SLeeLa HTTP lineage. It should preserve the architectural distinction established by earlier generations: transport mechanisms carry the exchange, while SLeeLa defines its application-level identity, metadata, and protocol behavior.
