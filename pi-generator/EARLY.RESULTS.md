# EARLY RESULTS — SLeeLa and C++ Pi Runtime Test

**Date:** 2026-11-09  
**Status:** Preliminary, measured smoke benchmark  
**Platform:** Linux x86-64; GCC 14.2.0  
**Runtime tested:** SLeeLa Language/Runtime 1.3.2-pro

## Summary

An executable SLeeLa runtime and a matching C++17 program were run against the same Machin-formula floating-point Pi workload. Both implementations completed 5,000 calculations and passed all 5,000 per-result accuracy assertions, requiring absolute error below `1e-12`.

| Metric | SLeeLa | C++17 |
|---|---:|---:|
| Median wall time (9 process runs) | 2,161.163 ms | 2.880 ms |
| Maximum of 9 runs (reported as small-sample p95) | 2,854.190 ms | 7.646 ms |
| Minimum | 1,353.366 ms | 2.226 ms |
| Accuracy checks | 5,000 / 5,000 PASS | 5,000 / 5,000 PASS |

For this specific interpreted floating-point workload, the C++ median was approximately 750 times faster. This is an early, workload-specific observation, not a general SLeeLa-versus-C++ performance claim.

## Workload and method

Both programs evaluated Machin's identity:

`pi = 16 * atan(1/5) - 4 * atan(1/239)`

The arctangent series used 100 terms for `atan(1/5)` and 20 terms for `atan(1/239)`. Each program repeated the calculation 5,000 times, checked each result against the same (10^{-12}) tolerance, and accumulated a checksum.

The SLeeLa source passed `sleela check` before execution. The C++ program was compiled with:

```sh
g++ -std=c++17 -O2 -Wall -Wextra -pedantic pi-machin-bench.cpp -o pi-machin-cpp
```

Nine independent process-level wall-time samples were collected for each implementation. The samples include process startup; SLeeLa timings also include its runtime integrity-verification step.

- **SLeeLa (ms):** 2637.006, 2241.776, 1737.767, 2275.447, 2161.163, 2854.190, 1827.003, 1451.998, 1353.366
- **C++ (ms):** 7.646, 3.037, 4.672, 2.880, 2.226, 2.778, 2.312, 7.010, 2.488

The reported p95 is the maximum of nine samples, stated explicitly because this sample size is too small for a stable tail-latency estimate.

## Runtime integrity

The uploaded SLeeLa executable matched the repository-published SHA-256:

`0653c9f90dec0f9e3e22ac929c36ce09acf40bb464c61e49176e565dff07fc48`

It was identified as a dynamically linked Linux x86-64 ELF executable, and its shared-library dependencies resolved in the test environment. The runtime's verification gate passed using an isolated temporary root and a local manifest hashing the executable and verifier. This local test manifest was not a replacement for the repository's production integrity manifest.

## Limitations — important

- The tested binary did not include the separate `sleelactl` front end. The test used `sleela check` / `sleela run` directly and GCC for C++; it did not exercise `sleelactl` or `tools/sleela-native-drive.py`.
- This was a floating-point Machin-formula test. The repository's `PiChudnovsky.sleela` base-1e9 big-integer binary-splitting implementation was **not** run.
- SLeeLa's default numeric-to-text output rounded the accumulated checksum to `15708`, whereas C++ printed `15707.963267950136`. The per-calculation assertions passed, but the SLeeLa checksum display is not a high-precision accuracy result.
- The timings are specific to one Linux x86-64 environment, this runtime build, and this test harness. They should not be extrapolated to other SLeeLa execution paths, native big-integer primitives, operating systems, or digit counts.

## Next benchmark required

The next meaningful Pi performance test is a matched high-precision Chudnovsky comparison:

1. Run the repository's `PiChudnovsky.sleela` with its intended runtime and full source-tree dependencies.
2. Implement or build the equivalent C++17 binary-splitting algorithm.
3. Generate the same digit count in both implementations.
4. Compare every output digit and record output hashes.
5. Measure repeated wall times and peak memory.
6. Separately validate the `sleelactl` / Native Drive route once its complete wrapper and dependencies are available.

## Full report

The detailed report, including raw samples and the complete environment notes, is maintained at:

- `pi-generator/PI-MACHIN-RUNTIME-COMPARISON-2026-11-09.md`
- `lib/pi-generator/PI-MACHIN-RUNTIME-COMPARISON-2026-11-09.md`

**Conclusion:** The first SLeeLa/C++ runtime smoke comparison has been executed and both implementations passed the same accuracy gate. High-precision Chudnovsky performance and Native Drive integration remain open; these early results do not claim otherwise.
