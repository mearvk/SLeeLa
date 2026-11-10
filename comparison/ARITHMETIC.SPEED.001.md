# Arithmetic Speed Series — Iteration 001

**Suite:** SCS-PERF-ARITH-001  
**Run status:** PASS — local reference execution  
**Recorded:** 2026-11-07  
**Scope:** C11, C++17, Java 21+, Basic Native SLeeLa VM, and Premium SLVM/1–11; four arithmetic operators across fixed-width integer and floating-point types. Native-language timings are measured; SLeeLa VM execution qualification is recorded separately.

## 1. Result

The full arithmetic operator sweep executed for all supported target/type combinations. The run completed five measured repetitions per case, after one native warm-up run and three in-process Java warm-up sweeps. All 40 common type/operator checksums matched across C11, C++17, and Java. C/C++ additionally ran four signed-128-bit cases; Java has no primitive 128-bit integer.

The reported values are median nanoseconds per operation (ns/op). Lower is faster for this specific microbenchmark. These are local-container observations, not universal language rankings or release-grade measurements.

## 2. Host and toolchain

| Property | Value |
|---|---|
| OS | Linux 6.18.57 x86_64, glibc 2.41 |
| CPU | AMD EPYC 9V74 80-Core Processor (container allocation: 3 logical CPUs) |
| RAM visible to container | 6,091,004 kB |
| C compiler | GCC 14.2.0 |
| C++ compiler | G++ 14.2.0 |
| Java | OpenJDK 21.0.12.1 |
| Native optimization | `-O2` |
| Operations per case | 2,000,000 |
| Operand set | 4,096 deterministic elements; shared seed `0x53435350` |
| Repetitions | 5 measured samples per type/operator |
| Warm-up | One untimed native process; three Java warm-up sweeps inside each measured JVM |
| Timing source | Monotonic high-resolution clock |

## 3. Full results — median ns/op

