# Pi Runtime Comparison — SLeeLa and C++

**Date:** 2026-11-09  
**Environment:** Linux x86-64, GNU/Linux 3.2+ ABI, GCC 14.2.0  
**Scope:** Executed Machin-formula floating-point Pi workload in the uploaded SLeeLa runtime and a matching C++17 program. This is a runtime smoke/throughput comparison, **not** a 20,000-digit Chudnovsky benchmark.

## Executive result

Both implementations completed 5,000 identical Pi calculations and passed the same per-result accuracy assertion: the result had to fall within `3.141592653588793` and `3.141592653590793` (absolute error below `1e-12` against the reference value used by the test).

| Implementation | Median wall time | p95 (max of 9 samples) | Minimum | Accuracy assertion |
|---|---:|---:|---:|---|
| SLeeLa 1.3.2-pro (`sleela run`) | 2,161.163 ms | 2,854.190 ms | 1,353.366 ms | PASS, 5,000 / 5,000 |
| C++17 (`g++ -O2`) | 2.880 ms | 7.646 ms | 2.226 ms | PASS, 5,000 / 5,000 |

The measured median for this small floating-point workload was approximately **750× faster in C++**. This ratio is specific to this interpreted SLeeLa workload, its runtime build, and this test harness; it must not be generalized to native SLeeLa big-integer opcodes or to the full Chudnovsky generator.

## Workload

Both programs evaluated Machin's identity:

`pi = 16 * atan(1/5) - 4 * atan(1/239)`

Each arctangent used the alternating power series with 100 terms for `atan(1/5)` and 20 terms for `atan(1/239)`. Each program repeated the calculation 5,000 times, checked every result against the same `1e-12` interval, and accumulated a checksum to keep the work observable.

The SLeeLa source was checked with `sleela check` before execution. The C++ source was compiled with:

```sh
g++ -std=c++17 -O2 -Wall -Wextra -pedantic pi-machin-bench.cpp -o pi-machin-cpp
```

## Raw timing samples

Nine independent process executions per implementation; process startup, runtime verification, and program execution are included in the wall time.

- **SLeeLa (ms):** 2637.006, 2241.776, 1737.767, 2275.447, 2161.163, 2854.190, 1827.003, 1451.998, 1353.366
- **C++ (ms):** 7.646, 3.037, 4.672, 2.880, 2.226, 2.778, 2.312, 7.010, 2.488

The p95 shown above is the maximum of the nine observations, a deliberately transparent small-sample convention rather than a statistically stable tail-latency estimate.

## Integrity and environment checks

The uploaded `sleela` executable matched the repository-published SHA-256:

`0653c9f90dec0f9e3e22ac929c36ce09acf40bb464c61e49176e565dff07fc48`

`file` identified it as a dynamically linked ELF 64-bit x86-64 executable. `ldd` resolved its listed shared-library dependencies in the test environment. The runtime reported **SLeeLa Language/Runtime 1.3.2-pro**, source syntax 1.0–3.0, and a 2 GiB memory-manager limit.

The runtime requires a SLeeLa root, `tools/verify-before-execution.py`, and a SHA-256 manifest before source execution. To avoid disabling this gate, testing used an isolated temporary root containing the uploaded executable and the repository's verifier implementation, with a local manifest hashing both files. The gate reported `verification: PASS (2 item(s))` before the SLeeLa check and run. This local test manifest is not a replacement for the repository's production integrity manifest.

## Important limitations

1. The uploaded executable does not include the separate `sleelactl` front end. Its help exposes `sleela compile`, `check`, and `run`, among other commands. Therefore this test invoked `sleela run` directly and compiled C++ with GCC; it did **not** claim to exercise `sleelactl` or `tools/sleela-native-drive.py`.
2. The repository's `pi-generator/PiChudnovsky.sleela` was not run in this test. The measured SLeeLa program was a small Machin-formula floating-point workload, not the repository's base-1e9 big-integer binary-splitting program.
3. SLeeLa's default numeric-to-text output rendered the accumulated checksum as `15708`, while C++ printed `15707.963267950136`. This is a formatting/printing precision limitation in the displayed checksum; the actual per-iteration floating-point assertion passed. Do not use the displayed SLeeLa checksum as a high-precision accuracy measurement.
4. These timings are process-level measurements from one Linux x86-64 environment. They include SLeeLa's integrity verification and process startup, and they are not a controlled cross-platform benchmark.

## Next required benchmark

For the intended high-precision Pi comparison, run the repository's `PiChudnovsky.sleela` through a complete source checkout with the matching runtime, then compare it with a C++17 binary-splitting Chudnovsky implementation using the same digit count and output verification. Record output hashes, digit-by-digit comparison, peak memory, and repeated timings. The separate `sleelactl` / Native Drive path should be tested only after its wrapper and source-tree dependencies are available in the execution environment.

---

**Result:** The uploaded SLeeLa binary is executable and can run SLeeLa source successfully. A matching SLeeLa/C++ Machin Pi smoke benchmark passed its accuracy gate in both languages. The full high-precision Chudnovsky and `sleelactl` Native Drive benchmarks remain open.
