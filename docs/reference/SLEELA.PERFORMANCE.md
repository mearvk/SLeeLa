<img align="right" src="../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# SLeeLa Performance Report

**Report edition:** 001  
**Repository:** `mearvk/SLeeLa`  
**Baseline:** SLeeLa Syntax 3.0  
**Last consolidated:** 2026-11-07  
**Status:** Arithmetic datasets recorded and checksum-validated; VM/language conformance passed on all three operating systems; native portability remains blocked by regression-test harness failures (2026-11-07)

## 1. Executive summary

This report consolidates the arithmetic performance measurements currently recorded for native C11, native C++17, and Java 21+, and tracks the corresponding SLeeLa VM qualification work.

The report now contains two separately identified native arithmetic datasets: the earlier local Linux reference run and a hosted GitHub Actions Linux run. Neither should be treated as a universal language ranking or release-grade measurement.

The corrected integrated workflow measured all twelve runnable SLeeLa VM targets across integer and double addition, subtraction, multiplication, and division. All 96 target-operation cases and all 480 sample checksum validations passed. The overall VM workflow later failed only because the checked-in Standard VM Modium manifests were stale; that later registry freshness check does not invalidate the completed arithmetic artifact.

## 2. Current qualification status

| Environment | Arithmetic data | Execution evidence | Status |
|---|---|---|---|
| Native C11 | int widths, float32, float64; signed 128-bit extension | Local reference + hosted CI run 42340090666 | Recorded; both suites passed |
| Native C++17 | int widths, float32, float64; signed 128-bit extension | Local reference + hosted CI run 42340090666 | Recorded; both suites passed |
| Java 21+ | integer-width emulations, float32, float64; no primitive 128-bit | Local reference + hosted CI run 42340090666 | Recorded; both suites passed |
| SLeeLa Basic Native VM | `int`, `double`; +, -, *, / | Workflow run 42342660507 | **PASS** — 8 cases × 5 samples; checksums validated |
| SLeeLa Premium SLVM 1–11 | `int`, `double`; +, -, *, / | Workflow run 42342660507 | **PASS** — 11/11 targets; 88 cases × 5 samples; checksums validated |