| Type | Operator | C11 (ns/op) | C++17 (ns/op) | Java 21+ (ns/op) | Basic Native VM (ns/op) | Premium SLVM/1 (ns/op) | Premium SLVM/2 (ns/op) | Premium SLVM/3 (ns/op) | Premium SLVM/4 (ns/op) | Premium SLVM/5 (ns/op) | Premium SLVM/6 (ns/op) | Premium SLVM/7 (ns/op) | Premium SLVM/8 (ns/op) | Premium SLVM/9 (ns/op) | Premium SLVM/10 (ns/op) | Premium SLVM/11 (ns/op) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| i8 | `+` | 0.575 | 0.603 | 0.824 | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED |
| i8 | `-` | 1.128 | 1.173 | 1.382 | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED |
| i8 | `*` | 1.241 | 1.276 | 5.483 | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED |
| i8 | `/` | 1.704 | 1.732 | 7.171 | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED |
| u8 | `+` | 0.593 | 0.570 | 1.075 | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED |
| u8 | `-` | 1.220 | 0.835 | 1.164 | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED |
| u8 | `*` | 1.281 | 0.979 | 1.798 | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED |
| u8 | `/` | 1.116 | 1.143 | 2.476 | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED |
| i16 | `+` | 0.567 | 0.603 | 1.259 | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED |
| i16 | `-` | 1.121 | 1.164 | 1.364 | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED |
| i16 | `*` | 1.264 | 1.191 | 1.818 | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED |
| i16 | `/` | 1.768 | 1.729 | 2.251 | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED |
| u16 | `+` | 0.662 | 0.645 | 1.102 | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED |
| u16 | `-` | 1.175 | 1.138 | 1.121 | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED |
| u16 | `*` | 1.162 | 1.239 | 1.026 | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED |
| u16 | `/` | 1.160 | 1.154 | 1.985 | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED |
| i32 | `+` | 0.663 | 0.679 | 1.158 | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED |
| i32 | `-` | 1.230 | 0.987 | 1.510 | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED |
| i32 | `*` | 1.262 | 1.013 | 1.218 | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED |
| i32 | `/` | 1.754 | 1.693 | 2.017 | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED |
| u32 | `+` | 0.707 | 0.736 | 0.602 | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED |
| u32 | `-` | 1.244 | 1.191 | 1.230 | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED |
| u32 | `*` | 1.421 | 1.317 | 0.966 | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED |
| u32 | `/` | 1.710 | 1.827 | 2.001 | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED |
| i64 | `+` | 0.587 | 0.687 | 0.708 | 46.740 | 48.300 | 48.537 | 48.608 | 48.532 | 48.446 | 48.531 | 48.490 | 48.557 | 49.177 | 49.040 | 48.677 |
| i64 | `-` | 1.180 | 1.254 | 1.110 | 46.532 | 48.354 | 48.560 | 48.533 | 48.361 | 48.314 | 48.446 | 48.609 | 48.329 | 48.544 | 48.442 | 48.664 |
| i64 | `*` | 1.417 | 1.426 | 0.992 | 46.212 | 48.304 | 48.404 | 48.577 | 48.524 | 48.477 | 48.487 | 48.598 | 48.332 | 48.493 | 48.608 | 48.569 |
| i64 | `/` | 2.087 | 2.223 | 2.010 | 46.182 | 47.929 | 48.211 | 47.981 | 48.031 | 48.046 | 48.173 | 48.123 | 47.997 | 48.165 | 48.468 | 48.275 |
| u64 | `+` | 0.577 | 0.698 | 0.573 | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED |
| u64 | `-` | 1.209 | 1.170 | 0.728 | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED |
| u64 | `*` | 1.327 | 1.317 | 0.827 | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED |
| u64 | `/` | 1.966 | 2.043 | 1.933 | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED |
| i128 | `+` | 0.610 | 0.641 | N/A | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED |
| i128 | `-` | 0.862 | 0.798 | N/A | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED |
| i128 | `*` | 1.020 | 0.932 | N/A | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED |
| i128 | `/` | 2.431 | 3.691 | N/A | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED |
| f32 | `+` | 1.243 | 1.142 | 0.813 | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED |
| f32 | `-` | 1.261 | 1.256 | 0.839 | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED |
| f32 | `*` | 1.293 | 1.236 | 0.867 | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED |
| f32 | `/` | 1.019 | 0.967 | 0.911 | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED |
| f64 | `+` | 1.128 | 1.131 | 0.782 | 53.911 | 56.794 | 56.751 | 56.482 | 56.450 | 56.494 | 56.704 | 56.529 | 56.834 | 56.581 | 56.600 | 56.531 |
| f64 | `-` | 1.250 | 1.223 | 0.848 | 54.257 | 58.038 | 57.814 | 57.471 | 57.380 | 57.416 | 57.389 | 57.444 | 57.417 | 57.373 | 57.490 | 57.620 |
| f64 | `*` | 1.224 | 1.291 | 0.842 | 54.340 | 56.549 | 56.525 | 56.481 | 56.479 | 56.513 | 56.552 | 56.516 | 56.644 | 56.454 | 56.673 | 56.849 |
| f64 | `/` | 1.356 | 1.410 | 1.318 | 53.870 | 56.615 | 56.519 | 56.552 | 56.490 | 56.524 | 56.465 | 56.598 | 56.519 | 56.664 | 56.871 | 56.522 |


### SLeeLa VM table completion — hosted CI

