# Alphabet Conformance Test Plan

These cases are required for compiler/runtime integration. Native UTF-8 and bounded-set tests are implemented in lib/alphabet/native/tests.cpp; this plan also covers language and catalogue behavior.

- [x] Native UTF-8 scalar decoding and encoding checks.
- [x] Reject overlong UTF-8, surrogate encodings, out-of-range values, and truncation.
- [x] C<n> canonical signature parsing and limits.
- [x] Set insertion, membership, duplicate rejection, and capacity overflow.
- [ ] Compile SLeeLa C<n> declarations and verify static diagnostics.
- [ ] Execute SLeeLa CharacterSet methods through SLVM native bindings.
- [ ] Add a UTF-4088 adapter integration test against the published 16,606-record registry.
- [ ] Add versioned Unicode normalization and grapheme-cluster support if the runtime adopts a Unicode library.
- [ ] Run full Linux, Windows, and macOS CI for compiler/VM integrations.
