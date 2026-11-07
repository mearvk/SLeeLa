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
| i64 | `+` | 0.587 | 0.687 | 0.708 | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED |
| i64 | `-` | 1.180 | 1.254 | 1.110 | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED |
| i64 | `*` | 1.417 | 1.426 | 0.992 | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED |
| i64 | `/` | 2.087 | 2.223 | 2.010 | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED |
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
| f64 | `+` | 1.128 | 1.131 | 0.782 | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED |
| f64 | `-` | 1.250 | 1.223 | 0.848 | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED |
| f64 | `*` | 1.224 | 1.291 | 0.842 | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED |
| f64 | `/` | 1.356 | 1.410 | 1.318 | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED | NOT MEASURED |

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

## 5. SLeeLa VM status and column interpretation

The twelve SLeeLa VM columns have been added to the result matrix so that every runnable VM tested is represented explicitly:

- **Basic Native VM** — the baseline SLeeLa runtime in `impl/core`.
- **Premium SLVM/1** through **Premium SLVM/11** — the eleven runnable Premium VM generations.

Every SLeeLa VM timing cell is marked **NOT MEASURED**. This is deliberate: the latest hosted arithmetic job did not pass its result-integrity gate, so no SLeeLa timing is accepted or published. The VM execution smoke matrix did pass for all twelve runnable targets, but that is correctness/conformance evidence, not an arithmetic timing result.

The SLeeLa benchmark attempted 250,000 iterations per case, used 50,000 warm-up iterations, collected five samples per target/operation, and used `timeMonotonicNanos()`. Process startup and Premium admission were outside the timed region. The run stopped at the first Basic Native VM check:

```text
Basic Native: DOUBLE_ADD
Actual checksum:   2000.0
Expected checksum: 1999.9991
Tolerance:         1.9999991e-09
```

Because the first target failed its checksum, the runner produced no accepted timing rows and did not proceed to publish measurements for the remaining VM targets. The mismatch must be resolved at the workload/result-definition or arithmetic-implementation level before timings are accepted; the tolerance must not be loosened merely to make the job green.

**Important column rule:** all values in the three native-language columns are measured median nanoseconds per operation from the reference run. All SLeeLa VM columns currently contain the status text **NOT MEASURED**, not zero, not a speed estimate, and not a claim that the VMs failed execution.

## 6. Reproduction

From the repository root:

```sh
python3 comparison/tests/run_arithmetic_speed.py
```

The runner writes `comparison-arithmetic-speed-results.json` with environment metadata, five timing samples, medians, and checksums. The GitHub Actions workflow retains this JSON as the `arithmetic-speed-linux` artifact. Hosted CI measurements are informational only; use a controlled, pinned host for release comparisons.

**Conclusion:** the arithmetic sweep passed locally with 40/40 common cross-language checksum cases matching. No universal fastest-language conclusion is drawn from this single host.


## 7. SLeeLa VM execution qualification — hosted CI

**Workflow:** [Complete VM and Language Conformance](https://github.com/mearvk/SLeeLa/actions/runs/42341824853)  
**VM job:** Basic Native VM + Premium SLVM 1–11  
**Artifact:** [sleela-complete-vm-matrix](https://github.com/mearvk/SLeeLa/actions/runs/42341824853/artifacts/13561938012)  
**Commit tested:** `a781abac5ff3c982b60febca890b5b8be87b8c8d`  
**Host:** Ubuntu 26.04.1, x86_64, 4 logical CPUs  
**Overall VM job:** **FAIL — arithmetic checksum gate**. The separate execution matrix and Premium admission checks passed.

### 7.1 Runnable VM results

| SLeeLa target | Build | Basic execution smoke test | Premium admission self-check | Arithmetic timing |
|---|---|---|---|---|
| Basic Native VM | PASS | **PASS** — `42`, `15`, `hello` | N/A — baseline | **NOT MEASURED** — first checksum gate failed |
| Premium SLVM/1 | PASS | **PASS** — `42`, `15`, `hello` | PASS | **NOT MEASURED** |
| Premium SLVM/2 | PASS | **PASS** — `42`, `15`, `hello` | PASS | **NOT MEASURED** |
| Premium SLVM/3 | PASS | **PASS** — `42`, `15`, `hello` | PASS | **NOT MEASURED** |
| Premium SLVM/4 | PASS | **PASS** — `42`, `15`, `hello` | PASS | **NOT MEASURED** |
| Premium SLVM/5 | PASS | **PASS** — `42`, `15`, `hello` | PASS | **NOT MEASURED** |
| Premium SLVM/6 | PASS | **PASS** — `42`, `15`, `hello` | PASS | **NOT MEASURED** |
| Premium SLVM/7 | PASS | **PASS** — `42`, `15`, `hello` | PASS | **NOT MEASURED** |
| Premium SLVM/8 | PASS | **PASS** — `42`, `15`, `hello` | PASS | **NOT MEASURED** |
| Premium SLVM/9 | PASS | **PASS** — `42`, `15`, `hello` | PASS | **NOT MEASURED** |
| Premium SLVM/10 | PASS | **PASS** — `42`, `15`, `hello` | PASS | **NOT MEASURED** |
| Premium SLVM/11 | PASS | **PASS** — `42`, `15`, `hello` | PASS | **NOT MEASURED** |

**Aggregate:** 12/12 runnable targets passed the execution smoke test; 11/11 Premium admission self-checks passed; 0/12 SLeeLa targets have accepted arithmetic timing results from this run.

### 7.2 Standard VM registry entries — excluded from executable timing columns

The ten Standard/think-tank entries were inventoried, but are not independently bootable through the documented runner. They are therefore not included as runnable VM timing columns and must not be confused with the Premium SLVM generations.

| Standard generation | Registry identity | Execution status | Arithmetic timing |
|---:|---|---|---|
| 1 | `idea-vm-001` | REGISTERED — NOT BOOTABLE | NOT RUN |
| 2 | `idea-vm-002` | REGISTERED — NOT BOOTABLE | NOT RUN |
| 3 | `idea-vm-003` | REGISTERED — NOT BOOTABLE | NOT RUN |
| 4 | `idea-vm-004` | REGISTERED — NOT BOOTABLE | NOT RUN |
| 5 | `idea-vm-005` | REGISTERED — NOT BOOTABLE | NOT RUN |
| 6 | `idea-vm-006` | REGISTERED — NOT BOOTABLE | NOT RUN |
| 7 | `idea-vm-007` | REGISTERED — NOT BOOTABLE | NOT RUN |
| 8 | `idea-vm-008` | REGISTERED — NOT BOOTABLE | NOT RUN |
| 9 | `idea-vm-009` | REGISTERED — NOT BOOTABLE | NOT RUN |
| 10 | `idea-vm-010` | REGISTERED — NOT BOOTABLE | NOT RUN |

### 7.3 Interpretation and next qualification gate

The successful smoke tests show that the Basic Native VM and Premium SLVM/1–11 built and executed the shared program correctly. They do not establish arithmetic throughput. The benchmark artifact records `status: FAIL`, an empty `targets` object, and the checksum error above; there are no hidden or partial timing rows to transfer into the performance matrix.

Before filling any SLeeLa `ns/op` cell:

1. Correct the DOUBLE_ADD expected checksum or the runtime arithmetic behavior based on an independent mathematical check.
2. Rerun the complete SLeeLa arithmetic suite.
3. Require all eight operation/type records per target to validate before accepting any target's timing rows.
4. Publish the median `ns/op` values only from the validated JSON artifact, preserving the five raw samples and host metadata.

No SLeeLa-versus-C/C++/Java speed ranking is established by the current run.
