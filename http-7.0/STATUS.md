# SLeeLa HTTP 7.0 — Status

HTTP 7.0 is an experimental SLeeLa application generation, not an IETF HTTP/7 standard.

## Implemented/documented

- Reality Assertion application model;
- explicit assertion status/source fields in the specification;
- C/C++ negotiation implementation;
- C/C++ syntax-check build.

## Assertion boundary

A packet carrying a statement does not make the statement true. Assertions must retain their declared status, authorship, and source metadata where applicable.

## Verification targets

Add conformance tests for assertion classification, missing-source handling, bounded statement lengths, negotiation, explicit fallback, and serialization round trips.
