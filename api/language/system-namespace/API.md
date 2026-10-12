# SLeeLa System Namespace API

## Purpose

This page records the verified contract surface for the `ran::` system-reference namespace exercised by the syntax 1.9 fixtures. It is intentionally conservative: only names demonstrated by repository tests are listed as verified here.

## Verified behavior

- `ran::mem` is accepted by the passing namespace fixture at `tests/sleela-syntax-1.9/ran-namespace-pass.sleela`.
- An unknown reference such as `ran::cpu` is exercised by `tests/sleela-syntax-1.9/ran-namespace-unknown.sleela` as a negative case and should be rejected by the compiler/runtime contract.
- The exact type, units, sampling cadence, and platform availability of `ran::mem` must be taken from the implementation and test assertions; this document does not invent those details.

## Consumer guidance

1. Use only system references documented by the implementation or a passing fixture.
2. Treat unknown references as errors; do not assume a name is supported because it sounds like a plausible system metric.
3. Do not confuse the `ran::` reference namespace with the C/C++ memory manager or the garbage collector's internal counters.
4. When adding a reference, update the registry/implementation, add positive and negative tests, and update [REFERENCES.md](REFERENCES.md).

## Evidence

- Positive fixture: [`ran-namespace-pass.sleela`](../../../tests/sleela-syntax-1.9/ran-namespace-pass.sleela)
- Negative fixture: [`ran-namespace-unknown.sleela`](../../../tests/sleela-syntax-1.9/ran-namespace-unknown.sleela)
- Versioned syntax overview: [`SLEELA.syntax`](../../../SLEELA.syntax)

## Status

The namespace has fixture-level evidence. This page does not claim that a complete cross-platform system-information API is implemented.
