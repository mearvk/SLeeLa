# SLeeLa / sleelactl — 20,000-Digit π Formula and Numeric-Type Study

**Date:** 2026-11-09  
**Status:** Prototype study and review plan; SLeeLa timing values are drawn from the repository's existing measurements or clearly marked extrapolations.  
**Target:** An interrogative/iterative 20,000-fractional-digit π calculation through the early `sleelactl` math-subject interface.

## Descriptive overview

This document describes an **early prototype of `sleelactl`**, using its math subject as the proposed control and demonstration surface for a high-precision numerical workload. The question under investigation is: *Can the SLeeLa math stack calculate, verify, and report 20,000 fractional digits of π consistently across the available integer, floating-point, and arbitrary-precision representations, while making the time and accuracy trade-offs visible?* The existing `sleelactl` math subject demonstrates scalar mathematical operations; the existing Pi generator supplies the arbitrary-precision algorithms and native big-integer operations needed for this larger task. These are related but distinct layers: a scalar `double` math function cannot itself retain 20,000 decimal digits.

**Important execution qualification:** this is a design-and-results review based on checked-in generator documentation and previously recorded SLeeLa timings. No new 20,000-digit run was launched through `sleelactl` in this session. Accordingly, estimated times are not represented as new measurements, and the document specifies the run and verification needed to turn this prototype study into a measured result.

## 1. Review summary

| Question | Current answer | Evidence / qualification |
|---|---|---|
| Can binary64 `double` hold 20,000 digits? | No | IEEE-754 binary64 has about 15–17 significant decimal digits |
| Can `float` hold 20,000 digits? | No | Binary32 has about 6–9 significant decimal digits |
| Can a fixed-width integer hold π to 20,000 digits? | No | Fixed-width types can hold limbs, carries, indices, and intermediate products, not the complete scaled value |
| Can SLeeLa's arbitrary-precision limb representation do it? | Yes, in principle and in the existing Pi generator | Base-1e9 limb arrays; existing Chudnovsky implementation uses native big-integer operations |
| Best current high-precision algorithm in the repository | Chudnovsky with binary splitting | Native `bigmul`, `bigadd`, `bigsub`, `bigdiv`, `bigisqrt` |
| Best documented 20k time estimate | Machin: about 11 s; Chudnovsky: about 15 s | Both are historical extrapolations, not a fresh 20k measurement; see timing table |
| Recommended next measured run | Chudnovsky at 20,000 digits, then Machin and reference cross-check | Capture whole-process and phase timings separately |

## 2. Formula families that can calculate π

The formulas below are not all equally efficient, nor are they all implemented in the current SLeeLa Pi generator. The table distinguishes existing implementations from viable follow-on algorithms.

| Formula / method | Core identity or approach | Convergence / complexity characteristics | High-precision suitability | SLeeLa status |
|---|---|---|---|---|
| **Chudnovsky** | (1/\pi = 12\sum_{k\ge0}(-1)^k(6k)!(13591409+545140134k)/((3k)!(k!)^3 640320^{3k+3/2})) | About 14.18 decimal digits per term; binary splitting reduces series-combination cost; final square root and division are still important costs | **Excellent; recommended** | Implemented in `PiChudnovsky.sleela`; native big-integer opcodes |
| **Machin** | \(\pi=16\arctan(1/5)-4\arctan(1/239)\) | Alternating arctangent series; current limb-array accumulation is O(n²) | Good for tens of thousands of digits; less attractive at very large scales | Implemented in `PiMachin.sleela` |
| **Rabinowitz–Wagon spigot** | Emits decimal digits using a bounded-integer state array | O(n²) interpreted array sweeps | Correct but slow as digit count grows | Implemented in `PiGenerator.sleela` |
| **Gauss–Legendre / AGM (Brent–Salamin)** | Arithmetic-geometric mean iteration; quadratic convergence | Precision roughly doubles per iteration; needs high-precision multiplication, division, and square root | Excellent alternative for cross-checking and high precision | Not identified as a current Pi generator implementation |
| **Borwein quadratic / quartic algorithms** | Iterative algebraic transformations with quadratic or quartic convergence | Very few iterations, but each iteration requires high-precision roots and products | Excellent when arbitrary-precision roots and products are robust | Candidate future implementation |
| **Ramanujan series** | Rapid hypergeometric series for (1/\pi) | Roughly 8 decimal digits per term; binary splitting can accelerate it | Very good independent series cross-check | Not identified as current generator implementation |
| **Ramanujan–Sato series** | Related rapidly convergent series with modular-form constants | Can exceed the classic Ramanujan rate, depending on series | Excellent, but more complex constants and proof/implementation burden | Research candidate |
| **BBP formula** | Extracts hexadecimal/binary digits of π at selected positions | Useful for digit-position checks; not the most direct route to 20,000 consecutive decimal digits | Useful independent spot-check, not primary decimal generator | Not identified as implemented |
| **Leibniz series** | \(\pi/4=1-1/3+1/5-1/7+\cdots\) | O(n) terms for only slow error reduction; impractical for 20k digits | Unsuitable | Reference-only teaching formula |
| **Wallis product** | Infinite product for π/2 | Very slow convergence | Unsuitable | Reference-only teaching formula |
| **Euler / Nilakantha series** | Alternating or rapidly decreasing elementary series | Better than Leibniz in practice, still far slower than Chudnovsky/AGM for 20k digits | Useful small-scale independent check, not 20k production | Reference-only / possible test |
| **Monte Carlo** | Estimate π from random points in a geometric region | Statistical error falls only as (1/\sqrt{N}) | Fundamentally unsuitable for deterministic 20k-digit output | Excluded from precision result |

