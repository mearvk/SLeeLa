# SLeeLa VM Arithmetic Speed — Iteration 001

**Suite:** SCS-PERF-SLEELA-VM-001  
**Status:** Benchmark implementation committed; execution pending GitHub Actions  
**Targets:** Basic Native SLeeLa and Premium SLVM 1–11  
**Types:** SLeeLa `int` and `double`  
**Operators:** Addition, subtraction, multiplication, division

## 1. Purpose

Measure arithmetic throughput inside the actual SLeeLa VM execution path. This suite complements, and does not replace, the C11/C++17/Java 21+ arithmetic reference suite in [ARITHMETIC.SPEED.001.md](ARITHMETIC.SPEED.001.md).

## 2. Timing boundary

The SLeeLa workload calls `timeMonotonicNanos()` immediately before and after each measured arithmetic loop. The following occur outside that timed interval:

- Process launch and CLI startup
- Compiler/build work
- VM selection and Premium admission
- Loading and entering the benchmark program
- The 50,000-iteration warm-up loop for each operation

Each measured loop executes 250,000 iterations. Five separate executions are recorded per target and operation. The report uses the median elapsed nanoseconds and median nanoseconds per operation.

The measured loop still includes the VM's loop counter, branch, assignment, and interpreter dispatch overhead. These results therefore represent an end-to-end in-VM arithmetic workload, not isolated hardware instruction latency.

## 3. Matrix

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

## 4. Correctness gates

Every target execution must emit all eight timing records and all eight result records. The runner checks the expected integer results and floating-point results (with a tight relative/absolute tolerance) before accepting the timings. A missing record, non-positive duration, failed process, or incorrect result fails the suite.

## 5. Boolean operations

Boolean values are intentionally excluded from arithmetic. A separate logical suite should measure logical AND, OR, NOT, equality/inequality, and short-circuit behavior, with truth-table and side-effect checks.

## 6. Reproduction

After the Basic Native compiler/runtime and all Premium VMs are built:

```sh
python3 comparison/tests/run_sleela_vm_arithmetic_speed.py
```

The runner writes `comparison-sleela-vm-arithmetic-results.json`. GitHub Actions retains that file in the `sleela-complete-vm-matrix` artifact.

## 7. Evidence status

The workload, runner, correctness checks, and workflow integration are committed to `master`. No performance numbers are asserted in this document until the corresponding GitHub Actions job actually executes and its artifact is inspected. The local C/C++/Java measurements are not substituted for missing SLeeLa VM measurements.