**Workflow:** [SLeeLa VM arithmetic table CI — run 42346969526](https://github.com/mearvk/SLeeLa/actions/runs/42346969526)

- Basic Native and Premium SLVM/1–11 values are populated for i64 and f64 only.
- SLeeLa int maps to i64: SLEELA.syntax section 21.1 specifies int, long, and int64 as 64-bit signed integers. double maps to f64.
- Other fixed-width rows remain NOT MEASURED; no narrow signed/unsigned or float32 equivalence is inferred.
- Values are the median of five hosted-CI samples, in ns/op. Raw samples and checksums are retained in the workflow artifact.

## 4. Coverage and qualifications

- Operators: addition, subtraction, multiplication, division.
- Integer types: signed/unsigned 8-, 16-, 32-, and 64-bit; signed 128-bit where GCC/Clang-style `__int128` is available.
- Floating point: IEEE-style float32 and float64 operations on the tested toolchains.
- Java unsigned 8/16/32-bit values use masked integer emulation; unsigned 64-bit uses Java's unsigned division helper. These are not native Java unsigned primitive types.
- Java has no primitive signed 128-bit integer; no BigInteger substitution was made in this batch.
- Boolean is not included in arithmetic because boolean addition, subtraction, multiplication, and division are not defined by these language contracts.
- Integer operands are small positive values with nonzero divisors. The test intentionally avoids overflow and does not measure overflow semantics.
- The test uses independent indexed operands and checksum folding. It measures the complete loop/index/operand/arithmetic/checksum path, not an isolated hardware instruction latency.
- No memory bandwidth, random RAM latency, cache sweep, allocation, or garbage-collection performance is measured here.

## 5. SLeeLa VM arithmetic results — hosted CI, validated

**Workflow:** [Complete SLeeLa VM and Language Conformance — run 42342660507](https://github.com/mearvk/SLeeLa/actions/runs/42342660507)  
**Commit:** `ff75886e08bbff7f325d1ca84b0e86b930bda54c`  
**Artifact:** [sleela-complete-vm-matrix](https://github.com/mearvk/SLeeLa/actions/runs/42342660507/artifacts/13562815822) — `comparison-sleela-vm-arithmetic-results.json`  
**Arithmetic result:** **PASS — 12/12 runnable targets, 96/96 target-operation cases, 480/480 sample checksum validations.**

The arithmetic benchmark completed before the workflow's later Modium freshness check. That subsequent check failed because the checked-in `MODIUM.json` and `rebuild.sql` artifacts for Standard VM generations 001–010 were out of date. It did not fail the VM arithmetic test, invalidate its JSON artifact, or indicate a runtime arithmetic defect.

### 5.1 Host and measurement protocol

| Property | Value |
|---|---|
| Host OS | Linux 7.0.0-1019-azure, x86_64, glibc 2.43 |
| Logical cores | 4 |
| Targets | Basic Native + Premium SLVM/1–11 |
| Numeric cases | `int` and `double` |
| Operators | Addition, subtraction, multiplication, division |
| Iterations per case | 250,000 |
| Warm-up | 50,000 iterations per case |
| Samples | 5 per target/case |
| Timing source | SLeeLa `timeMonotonicNanos()` |
| Timed region | In-VM measured loop; process startup, compilation, VM selection/admission, and warm-up excluded |
| Statistic | Median nanoseconds per operation (ns/op) |

The benchmark covers the full in-VM loop, including loop-counter, branch, assignment, interpreter dispatch, arithmetic, and checksum work. It is not isolated hardware instruction latency. The double result is emitted as a scaled integer at 1e-6 resolution to avoid the runtime's lossy default floating-point text formatting. The checker validates the scaled checksums exactly; integer cases remain exact.

### 5.2 Median timing matrix — ns/op

| SLeeLa target | int + | int - | int * | int / | double + | double - | double * | double / |
|---|---:|---:|---:|---:|---:|---:|---:|---:|
| Basic Native | 72.107 | 72.178 | 72.112 | 72.260 | 80.552 | 79.124 | 79.768 | 82.920 |
| Premium SLVM/1 | 72.537 | 72.321 | 72.519 | 72.485 | 80.881 | 79.682 | 80.465 | 83.057 |
| Premium SLVM/2 | 72.542 | 72.277 | 72.241 | 72.410 | 80.656 | 79.702 | 79.910 | 83.378 |
| Premium SLVM/3 | 72.566 | 72.518 | 72.520 | 72.323 | 80.486 | 79.809 | 79.854 | 83.057 |
| Premium SLVM/4 | 72.571 | 72.498 | 72.218 | 72.297 | 80.579 | 79.710 | 79.971 | 82.897 |
| Premium SLVM/5 | 73.074 | 72.515 | 72.187 | 72.255 | 80.518 | 79.746 | 79.889 | 82.993 |
| Premium SLVM/6 | 72.807 | 72.650 | 72.436 | 72.318 | 80.738 | 79.554 | 79.916 | 83.165 |
| Premium SLVM/7 | 72.718 | 72.185 | 72.607 | 72.309 | 80.954 | 79.656 | 79.950 | 83.103 |
| Premium SLVM/8 | 75.779 | 75.111 | 75.329 | 75.530 | 82.888 | 81.717 | 82.721 | 84.797 |
| Premium SLVM/9 | 72.805 | 72.520 | 72.297 | 72.237 | 80.581 | 79.734 | 79.749 | 83.080 |
| Premium SLVM/10 | 72.576 | 72.340 | 72.238 | 72.332 | 80.506 | 79.608 | 80.119 | 83.183 |
| Premium SLVM/11 | 81.842 | 81.706 | 81.853 | 82.468 | 87.858 | 87.165 | 87.606 | 91.017 |

### 5.3 Arithmetic correctness and VM execution qualification

- Basic Native: 8/8 arithmetic cases passed; all five checksums per case matched.
- Premium SLVM/1–11: 88/88 arithmetic cases passed; all five checksums per case matched.
- Aggregate: 96/96 target-operation cases and 480/480 sample checksum validations passed.
- The VM matrix artifact also records all 12 runnable targets passing the shared execution smoke test (outputs `42`, `15`, `hello`).
- All 11 Premium VM admission self-checks passed.
- Standard VM generations 001–010 remain registered but not independently bootable; they are excluded from runnable timing columns.

### 5.4 Comparison limitations

The native C11/C++17/Java benchmark in sections 2–4 is a different harness with different operation counts and typed workloads. The SLeeLa VM numbers above should not be used for a direct speed ranking against those native-language numbers until a matched workload, host, compiler configuration, and timing boundary are established. The VM figures are useful as an internal baseline across the twelve runnable SLeeLa targets.

The SLeeLa workload currently measures only the language-level `int` and `double` types. It does not establish performance for every fixed-width integer type, float32, Boolean logic, RAM bandwidth/latency, allocation, or garbage collection.

## 6. Memory and RAM performance

The arithmetic runs do not measure RAM latency or bandwidth. A separate suite is required for sequential read/write bandwidth, random-access latency, cache-resident versus larger-than-cache working sets, strides, allocation, and garbage collection. The host's visible RAM figure is metadata, not a memory-speed result.

## 7. Boolean and logical-operation performance

Boolean arithmetic is not applicable. A separate suite should measure logical AND, OR, NOT, equality/inequality, and short-circuit behavior, with truth-table and side-effect validation before timings are accepted.

## 8. Reproduction and source artifacts

Native-language reference reproduction:

```sh
python3 comparison/tests/run_arithmetic_speed.py
```

SLeeLa VM reproduction after building the Basic Native and Premium VMs:

```sh
python3 comparison/tests/run_sleela_vm_arithmetic_speed.py
```

The VM runner writes `comparison-sleela-vm-arithmetic-results.json`; hosted CI retains it in the `sleela-complete-vm-matrix` artifact linked above. The raw five-sample arrays, checksums, target metadata, and host information are preserved in that JSON.

**Conclusion:** native C11, C++17, and Java 21+ retain their separately recorded local and hosted arithmetic datasets. SLeeLa Basic Native and Premium SLVM/1–11 now have validated internal arithmetic timings for the eight tested int/double operator cases. No cross-language speed ranking or RAM-performance claim is made.


## Latest cross-platform CI status — 2026-11-07

The corrective reruns are still in progress at the time of this update:

- [Complete VM and Language Conformance — run 42343381660](https://github.com/mearvk/SLeeLa/actions/runs/42343381660):

Basic Native VM + Premium SLVM 1-11: in_progress
Arithmetic speed — C11, C++17, Java 21+: completed (success)
C11, C++17, Java 21+ (macos-latest): completed (success)
C11, C++17, Java 21+ (ubuntu-latest): completed (success)
C11, C++17, Java 21+ (windows-latest): in_progress

- [Native C and C++ Portability — run 42343381645](https://github.com/mearvk/SLeeLa/actions/runs/42343381645):

Native runtime (ubuntu-latest): in_progress
Native runtime (macos-latest): in_progress
Native runtime (Windows 10+ / MinGW-w64): in_progress

These jobs have not yet produced final cross-platform qualification results. The previously validated SLeeLa arithmetic benchmark remains PASS (12/12 runnable targets; 96/96 target-operation cases; 480/480 sample checksum validations). The separate C11/C++17/Java 21+ hosted comparator result remains PASS (128/128 cases). The cross-platform reruns must not be conflated with those completed arithmetic measurements.


## Final corrective CI results — 2026-11-07

The two corrective workflows have now completed.

- [Complete VM and Language Conformance — run 42343381660](https://github.com/mearvk/SLeeLa/actions/runs/42343381660): **PASS, 5/5 jobs**. Basic Native + Premium SLVM 1–11 passed; the arithmetic-speed job passed; C11/C++17/Java 21+ comparator jobs passed on Ubuntu, macOS, and Windows.
- [Native C and C++ Portability — run 42343381645](https://github.com/mearvk/SLeeLa/actions/runs/42343381645): **FAIL, 0/3 platform jobs fully passed**. Linux and macOS built the native runtime, passed ISA verification, OSLS registry tests, and OSLS VM compiler/dispatch tests, then stopped in the test-math Makefile recipe with an unterminated shell quote / unexpected EOF. Windows built the native runtime and passed ISA verification, but OSLS CTest failed one osls_test assertion: the relative normalized path was compared against POSIX ../c instead of the Windows separator form ..\\c.

These failures are test-harness portability issues observed after the earlier native build and several OSLS checks succeeded. They do not overturn the successful VM/language conformance workflow or the separately validated arithmetic measurements. Native portability is NOT qualified on any of the three operating systems until the shell recipe and Windows path expectation are corrected and the workflow is rerun.
