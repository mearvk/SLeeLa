<!-- Author: Max Rupplin — MEARVK LLC — 2026 -->

# Modest development roadmap

This is a planning note, not a promise that the listed algorithms are available
in the current build. New functionality should arrive as isolated code with
API contracts, independent tests, C/C++ examples, and measurements where
performance is the motivation.

## Near-term candidates

1. **Filter design helpers:** low-pass/high-pass FIR design with explicit sample
   rate, cutoff and window choices; validate frequency response in tests.
2. **Streaming primitives:** caller-owned ring buffers and block APIs with clear
   ownership, discontinuity and reset behavior.
3. **Resampling:** rational-rate resampling with documented anti-alias filtering
   and group-delay handling.
4. **Correlation and cross-correlation:** explicit lag sign, normalization and
   output ordering; test against hand-computed sequences.
5. **IIR filters:** stable second-order sections, finite-value checks, and state
   objects with documented initialization and reset behavior.
6. **Windowed spectrum helpers:** documented coherent-gain and power scaling,
   frame length and hop, DC/Nyquist handling, and sample-rate metadata.
7. **More transforms:** wavelets and non-radix-2 FFT strategies, each with known
   reference vectors and numerical error thresholds.

## Quality gates

Before merging a new component, include:
- a written mathematical convention and boundary behavior;
- a C interface with capacity and ownership rules;
- a C++ wrapper only where it improves safe use;
- deterministic tests against small known vectors or a trusted reference;
- CMake/Make integration and examples;
- cross-platform CI plus a benchmark if performance claims are made.

Avoid adding hardware-specific optimization until the portable implementation
is measurable and tests can check the optimized path against it.