**Recommended primary algorithm:** Chudnovsky binary splitting.  
**Recommended independent algorithm:** Machin for a second implementation already present in the repository; add Gauss–Legendre or Ramanujan as a future third independent method. A method is not independent merely because it has a different class name: its arithmetic path and implementation should differ enough to catch shared defects.

## 3. Numeric representation matrix

A 20,000-digit result must be stored as an arbitrary-precision integer or a decimal digit stream. Fixed-width types remain important for limb arithmetic and bookkeeping, but none of them can represent the full result alone.

| Numeric type / representation | Typical precision or range | Role in a 20,000-digit π calculation | Can it hold all 20,000 digits? | Review |
|---|---|---|---|---|
| `int8` / signed 8-bit | −128…127 | Small flags, tiny counters only | No | Not suitable for limbs |
| `uint8` / unsigned 8-bit | 0…255 | Byte output buffers and digit bytes | No | Useful for serialization |
| `int16` / signed 16-bit | −32,768…32,767 | Small indices or bounded constants | No | Not suitable for base-1e9 limbs |
| `uint16` / unsigned 16-bit | 0…65,535 | Compact digit/metadata storage | No | Not suitable for base-1e9 limbs |
| `int32` / `int` | Usually signed 32-bit in C-like ABIs; language-specific | Base-1e9 limb storage is valid because each limb is 0…999,999,999; loop counters and small divisors | No | **Current SLeeLa generator's principal limb type** |
| `uint32` | 0…4,294,967,295 | Native kernel limb representation and carry arithmetic | No | Native base-1e9 kernel representation |
| `int64` / `long` | Usually signed 64-bit; ABI/language-dependent | Products of two base-1e9 limbs (up to about 10^18), carries, intermediate arithmetic, offsets | No | Strong candidate for native multiplication intermediates |
| `uint64` / `unsigned long long` | 0…2^64−1 | Native limb products and unsigned carry paths | No | Check overflow bounds and portability explicitly |
| `int128` / `__int128` | 128-bit integer where supported | Wider products, exact intermediate arithmetic, optimized division helpers | No | Optional acceleration; not universally portable |
| `float` / binary32 | About 6–9 significant decimal digits | Approximate seeds, plotting, coarse diagnostics | No | Never use for digit production |
| `double` / binary64 | About 15–17 significant decimal digits | Scalar `math` subject, timing, approximate seed, low-precision sanity check | No | Cannot certify or store a 20k-digit result |
| `long double` | Platform-dependent; commonly 64, 80, or 128-bit format | Higher-precision approximate seed/reference on some platforms | No | ABI and platform differences prevent treating it as a portable 20k-digit type |
| Fixed-point decimal / scaled integer | Precision chosen by scale and storage width | Exact decimal scaling when backed by a large integer | Only if arbitrary-width backing is used | Viable as a formatting/representation layer |
| Decimal floating point | Precision depends on implementation/context | Decimal reference calculations and independent validation | Only if the implementation supports ≥20,000 digits | Good reference path; not a fixed-width primitive |
| Base-1e9 arbitrary-precision limb array | 9 decimal digits per limb | Stores the scaled integer, partial sums, products, quotient, remainder, and square-root result | **Yes** | **Current SLeeLa Pi architecture** |
| Decimal digit string / streamed file | One character per decimal digit | Final output, hashing, and external comparison | Yes, as text | Output representation only; not arithmetic storage |

