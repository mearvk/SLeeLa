# Design Activity Verification

The Design Activity C module requires boundary and numerical tests before a release is treated as build-verified.

Required tests:

1. Normalize six valid 0–100 measurements.
2. Reject null input/output.
3. Reject values below 0, above 100, NaN, and infinity.
4. Validate every supported science domain.
5. Reject unknown domains.
6. Compare normalized and reference vectors.
7. Verify zero variance when vectors are identical.
8. Verify finite variance, RMS deviation, and L2 distance.
9. Reject non-finite comparison vectors.
10. Verify JSON output rejects unsafe executable names and reports buffer exhaustion.
11. Run the same tests under Linux, macOS, and Windows CI.
12. Run the tests with warnings enabled and treat warnings as errors where the platform toolchain supports it.

The production module performs no dynamic allocation, so the tests should remain deterministic and bounded.

A design-activity record is a software conformance record. It must not be presented as a physical measurement uncertainty statement.
