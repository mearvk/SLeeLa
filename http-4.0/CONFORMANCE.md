# HTTP 4.0 Conformance and Adversarial Test Plan

This is the required conformance plan for the experimental protocol. A test is not considered passed until it has actually run in CI and its output is retained.

## Required deterministic tests

1. Encode/decode a valid frame and compare all 32 header bytes against TEST-VECTORS.md.
2. Reject unknown version, type 0, type 10, undefined flag bits, and payload length greater than configured maximum.
3. Reject headers shorter than 32 bytes and payloads truncated by one byte.
4. Accept a complete frame followed by another frame while reporting the exact first-frame byte count.
5. Verify big-endian boundaries for 16-, 32-, and 64-bit fields, including zero and maximum values.
6. Verify sequence handling independently for two streams and both directions, including duplicates, gaps, and counter exhaustion.
7. Verify OPEN → DATA → END; reject DATA after END; verify RESET terminality and duplicate OPEN rejection.
8. Verify WINDOW credit cannot overflow and DATA cannot exceed granted credit.
9. Reassemble in order and out of order; reject duplicates, overlaps, wrong IDs/count/total, offset overflow, invalid FIRST/LAST, truncated segment headers, and incomplete transfers.
10. Exercise transfer size 0, 1, 50 MiB exactly, 50 MiB + 1, configured maximum, and maximum + 1.
11. Resume from a valid checkpoint; reject wrong principal, file version, digest, total size, offset, expired token, stale checkpoint, and duplicate mutating resume.
12. Verify negotiation fails closed when required capability/security bits are missing and legacy fallback is explicit and observable.

## Native quality gates

- Build with strict warnings on Linux, macOS, and Windows.
- Run AddressSanitizer and UndefinedBehaviorSanitizer where supported.
- Run a coverage-guided fuzzer against frame decode, segment decode, context parse, DOWNLOAD parse, and HTTP request parsing.
- Use allocation-failure tests and assert cleanup for partial reassembly initialization.
- Run deterministic property tests: encode → decode preserves fields; accepted segments cover each byte exactly once; rejected input does not mutate state.
- Compare C and SLeeLa models using shared JSON or binary fixtures.
- Keep regression cases for every discovered defect.

## Status

The test plan itself is documented. It is not evidence that every test has passed. Consult STATUS.md for verified execution state. Do not claim production readiness until test logs, supported-platform results, and a security review are attached to a release.