### Limb arithmetic safety notes

- With base (B=10^9), a single limb is in ([0,B-1]).
- The product of two limbs is less than (10^{18}), which fits within a signed 64-bit integer ((approx9.22\times10^{18})) before adding a suitably bounded carry. The actual kernel must prove its maximum accumulator bound, not rely on this simplified product bound alone.
- A base-1e9 array represents a large integer as (X=\sum_i a_i B^i). About 2,223 limbs hold 20,000 decimal fractional places, before guard digits and any extra working width.
- The current Chudnovsky implementation uses signed magnitude (magnitude limb array plus sign) for intermediate (T) values.
- The final decimal output should be streamed in chunks; do not concatenate an unbounded 20,000-character result repeatedly inside the hot arithmetic loop.

## 4. Existing SLeeLa performance baseline and 20,000-digit review estimate

The figures below are drawn from the repository's Pi generator documentation, especially `pi-generator/README.md` and `pi-generator/NATIVE.ADD.md`. They are not new runs performed for this report.

| Generator / revision | Digits | Wall time | Evidence class | Notes |
|---|---:|---:|---|---|
| Spigot v2.0 | 1,000 | ~2 s | Recorded measurement | Reference match documented |
| Spigot v2.0 | 2,000 | ~4 s | Recorded measurement | O(n²) |
| Spigot v2.0 | 4,000 | ~9 s | Recorded measurement | O(n²) |
| Spigot v2.0 | 10,000 | ~48–51 s | Recorded measurement across notes | O(n²); 51 s appears in the generator comparison |
| Spigot v2.0 | 20,000 | ~192–200 s | Extrapolation | Approximately 3.2–3.3 minutes |
| Machin v2.0 | 1,000 | <1 s | Recorded measurement | Native small-divisor big-integer kernel |
| Machin v2.0 | 10,000 | ~3 s | Recorded measurement | O(n²) overall |
| Machin v2.0 | 20,000 | ~11 s | Extrapolation | Existing README estimate; about 18× faster than spigot estimate |
| Chudnovsky v3.0 before fast-isqrt seed | 10,000 | ~9 s | Earlier recorded measurement | Before R1 improvement |
| Chudnovsky v3.0 after R1 fast-isqrt seed | 10,000 | ~4 s | Recorded measurement | Digit-for-digit reference check documented |
| Chudnovsky v3.0 after R1 fast-isqrt seed | 20,000 | **~15 s** | **Extrapolation for this review** | Interpolated from 10k ≈4 s and 50k ≈87 s; not a measured 20k run |
| Chudnovsky v3.0 after R1 fast-isqrt seed | 50,000 | ~87 s | Recorded measurement | Digit-for-digit reference check documented |

### How the 20k Chudnovsky estimate was obtained

The measured 10k-to-50k ratio is approximately (87/4=21.75) for a 5× increase in digits. A simple power-law fit gives an effective exponent near 1.92 over that interval. Applying that local fit to 10k→20k gives approximately:

[
t(20{,}000)\approx4\cdot2^{1.92}\approx15\text{ seconds}
]

This is a planning estimate only. It inherits the machine, build, GC, and measurement conditions of the earlier runs and must be replaced by a timed 20k execution. The existing 50k report attributes most of the runtime to the final `bigisqrt` and schoolbook big division; the binary-splitting recursion itself is already a small fraction of the run.

## 5. Accuracy and verification plan

A 20,000-digit run should be treated as verified only after all of the following checks pass:

| Check | Procedure | Acceptance criterion |
|---|---|---|
| Output shape | Count digits after the decimal point; validate the leading integer and decimal point | Exactly 20,000 fractional digits, one leading `3.`, no embedded whitespace |
| Known prefix | Compare the first 100 digits with a trusted π reference | Exact match |
| Interior landmarks | Check well-known interior positions, including the Feynman point at fractional positions 762–767 | Exact match |
| Full independent reference | Compare all 20,000 digits against an independently generated high-precision reference | Byte-for-byte match |
| Second SLeeLa algorithm | Run Machin at 20,000 digits and compare with Chudnovsky | Exact match |
| Checksum | SHA-256 the fractional digit stream | Record checksum and byte count |
| Guard precision | Re-run with a larger guard-digit count | The first 20,000 fractional digits remain identical |
| Repeatability | Run the same build and configuration at least twice | Same output hash; timing variance recorded |
| Numeric edge behavior | Exercise carry propagation, borrow propagation, signed intermediate values, division remainder, and square-root floor behavior | All invariants hold; no overflow or silent truncation |
| Timing | Capture total wall time and per-phase monotonic timings | Record build, CPU, OS, run count, median, and range |

