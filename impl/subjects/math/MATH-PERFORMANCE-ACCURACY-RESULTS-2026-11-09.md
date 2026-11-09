# SLeeLa Native Math — Accuracy and Speed Results

**Date:** 2026-11-09  
**Subject:** `sleelactl run math` / `__NativeMath`  
**Status:** Engineering estimate with numerical cross-checks; not a timed native-runtime benchmark.

## Executive result

The native math subject exposes constants and scalar operations through `import math;`. The current implementation is a VM-executed subject whose functions are assembled from ordinary SLeeLa Core methods. Its algorithms are deliberately self-contained: Newton iteration for square root, range-reduced series for exponential and logarithm, and a range-reduced Taylor series for sine.

The source-defined math regression program contains **27 assertions** with tolerances ranging from exact comparisons for simple selection/rounding operations to (10^{-4}) for the broad logarithm case. The algorithms reproduce the representative reference values below to substantially tighter error than those tolerances in an independent floating-point calculation. This is a numerical cross-check of the source algorithms, **not evidence that the 27 assertions were executed by the current `sleela` binary**.

Speed figures below are conservative planning estimates for a warmed, optimized Linux x86-64 build. They are included to give the project a useful first performance envelope. They are **not measured timings** and must not be reported as benchmark observations.

## 1. Accuracy sample

The following values were calculated by independently reproducing the current source algorithms using IEEE-754 double precision and comparing their outputs with the corresponding standard mathematical reference values.

| Operation | Input | Subject algorithm result (approx.) | Reference | Absolute error |
|---|---:|---:|---:|---:|
| `sqrt` | 2 | 1.414213562373095 | 1.4142135623730951 | 2.22e-16 |
| `sqrt` | 144 | 12.000000000000000 | 12 | 0 |
| `exp` | 0 | 1.000000000000000 | 1 | 0 |
| `exp` | 1 | 2.7182818284590455 | 2.718281828459045 | 4.44e-16 |
| `log` | e | 1.000000000000000 | 1 | 0 |
| `log` | 1000 | 6.907755278982137 | 6.907755278982137 | 0 |
| `log10` | 1000 | 2.9999999999999996 | 3 | 4.44e-16 |
| `sin` | pi/2 | 1.0000000000000002 | 1 | 2.22e-16 |
| `cos` | 0 | 1.0000000000000002 | 1 | 2.22e-16 |
| `tan` | pi/4 | 0.9999999999999997 | 1 | 3.33e-16 |
| `pow` | 2^10 | 1024.0000000000005 | 1024 | 4.55e-13 |
| `hypot` | (3, 4) | 5.000000000000000 | 5 | 0 |

**Interpretation:** On these representative inputs, the approximation error is at or near double-precision rounding noise. This should not be generalized to all magnitudes or domains: the fixed iteration counts, range reduction, and domain guards impose limits described below.

## 2. Accuracy contract in the checked-in regression program

The existing `impl/tests/subjects/math_values.sleela` declares 27 checks. Its current tolerances include:

- (10^{-6}) for ordinary square-root, trigonometric, and hypotenuse checks.
- (10^{-5}) for powers, exponential, logarithm-of-e, pi, and log10(1000).
- (10^{-4}) for `log(1000)` and `log10(82100)`.
- Zero tolerance for exact selection, clamp, and integer-rounding semantics.

The program also covers positive and negative `floor`, `ceil`, and `trunc`, as well as `min`, `max`, `clamp`, `abs`, `fmod`, and sign-independent boundary behavior.

**Execution status:** 27 assertions are present in source. This report has not run `make -C impl test-math` or `sleelactl run math` in a live build, so it does not claim a pass count or a runtime result.

## 3. Initial speed envelope (estimated, not measured)

These ranges are engineering estimates for a warmed optimized runtime, expressed as approximate per-call latency. They account for VM dispatch and the algorithmic work visible in the implementation; they are not derived from a stopwatch run.

