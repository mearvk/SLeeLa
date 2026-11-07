# Arithmetic Comparison — Common Eight Cases

**Suite:** SCS-PERF-ARITH-COMMON-002  
**Recorded:** 2026-11-07  
**Scope:** The eight shared cases present in both hosted Linux datasets: signed 64-bit integer and double, each with +, -, *, /.

## 1. Consolidated results

Median nanoseconds per operation (ns/op). Lower is faster within a given protocol.

| Case | C11 | C++17 | Java 21+ | Basic Native | SLVM 1 | SLVM 2 | SLVM 3 | SLVM 4 | SLVM 5 | SLVM 6 | SLVM 7 | SLVM 8 | SLVM 9 | SLVM 10 | SLVM 11 |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| i64 + | 0.641 | 0.776 | 0.615 | 72.107 | 72.537 | 72.542 | 72.566 | 72.571 | 73.074 | 72.807 | 72.718 | 75.779 | 72.805 | 72.576 | 81.842 |
| i64 - | 0.866 | 1.151 | 1.100 | 72.178 | 72.321 | 72.277 | 72.518 | 72.498 | 72.515 | 72.650 | 72.185 | 75.111 | 72.520 | 72.340 | 81.706 |
| i64 * | 1.152 | 1.153 | 0.968 | 72.112 | 72.519 | 72.241 | 72.520 | 72.218 | 72.187 | 72.436 | 72.607 | 75.329 | 72.297 | 72.238 | 81.853 |
| i64 / | 2.878 | 2.883 | 2.887 | 72.260 | 72.485 | 72.410 | 72.323 | 72.297 | 72.255 | 72.318 | 72.309 | 75.530 | 72.237 | 72.332 | 82.468 |
| f64 + | 1.439 | 1.153 | 1.018 | 80.552 | 80.881 | 80.656 | 80.486 | 80.579 | 80.518 | 80.738 | 80.954 | 82.888 | 80.581 | 80.506 | 87.858 |
| f64 - | 1.441 | 1.151 | 1.107 | 79.124 | 79.682 | 79.702 | 79.809 | 79.710 | 79.746 | 79.554 | 79.656 | 81.717 | 79.734 | 79.608 | 87.165 |
| f64 * | 1.441 | 1.153 | 1.006 | 79.768 | 80.465 | 79.910 | 79.854 | 79.971 | 79.889 | 79.916 | 79.950 | 82.721 | 79.749 | 80.119 | 87.606 |
| f64 / | 1.387 | 1.287 | 1.469 | 82.920 | 83.057 | 83.378 | 83.057 | 82.897 | 82.993 | 83.165 | 83.103 | 84.797 | 83.080 | 83.183 | 91.017 |

## 2. Comparability qualification

This is a **common-coverage consolidation**, not yet a fully equivalent timing comparison. It aligns the semantic cases, but the existing source measurements use different harnesses and timing boundaries:

- C11, C++17, and Java 21+ use 2,000,000 operations per case, indexed deterministic operand arrays, and a loop/index/operand/arithmetic/checksum path. Five measured samples; Java uses three warm-up sweeps.
- SLeeLa Basic Native and Premium SLVM 1–11 use 250,000 measured iterations per case, 50,000 warm-up iterations, and five samples. Their timed source loop uses constant operands and includes VM loop, branch, assignment, and dispatch overhead.
- Both datasets report hosted Linux 7.0.0-1019-azure, x86_64, four logical cores, but are separate workflow executions without controlled CPU model or host load.

**Do not calculate cross-language or native-versus-VM speed ratios from this table.** Compare C11/C++17/Java within their own dataset, and compare Basic Native/SLVM generations within theirs. Matching labels do not make the ns/op measurements interchangeable.

## 3. Validation and provenance

- Native comparator: [workflow 42340090666](https://github.com/mearvk/SLeeLa/actions/runs/42340090666), artifact `comparison-arithmetic-speed-results.json` (ID `13561852004`); 128/128 measurement/checksum cases passed.
- SLeeLa VM: [workflow 42342660507](https://github.com/mearvk/SLeeLa/actions/runs/42342660507), artifact [`sleela-complete-vm-matrix`](https://github.com/mearvk/SLeeLa/actions/runs/42342660507/artifacts/13562815822); 12/12 targets, 96/96 cases, 480/480 sample checksums passed.
- Main report: [`SLEELA.PERFORMANCE.md`](../SLEELA.PERFORMANCE.md).

## 4. Next genuinely equivalent iteration

A true matched comparison must rerun C11, C++17, Java 21+, Basic Native, and Premium SLVM 1–11 under one protocol: identical deterministic operand arrays, signed 64-bit and double operations, equal measured operation counts, equivalent warm-up, common checksum validation, aligned timing boundaries, medians, and sample spread. Raw samples should be retained. This report deliberately does not claim timing equivalence until that run exists.