**Workflow checked:** [Complete SLeeLa VM and Language Conformance — run 42340090666](https://github.com/mearvk/SLeeLa/actions/runs/42340090666). The run completed with 4 of 5 jobs successful. The arithmetic speed job passed; the VM job failed after the twelve targets were runnable and the Premium self-checks passed, when the new benchmark runner attempted to read an undefined `checks` variable. That defect is fixed in the current source. The VM timing results from this first attempt remain unavailable.



## Latest cross-platform CI qualification — 2026-11-07

The corrective workflows are still running at the time of this report update. Their current state is recorded as a snapshot, not a final pass/fail determination:

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

The previously validated arithmetic dataset remains: SLeeLa Basic Native plus Premium SLVM 1–11, 96/96 target-operation cases and 480/480 sample checksum validations; C11/C++17/Java 21+ comparator suite, 128/128 cases on the recorded hosted Linux run. The in-progress reruns are intended to close the separate Modium-manifest and native-portability gaps. Do not mark cross-platform VM conformance or native portability complete until these runs finish successfully. This status snapshot should be replaced with final job conclusions after completion.



## Final corrective CI results — 2026-11-07

The two corrective workflows have now completed.

- [Complete VM and Language Conformance — run 42343381660](https://github.com/mearvk/SLeeLa/actions/runs/42343381660): **PASS, 5/5 jobs**. Basic Native + Premium SLVM 1–11 passed; the arithmetic-speed job passed; C11/C++17/Java 21+ comparator jobs passed on Ubuntu, macOS, and Windows.
- [Native C and C++ Portability — run 42343381645](https://github.com/mearvk/SLeeLa/actions/runs/42343381645): **FAIL, 0/3 platform jobs fully passed**. Linux and macOS built the native runtime, passed ISA verification, OSLS registry tests, and OSLS VM compiler/dispatch tests, then stopped in the test-math Makefile recipe with an unterminated shell quote / unexpected EOF. Windows built the native runtime and passed ISA verification, but OSLS CTest failed one osls_test assertion: the relative normalized path was compared against POSIX ../c instead of the Windows separator form ..\\c.

These failures are test-harness portability issues observed after the earlier native build and several OSLS checks succeeded. They do not overturn the successful VM/language conformance workflow or the separately validated arithmetic measurements. Native portability is NOT qualified on any of the three operating systems until the shell recipe and Windows path expectation are corrected and the workflow is rerun.

## 3. Native C11 / C++17 / Java 21+ arithmetic results

The following values are the median nanoseconds per operation (ns/op) from Arithmetic Speed Series — Iteration 001. Lower values indicate lower observed time for this particular benchmark path.

| Type | Operator | C11 | C++17 | Java 21+ |
|---|---:|---:|---:|---:|
| i8 | + | 0.575 | 0.603 | 0.824 |
| i8 | - | 1.128 | 1.173 | 1.382 |
| i8 | * | 1.241 | 1.276 | 5.483 |
| i8 | / | 1.704 | 1.732 | 7.171 |
| u8 | + | 0.593 | 0.570 | 1.075 |
| u8 | - | 1.220 | 0.835 | 1.164 |
| u8 | * | 1.281 | 0.979 | 1.798 |
| u8 | / | 1.116 | 1.143 | 2.476 |
| i16 | + | 0.567 | 0.603 | 1.259 |
| i16 | - | 1.121 | 1.164 | 1.364 |
| i16 | * | 1.264 | 1.191 | 1.818 |
| i16 | / | 1.768 | 1.729 | 2.251 |
| u16 | + | 0.662 | 0.645 | 1.102 |
| u16 | - | 1.175 | 1.138 | 1.121 |
| u16 | * | 1.162 | 1.239 | 1.026 |
| u16 | / | 1.160 | 1.154 | 1.985 |
| i32 | + | 0.663 | 0.679 | 1.158 |
| i32 | - | 1.230 | 0.987 | 1.510 |
| i32 | * | 1.262 | 1.013 | 1.218 |
| i32 | / | 1.754 | 1.693 | 2.017 |
| u32 | + | 0.707 | 0.736 | 0.602 |
| u32 | - | 1.244 | 1.191 | 1.230 |
| u32 | * | 1.421 | 1.317 | 0.966 |
| u32 | / | 1.710 | 1.827 | 2.001 |
| i64 | + | 0.587 | 0.687 | 0.708 |
| i64 | - | 1.180 | 1.254 | 1.110 |
| i64 | * | 1.417 | 1.426 | 0.992 |
| i64 | / | 2.087 | 2.223 | 2.010 |
| u64 | + | 0.577 | 0.698 | 0.573 |
| u64 | - | 1.209 | 1.170 | 0.728 |
| u64 | * | 1.327 | 1.317 | 0.827 |
| u64 | / | 1.966 | 2.043 | 1.933 |
| i128 | + | 0.610 | 0.641 | N/A |
| i128 | - | 0.862 | 0.798 | N/A |
| i128 | * | 1.020 | 0.932 | N/A |
| i128 | / | 2.431 | 3.691 | N/A |
| f32 | + | 1.243 | 1.142 | 0.813 |
| f32 | - | 1.261 | 1.256 | 0.839 |
| f32 | * | 1.293 | 1.236 | 0.867 |
| f32 | / | 1.019 | 0.967 | 0.911 |
| f64 | + | 1.128 | 1.131 | 0.782 |
| f64 | - | 1.250 | 1.223 | 0.848 |
| f64 | * | 1.224 | 1.291 | 0.842 |
| f64 | / | 1.356 | 1.410 | 1.318 |

### 3.1 Native reference host and method

| Property | Recorded configuration |
|---|---|
| OS | Linux 6.18.57 x86_64, glibc 2.41 |
| CPU | AMD EPYC 9V74; container allocation of 3 logical CPUs |
| RAM visible to container | 6,091,004 kB |
| C compiler | GCC 14.2.0 |
| C++ compiler | G++ 14.2.0 |
| Java | OpenJDK 21.0.12.1 |
| Native optimization | `-O2` |
| Operations per case | 2,000,000 |
| Operand set | 4,096 deterministic elements; seed `0x53435350` |
| Repetitions | 5 measured samples per type/operator |
| Warm-up | One untimed native process; three Java warm-up sweeps inside each measured JVM |
| Timing source | Monotonic high-resolution clock |
| Reported statistic | Median ns/op |

The native benchmark exercises indexed operands and checksum folding. It measures the full loop/index/operand/arithmetic/checksum path, not an isolated processor instruction.

### 3.2 Type and language qualifications

- C11 and C++17 include signed and unsigned 8-, 16-, 32-, and 64-bit integer cases, plus signed 128-bit cases where the compiler extension is available.
- Java's unsigned 8-, 16-, and 32-bit cases use masked integer emulation; unsigned 64-bit division uses Java's unsigned helper. These are not Java unsigned primitive types.
- Java has no primitive signed 128-bit integer; no `BigInteger` replacement was included.
- The operands avoid integer overflow and use nonzero divisors.
- Boolean arithmetic is not applicable. Boolean performance belongs in a separate logical-operation suite.



### 3.2 Hosted GitHub Actions Linux arithmetic results

**Execution:** [Complete SLeeLa VM and Language Conformance — run 42340090666](https://github.com/mearvk/SLeeLa/actions/runs/42340090666), arithmetic-speed job.  
**Artifact:** `comparison-arithmetic-speed-results.json`, artifact ID `13561852004`.  
**Result:** PASS — 128/128 measurements/checksum cases (C11 44/44, C++17 44/44, Java 21+ 40/40). Each case used five measured samples; the values below are the medians in ns/op.

Hosted runner metadata: Linux 7.0.0-1019-azure, x86_64, glibc 2.43, 4 logical cores. The CPU model was not exposed by the runner metadata. These hosted values are a separate dataset from the local reference values in section 3 and must not be merged or averaged with them.

| Type | Operator | C11 | C++17 | Java 21+ |
|---|---:|---:|---:|---:|
| f32 | * | 1.374 | 1.659 | 4.881 |
| f32 | + | 1.152 | 1.158 | 1.245 |
| f32 | - | 1.248 | 1.407 | 1.288 |
| f32 | / | 1.520 | 1.517 | 3.321 |
| f64 | * | 1.441 | 1.153 | 1.006 |
| f64 | + | 1.439 | 1.153 | 1.018 |
| f64 | - | 1.441 | 1.151 | 1.107 |
| f64 | / | 1.387 | 1.287 | 1.469 |
| i128 | * | 1.250 | 1.247 | N/A |
| i128 | + | 0.920 | 0.914 | N/A |
| i128 | - | 1.151 | 0.952 | N/A |
| i128 | / | 3.178 | 3.463 | N/A |
| i16 | * | 0.970 | 1.151 | 1.822 |
| i16 | + | 0.671 | 0.678 | 1.068 |
| i16 | - | 0.874 | 1.155 | 1.781 |
| i16 | / | 1.728 | 1.729 | 3.191 |
| i32 | * | 1.151 | 1.081 | 1.133 |
| i32 | + | 0.675 | 0.671 | 0.734 |
| i32 | - | 1.151 | 0.895 | 1.117 |
| i32 | / | 1.730 | 1.730 | 2.887 |
| i64 | * | 1.152 | 1.153 | 0.968 |
| i64 | + | 0.641 | 0.776 | 0.615 |
| i64 | - | 0.866 | 1.151 | 1.100 |
| i64 | / | 2.878 | 2.883 | 2.887 |
| i8 | * | 0.994 | 1.161 | 6.466 |
| i8 | + | 0.696 | 0.675 | 0.891 |
| i8 | - | 0.908 | 1.160 | 1.344 |
| i8 | / | 1.742 | 1.739 | 13.812 |
| u16 | * | 1.053 | 1.153 | 1.536 |
| u16 | + | 0.673 | 0.673 | 1.063 |
| u16 | - | 1.057 | 1.153 | 1.344 |
| u16 | / | 2.016 | 2.017 | 2.950 |
| u32 | * | 0.947 | 1.152 | 0.819 |
| u32 | + | 0.671 | 0.752 | 0.619 |
| u32 | - | 0.864 | 1.151 | 0.621 |
| u32 | / | 1.728 | 1.726 | 2.888 |
| u64 | * | 1.151 | 1.113 | 0.852 |
| u64 | + | 0.644 | 0.688 | 0.617 |
| u64 | - | 1.182 | 0.901 | 0.630 |
| u64 | / | 2.882 | 2.879 | 2.883 |
| u8 | * | 1.156 | 1.085 | 1.518 |
| u8 | + | 0.674 | 0.673 | 1.769 |
| u8 | - | 1.160 | 0.872 | 0.979 |
| u8 | / | 1.731 | 1.733 | 2.981 |

The hosted runner and local reference benchmark are distinct executions with different hosts and timing environments. Compare only within each dataset unless a controlled matched-runner protocol is introduced.

## 4. SLeeLa VM arithmetic qualification

The SLeeLa-specific workload is defined in:

- [SLeeLa arithmetic source](../../comparison/tests/sleela/arithmetic_speed.sleela)
- [SLeeLa VM benchmark runner](../../comparison/tests/run_sleela_vm_arithmetic_speed.py)
- [SLeeLa VM arithmetic protocol](../../comparison/ARITHMETIC.SPEED.SLEELA.001.md)

The workload measures SLeeLa `int` and `double` for addition, subtraction, multiplication, and division on twelve targets: Basic Native and Premium SLVM 1 through 11.

### 4.1 Timing boundary

The benchmark uses the SLeeLa monotonic nanosecond timer around the measured loop. Process launch, CLI startup, compilation/build, VM selection, Premium admission, and per-operation warm-up are outside the measured interval. Each operation has a 50,000-iteration warm-up and a 250,000-iteration measured loop, with five executions per target.

The measured loop includes VM loop-counter, branch, assignment, and interpreter-dispatch overhead. It is an end-to-end in-VM workload, not isolated arithmetic instruction latency.

### 4.2 SLeeLa results matrix

The hosted CI measurements are now complete. See section 4.3 for the validated medians and the exact workflow artifact. All twelve targets passed all eight arithmetic cases.

### 4.3 Hosted VM arithmetic results — validated

**Workflow:** [Complete SLeeLa VM and Language Conformance — run 42342660507](https://github.com/mearvk/SLeeLa/actions/runs/42342660507)  
**Commit:** `ff75886e08bbff7f325d1ca84b0e86b930bda54c`  
**Artifact:** [sleela-complete-vm-matrix](https://github.com/mearvk/SLeeLa/actions/runs/42342660507/artifacts/13562815822) — contains `comparison-sleela-vm-arithmetic-results.json`.  
**Arithmetic status:** **PASS — 12/12 targets, 96/96 target-operation cases, 480/480 sample checksums.**

The hosted Linux runner reported Linux 7.0.0-1019-azure, x86_64, 4 logical cores. Each target ran eight cases (int and double × +, -, *, /), with 250,000 measured iterations per case, 50,000 warm-up iterations, and five samples. Values below are median nanoseconds per operation. The integer checksums were exact; double checksums were validated at a 1e-6 resolution after correcting lossy default double-to-text formatting. This is a distinct in-VM workload from the native-language matrix in section 3; do not treat the figures as directly comparable without matching harnesses and timing boundaries.

| Target | int + | int - | int * | int / | double + | double - | double * | double / |
|---|---:|---:|---:|---:|---:|---:|---:|---:|
| Basic Native | 72.107 | 72.178 | 72.112 | 72.260 | 80.552 | 79.124 | 79.768 | 82.920 |
| Premium SLVM 1 | 72.537 | 72.321 | 72.519 | 72.485 | 80.881 | 79.682 | 80.465 | 83.057 |
| Premium SLVM 2 | 72.542 | 72.277 | 72.241 | 72.410 | 80.656 | 79.702 | 79.910 | 83.378 |
| Premium SLVM 3 | 72.566 | 72.518 | 72.520 | 72.323 | 80.486 | 79.809 | 79.854 | 83.057 |
| Premium SLVM 4 | 72.571 | 72.498 | 72.218 | 72.297 | 80.579 | 79.710 | 79.971 | 82.897 |
| Premium SLVM 5 | 73.074 | 72.515 | 72.187 | 72.255 | 80.518 | 79.746 | 79.889 | 82.993 |
| Premium SLVM 6 | 72.807 | 72.650 | 72.436 | 72.318 | 80.738 | 79.554 | 79.916 | 83.165 |
| Premium SLVM 7 | 72.718 | 72.185 | 72.607 | 72.309 | 80.954 | 79.656 | 79.950 | 83.103 |
| Premium SLVM 8 | 75.779 | 75.111 | 75.329 | 75.530 | 82.888 | 81.717 | 82.721 | 84.797 |
| Premium SLVM 9 | 72.805 | 72.520 | 72.297 | 72.237 | 80.581 | 79.734 | 79.749 | 83.080 |
| Premium SLVM 10 | 72.576 | 72.340 | 72.238 | 72.332 | 80.506 | 79.608 | 80.119 | 83.183 |
| Premium SLVM 11 | 81.842 | 81.706 | 81.853 | 82.468 | 87.858 | 87.165 | 87.606 | 91.017 |

**Qualification note:** all twelve runnable targets completed every arithmetic case and checksum check. The overall VM workflow was nevertheless marked failed after the arithmetic suite passed, because the subsequent `tools/generate-modium.py --check` step found the checked-in `MODIUM.json` and `rebuild.sql` artifacts stale for Standard VM generations 001–010. That is a generated-registry freshness failure, not an arithmetic or VM execution failure. The arithmetic artifact is valid and retained above.


## 5. Memory and RAM performance

The arithmetic results above do **not** measure RAM latency or bandwidth. The host's visible RAM figure is environment metadata, not a memory-speed result.

A separate memory benchmark is required to report:
- Sequential read and write bandwidth
- Random-access latency
- Cache-resident versus working sets larger than last-level cache
- Multiple access strides and working-set sizes
- Allocation and deallocation cost, where relevant
- SLeeLa VM versus native and Java memory access under equivalent workloads

Memory results must identify the data size, access pattern, repetitions, warm-up, thread count, and timing boundary. They must not be mixed into arithmetic ns/op tables.

## 6. Boolean and logical-operation performance

Boolean values are excluded from arithmetic. A separate suite should measure logical AND, OR, NOT, equality/inequality, and short-circuit behavior. It should validate truth tables and side-effect behavior before accepting timing data.

## 7. Interpretation and limitations

1. The native-language measurements are a single local Linux-container reference run, not a controlled cross-platform benchmark.
2. Hosted CI measurements, when available, should be recorded separately with runner OS, CPU, compiler/JDK versions, and run identifiers.
3. The C/C++/Java and SLeeLa VM suites use different harness paths and operation counts. Direct ranking should wait until a deliberately matched workload and timing model are established.
4. This report does not claim a universal fastest language or VM.
5. No memory bandwidth, random RAM latency, cache sweep, allocation, or garbage-collection performance is included in the arithmetic data.
6. SLeeLa VM measurements are available for the eight tested int/double cases; other integer widths, float32, memory access, and Boolean logic remain unmeasured for SLeeLa.

## 8. Source reports and reproduction

- [Arithmetic Speed Series — Iteration 001 (C11/C++17/Java 21+)](../../comparison/ARITHMETIC.SPEED.001.md)
- [SLeeLa VM Arithmetic Speed — Iteration 001](../../comparison/ARITHMETIC.SPEED.SLEELA.001.md)
- [Performance Analysis — Iteration 001](../../comparison/PERFORMANCE.ANALYSIS.001.md)

Native-language reference reproduction:

```sh
python3 comparison/tests/run_arithmetic_speed.py
```

SLeeLa VM reproduction after the Basic Native and Premium VMs are built:

```sh
python3 comparison/tests/run_sleela_vm_arithmetic_speed.py
```

The SLeeLa runner writes `comparison-sleela-vm-arithmetic-results.json`; the integrated workflow retains it in the `sleela-complete-vm-matrix` artifact. The validated run is 42342660507 (artifact 13562815822).

---

**Current conclusion:** C11, C++17, and Java 21+ have local reference data and a completed hosted Linux arithmetic dataset (128/128 cases passed). SLeeLa Basic Native and Premium SLVM 1–11 now have validated arithmetic measurements (96/96 target-operation cases; 480/480 checksum samples passed). The overall VM workflow remains red because the later Standard VM Modium freshness check detected stale generated files. RAM performance, broader SLeeLa numeric widths, float32, and Boolean logic still require separate benchmark suites.

## 9. Common eight-case comparison — Iteration 002

A consolidated table now places the eight shared hosted-Linux cases (i64 and f64 × +, -, *, /) side by side for C11, C++17, Java 21+, Basic Native, and Premium SLVM 1–11: [Arithmetic Comparison — Common Eight Cases](../../comparison/ARITHMETIC.SPEED.002.md). This is a common-coverage view only. The source datasets still use different iteration counts, operand patterns, and timing boundaries, so cross-language and native-versus-VM timing ratios remain invalid. A genuinely equivalent comparison requires a new matched-protocol CI run.
