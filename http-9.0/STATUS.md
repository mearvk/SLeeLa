# SLeeLa HTTP 9.0 — Status

HTTP 9.0 is an experimental SLeeLa application-protocol generation, not an IETF HTTP/9 standard.

## Repository components

- C API and C++ wrapper for HTTP 9.0 metadata;
- explicit specification and deployment configuration;
- C and C++ negotiation adapters;
- shared HTTP negotiation contract;
- Makefile with C11/C++17 syntax checks;
- build and conformance guidance.

## Required before deployment

- Complete runtime tests for all metadata setters and validation branches.
- Validate all fixed-size strings and reject truncated identifiers where truncation could change meaning.
- Confirm route/resolver/router policy integration on each supported platform.
- Add fuzzing for malformed and oversized metadata.
- Verify explicit fallback and ensure required security properties cannot be downgraded.
- Review authentication, authorization, integrity, privacy, replay handling, logging, and secret management in the consuming transport.

## Build

From this directory:

```sh
make syntax
make test
```

The syntax target checks the local C/C++ translation units. It does not claim full link, runtime, wire-level, or interoperability conformance.
