# SLeeLa Performance Report

**Report edition:** 001  
**Repository:** `mearvk/SLeeLa`  
**Baseline:** SLeeLa Syntax 3.0  
**Last consolidated:** 2026-11-07  
**Status:** Native-language arithmetic reference recorded; SLeeLa VM arithmetic CI queued

## 1. Executive summary

This report consolidates the arithmetic performance measurements currently recorded for native C11, native C++17, and Java 21+, and tracks the corresponding SLeeLa VM qualification work.

The currently available numeric results are from one local Linux reference execution. They are **not** hosted GitHub Actions results and should not be treated as universal language rankings or release-grade measurements.

The SLeeLa VM arithmetic workload has been committed for the Basic Native VM and Premium SLVM 1–11, but its first integrated workflow run is still queued. Consequently, no SLeeLa VM timing is reported here yet.

## 2. Current qualification status

| Environment | Arithmetic data | Execution evidence | Status |
|---|---|---|---|
| Native C11 | int widths, float32, float64; signed 128-bit extension | Local reference run, five samples per case | Recorded |
| Native C++17 | int widths, float32, float64; signed 128-bit extension | Local reference run, five samples per case | Recorded |
| Java 21+ | integer-width emulations, float32, float64; no primitive 128-bit | Local reference run, five samples per case | Recorded |
| SLeeLa Basic Native VM | `int`, `double`; +, -, *, / | Workflow run 42340090666 | Pending — queued |
| SLeeLa Premium SLVM 1–11 | `int`, `double`; +, -, *, / | Workflow run 42340090666 | Pending — queued |

**Workflow checked:** [Complete SLeeLa VM and Language Conformance — run 42340090666](https://github.com/mearvk/SLeeLa/actions/runs/42340090666). At the time of this report, the overall run and all five jobs were `queued`; none had produced an execution result. The run is associated with commit `70c0f6c0234b1508cb858d481ca4a6c841333706`.

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

## 4. SLeeLa VM arithmetic qualification

The SLeeLa-specific workload is defined in:

- [SLeeLa arithmetic source](comparison/tests/sleela/arithmetic_speed.sleela)
- [SLeeLa VM benchmark runner](comparison/tests/run_sleela_vm_arithmetic_speed.py)
- [SLeeLa VM arithmetic protocol](comparison/ARITHMETIC.SPEED.SLEELA.001.md)

The workload measures SLeeLa `int` and `double` for addition, subtraction, multiplication, and division on twelve targets: Basic Native and Premium SLVM 1 through 11.

### 4.1 Timing boundary

The benchmark uses the SLeeLa monotonic nanosecond timer around the measured loop. Process launch, CLI startup, compilation/build, VM selection, Premium admission, and per-operation warm-up are outside the measured interval. Each operation has a 50,000-iteration warm-up and a 250,000-iteration measured loop, with five executions per target.

The measured loop includes VM loop-counter, branch, assignment, and interpreter-dispatch overhead. It is an end-to-end in-VM workload, not isolated arithmetic instruction latency.

### 4.2 SLeeLa results matrix

| Target | int + | int - | int * | int / | double + | double - | double * | double / |
|---|---:|---:|---:|---:|---:|---:|---:|---:|
| Basic Native | Pending | Pending | Pending | Pending | Pending | Pending | Pending | Pending |
| Premium SLVM 1 | Pending | Pending | Pending | Pending | Pending | Pending | Pending | Pending |
| Premium SLVM 2 | Pending | Pending | Pending | Pending | Pending | Pending | Pending | Pending |
| Premium SLVM 3 | Pending | Pending | Pending | Pending | Pending | Pending | Pending | Pending |
| Premium SLVM 4 | Pending | Pending | Pending | Pending | Pending | Pending | Pending | Pending |
| Premium SLVM 5 | Pending | Pending | Pending | Pending | Pending | Pending | Pending | Pending |
| Premium SLVM 6 | Pending | Pending | Pending | Pending | Pending | Pending | Pending | Pending |
| Premium SLVM 7 | Pending | Pending | Pending | Pending | Pending | Pending | Pending | Pending |
| Premium SLVM 8 | Pending | Pending | Pending | Pending | Pending | Pending | Pending | Pending |
| Premium SLVM 9 | Pending | Pending | Pending | Pending | Pending | Pending | Pending | Pending |
| Premium SLVM 10 | Pending | Pending | Pending | Pending | Pending | Pending | Pending | Pending |
| Premium SLVM 11 | Pending | Pending | Pending | Pending | Pending | Pending | Pending | Pending |

No SLeeLa timing values are inferred from C, C++, or Java results. The table must be populated only from the completed workflow artifact after validating all twelve targets and their arithmetic checksums.

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
6. SLeeLa VM numbers remain pending until the run artifact is available and inspected.

## 8. Source reports and reproduction

- [Arithmetic Speed Series — Iteration 001 (C11/C++17/Java 21+)](comparison/ARITHMETIC.SPEED.001.md)
- [SLeeLa VM Arithmetic Speed — Iteration 001](comparison/ARITHMETIC.SPEED.SLEELA.001.md)
- [Performance Analysis — Iteration 001](comparison/PERFORMANCE.ANALYSIS.001.md)

Native-language reference reproduction:

```sh
python3 comparison/tests/run_arithmetic_speed.py
```

SLeeLa VM reproduction after the Basic Native and Premium VMs are built:

```sh
python3 comparison/tests/run_sleela_vm_arithmetic_speed.py
```

The SLeeLa runner writes `comparison-sleela-vm-arithmetic-results.json`; the integrated workflow retains it in the `sleela-complete-vm-matrix` artifact.

---

**Current conclusion:** C11, C++17, and Java 21+ have recorded local arithmetic reference measurements. SLeeLa Basic Native and Premium SLVM 1–11 are instrumented but remain unmeasured because the integrated CI run is queued. RAM performance and Boolean logic performance require their own benchmark suites.