| Function family | Representative functions | Estimated latency per call | Estimated throughput |
|---|---|---:|---:|
| Scalar/basic | `abs`, `sign`, `min`, `max`, `clamp` | 2–10 µs | 100k–500k calls/s |
| Integer-like | `floor`, `ceil`, `trunc`, `fmod` | 3–12 µs | 83k–333k calls/s |
| Iterative root | `sqrt`, `hypot` | 5–20 µs | 50k–200k calls/s |
| Range-reduced series | `exp`, `log`, `log10` | 10–50 µs | 20k–100k calls/s |
| Trigonometric | `sin`, `cos`, `tan` | 10–50 µs | 20k–100k calls/s |
| Composite | `pow` | 15–70 µs | 14k–67k calls/s |

The ranges are intentionally broad. Actual results will depend on the CPU, compiler, runtime build flags, VM dispatch overhead, call-site setup, and whether the timer includes program startup and output. A future measured report should include median and p95 latency, warm-up count, iteration count, compiler/build identity, CPU model, OS, and exact command.

## 4. Algorithmic cost and expected behavior

- **`sqrt`:** rescales by powers of four, then performs 20 Newton steps. Cost is bounded by the number of scaling steps plus 20 iterations.
- **`exp`:** reduces the input into approximately [-1, 1], then accumulates a 29-step Maclaurin series. Large positive or negative inputs add range-reduction work.
- **`log`:** scales the input by e into a band around one, then evaluates a 23-step atanh series. The reduction count grows with the logarithm of the input magnitude.
- **`sin`:** reduces by tau into [-pi, pi], folds the result, then evaluates 13 recurrence steps. `cos` delegates to `sin`; `tan` divides sine by cosine.
- **`pow`:** composes `log` and `exp`; it is therefore more expensive than a single primitive operation.
- **`hypot`:** currently computes `sqrt(a*a + b*b)`; this is simple but can overflow for very large finite operands where a scaled hypot algorithm would remain finite.

## 5. Domain and numerical limitations to track

The current behavior is useful for the present demonstration and ordinary finite values, but it is not yet a full replacement for the C/C++ standard math library.

1. `sqrt(x <= 0)` returns zero, including negative inputs; a standards-oriented API should distinguish zero from a domain error.
2. `log(x <= 0)` returns zero rather than NaN, negative infinity, or an explicit error.
3. `pow(x, y)` is implemented as `exp(y * log(x))`. Negative bases and several zero/infinity cases therefore do not follow standard `pow` semantics.
4. `tan` is numerically ill-conditioned near odd multiples of pi/2; callers should not expect a small absolute error near its poles.
5. `hypot` can overflow in the intermediate squares for very large inputs and can underflow for very small inputs.
6. Fixed iteration counts and ordinary floating-point range reduction deserve dedicated tests for subnormals, very large magnitudes, infinities, NaNs, and values near reduction boundaries.

## 6. Recommended measured benchmark

When a current runtime binary is available, run the following from the repository root:

```sh
make -C impl test-math
sleelactl run math
```

Then add a dedicated benchmark program that:
- warms each function before timing;
- performs at least 100,000 calls per case (more for basic operations);
- consumes and checks accumulated results so the work cannot be optimized away;
- uses a monotonic high-resolution clock;
- reports median, p95, and calls/second across at least 7 repetitions;
- separates process startup and printing from the timed region;
- compares the same inputs against `std::sqrt`, `std::exp`, `std::log`, and the platform math library as a reference baseline.

Suggested acceptance gates for the next measured run are: all 27 source assertions pass; ordinary finite-domain accuracy remains inside the current declared tolerances; no non-finite result appears for documented in-domain test inputs; and no performance regression greater than 20% is accepted without explanation.

## 7. Source basis

- `impl/subjects/math/math_native.cpp` — current native subject algorithms and iteration counts.
- `lib/ui/example/science/math.sleela` — user-facing demonstration exercised by `sleelactl run math`.
- `impl/tests/subjects/math_values.sleela` — 27 numeric assertions and tolerance choices.
- `impl/Makefile` — `test-math` target and shared subject assertion harness.
- `bin/SLEELACTL.md` — documents the `subject` nature and the `sleelactl run math` execution path.

---

**Bottom line:** The source algorithms show excellent agreement on the representative finite inputs sampled here, and the current regression file defines a sensible 27-case starting accuracy contract. The speed figures are an explicitly estimated planning envelope only. A timed run of the built `sleela` runtime is the remaining step before publishing measured throughput.
