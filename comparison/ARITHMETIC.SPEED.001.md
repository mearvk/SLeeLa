# Arithmetic Speed Series — Iteration 001

**Suite:** SCS-PERF-ARITH-001  
**Run status:** PASS — local reference execution  
**Recorded:** 2026-11-07  
**Scope:** C11, C++17, Java 21+; four arithmetic operators across fixed-width integer and floating-point types

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

| Type | Operator | C11 | C++17 | Java 21+ |
|---|---:|---:|---:|---:|
| i8 | `+` | 0.575 | 0.603 | 0.824 |
| i8 | `-` | 1.128 | 1.173 | 1.382 |
| i8 | `*` | 1.241 | 1.276 | 5.483 |
| i8 | `/` | 1.704 | 1.732 | 7.171 |
| u8 | `+` | 0.593 | 0.570 | 1.075 |
| u8 | `-` | 1.220 | 0.835 | 1.164 |
| u8 | `*` | 1.281 | 0.979 | 1.798 |
| u8 | `/` | 1.116 | 1.143 | 2.476 |
| i16 | `+` | 0.567 | 0.603 | 1.259 |
| i16 | `-` | 1.121 | 1.164 | 1.364 |
| i16 | `*` | 1.264 | 1.191 | 1.818 |
| i16 | `/` | 1.768 | 1.729 | 2.251 |
| u16 | `+` | 0.662 | 0.645 | 1.102 |
| u16 | `-` | 1.175 | 1.138 | 1.121 |
| u16 | `*` | 1.162 | 1.239 | 1.026 |
| u16 | `/` | 1.160 | 1.154 | 1.985 |
| i32 | `+` | 0.663 | 0.679 | 1.158 |
| i32 | `-` | 1.230 | 0.987 | 1.510 |
| i32 | `*` | 1.262 | 1.013 | 1.218 |
| i32 | `/` | 1.754 | 1.693 | 2.017 |
| u32 | `+` | 0.707 | 0.736 | 0.602 |
| u32 | `-` | 1.244 | 1.191 | 1.230 |
| u32 | `*` | 1.421 | 1.317 | 0.966 |
| u32 | `/` | 1.710 | 1.827 | 2.001 |
| i64 | `+` | 0.587 | 0.687 | 0.708 |
| i64 | `-` | 1.180 | 1.254 | 1.110 |
| i64 | `*` | 1.417 | 1.426 | 0.992 |
| i64 | `/` | 2.087 | 2.223 | 2.010 |
| u64 | `+` | 0.577 | 0.698 | 0.573 |
| u64 | `-` | 1.209 | 1.170 | 0.728 |
| u64 | `*` | 1.327 | 1.317 | 0.827 |
| u64 | `/` | 1.966 | 2.043 | 1.933 |
| i128 | `+` | 0.610 | 0.641 | N/A |
| i128 | `-` | 0.862 | 0.798 | N/A |
| i128 | `*` | 1.020 | 0.932 | N/A |
| i128 | `/` | 2.431 | 3.691 | N/A |
| f32 | `+` | 1.243 | 1.142 | 0.813 |
| f32 | `-` | 1.261 | 1.256 | 0.839 |
| f32 | `*` | 1.293 | 1.236 | 0.867 |
| f32 | `/` | 1.019 | 0.967 | 0.911 |
| f64 | `+` | 1.128 | 1.131 | 0.782 |
| f64 | `-` | 1.250 | 1.223 | 0.848 |
| f64 | `*` | 1.224 | 1.291 | 0.842 |
| f64 | `/` | 1.356 | 1.410 | 1.318 |

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

## 5. SLeeLa VM status

SLeeLa VM timing is not included in this table. The core value representation exposes `int` (64-bit), `double`, and `boolean`; the type-width family and 128-bit paths need separate runtime qualification. The next SLeeLa-specific stage must run an equivalent int64/double workload inside the Basic Native VM and Premium SLVM 1–11, using the VM's monotonic clock around the timed loop. Process launch, compilation, and Premium admission must remain outside the steady-state timing boundary.

## 6. Reproduction

From the repository root:

```sh
python3 comparison/tests/run_arithmetic_speed.py
```

The runner writes `comparison-arithmetic-speed-results.json` with environment metadata, five timing samples, medians, and checksums. The GitHub Actions workflow retains this JSON as the `arithmetic-speed-linux` artifact. Hosted CI measurements are informational only; use a controlled, pinned host for release comparisons.

**Conclusion:** the arithmetic sweep passed locally with 40/40 common cross-language checksum cases matching. No universal fastest-language conclusion is drawn from this single host.