For the reference, the repository already documents independent Python `decimal`/Chudnovsky comparisons and a 100-million-digit reference file. The strongest comparison for this test is a fresh independent high-precision reference plus the existing checked-in reference prefix; do not rely only on two generators that share the same big-integer kernel.

## 6. Proposed 20,000-digit execution matrix

| Run | Generator | Arithmetic path | Requested fractional digits | Expected time (planning only) | Purpose |
|---:|---|---|---:|---:|---|
| 1 | `PiChudnovsky` | Binary splitting + native big integer | 20,000 | ~15 s estimated | Primary result |
| 2 | `PiMachin` | Alternating arctangent + native small-divisor operations | 20,000 | ~11 s estimated | Independent formula cross-check |
| 3 | `PiGenerator` | Rabinowitz–Wagon spigot | 20,000 | ~192–200 s estimated | Algorithmic baseline only; optional if runtime budget is tight |
| 4 | `PiChudnovsky` | Same algorithm, increased guard digits | 20,000 | To be measured | Guard-digit stability |
| 5 | External high-precision reference | Independent decimal/Chudnovsky implementation | 20,000 | Environment-dependent | Full output validation |

The expected times in this matrix are inherited estimates or extrapolations from existing repository notes. They are not execution results for this 20,000-digit test.

## 7. Relationship to `sleelactl` and the scalar math subject

The current math-subject demo and numerical assertion suite exercise scalar operations such as `sqrt`, `pow`, `exp`, `log`, `sin`, `cos`, `hypot`, and rounding. Those are important for ordinary numerical programs, but a binary64 scalar cannot preserve 20,000 digits. The Pi generator therefore tests a different capability: arbitrary-precision integer arithmetic, signed intermediate values, multiplication algorithms, division, integer square root, memory management, and streamed output.

The proposed early-prototype integration is to make `sleelactl` expose a coherent high-precision math task (for example, `sleelactl run pi --digits 20000 --algorithm chudnovsky`) while preserving the existing `sleelactl info math` and `sleelactl run math` scalar-subject paths. This is a proposed interface, not a claim that these exact Pi CLI flags already exist.

The result should report at minimum:

- algorithm and formula;
- requested and produced digit counts;
- numeric representation (base-1e9 limbs, limb count, guard digits);
- total wall time and phase timings;
- output byte count and SHA-256;
- reference comparison result;
- runtime/compiler commit, build flags, CPU, and OS;
- whether a timing is measured, estimated, or extrapolated.

## 8. Source references

- `pi-generator/README.md` — historical spigot and Machin measurements; Version 3.0 generator comparison; 1k/10k/50k Chudnovsky performance.
- `pi-generator/NATIVE.ADD.md` — native big-integer opcode design, phase timings, fast-isqrt R1 improvement, and remaining schoolbook-division bottleneck.
- `pi-generator/PiGenerator.sleela` — Rabinowitz–Wagon spigot implementation.
- `pi-generator/PiMachin.sleela` — Machin formula over the native big-integer kernel.
- `pi-generator/PiChudnovsky.sleela` — Chudnovsky binary-splitting implementation.
- `lib/pi-generator/ADVANTAGE-LIBRARY-VERIFICATION-2026-11-09.md` — ALV #395 verification record on the math-synthesis branch (6/6 demonstrations and 12,499 integrity items); this verifies the Advantage library workflow, not the 20,000-digit π workload.
- `impl/subjects/math/math_native.cpp` and `impl/tests/subjects/math_values.sleela` — scalar math subject implementation and numerical assertions.

---

## Review disposition

**Recommended:** approve Chudnovsky as the primary 20,000-digit candidate, retain Machin as an independent formula check, and use base-1e9 limb arrays for the actual precision. Treat `float`, `double`, `long double`, and fixed-width integers as helper representations only, never as the 20,000-digit result store.

**Not yet claimed:** a new SLeeLa 20,000-digit execution, a measured 20k runtime, or a successful `sleelactl` Pi command. The next implementation/benchmark step is to run the matrix above on the source-built runtime and replace the estimates with measured timings and a full 20,000-digit comparison.
