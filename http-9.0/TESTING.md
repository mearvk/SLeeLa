# HTTP 9.0 Build and Conformance Checks

## Local build

Run from `http-9.0/`:

```sh
make syntax
make test
```

The syntax target requires a C11 compiler and a C++17 compiler. The negotiation units include shared SLeeLa router/resolver headers; build them in the repository tree so relative includes resolve.

## Minimum runtime test matrix

A release-quality test harness should exercise:

1. Null arguments for every public C entry point.
2. Default initialization and stable placeholder values.
3. Identity and frequency setter boundaries, including empty and overlong strings.
4. Frequency enabled with value `DISABLED` (must fail closed).
5. International data and Dark Band required fields.
6. Sequence and prior-metadata preservation by the caller's serializer.
7. Negotiation for exact version matches, unsupported versions, whitespace/comma-delimited accept lists, and fallback disabled.
8. Required-security policy when peers offer only a lower generation.
9. C and C++ API parity.
10. Fuzzing of metadata and configuration parsing in the integrating application.

## Interpretation

A syntax pass does not establish link success, correct wire serialization, network interoperability, or security. Those require separate integration tests and an explicitly documented packet serialization format.
