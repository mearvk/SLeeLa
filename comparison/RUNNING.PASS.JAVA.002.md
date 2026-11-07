# Running PASS Record — Java/JVM, Native C/C++, and SLeeLa (Iteration 002)

**Record ID:** RUNNING.PASS.JAVA.002  
**Edition:** 1.0  
**Recorded:** 2026-11-07 (UTC, GitHub Actions timestamps)  
**Repository:** mearvk/SLeeLa · **Branch:** master · **SLeeLa baseline:** Syntax 3.0  
**Record status:** ITERATIVE — baseline PASS; numeric/Unicode edge batch pending CI verification

## 1. Executive result

Iteration 002 adds seven deterministic numeric and Unicode edge vectors to the C11, C++17, and Java comparison suite. The source files, runner, specification, and workflow integration are committed to `master`.

The available evidence supports two separate conclusions:

1. **Existing baseline:** PASS. Run [42332289318](https://github.com/mearvk/SLeeLa/actions/runs/42332289318) completed all four jobs successfully on Ubuntu, macOS, Windows, and the SLeeLa Syntax 3.0 VM job.
2. **New edge-vector batch:** PENDING. Run [42332366905](https://github.com/mearvk/SLeeLa/actions/runs/42332366905), which is associated with the final documentation commit and the workflow containing the new edge runner, was still queued at the latest recorded check. Its four jobs had not started.

Important evidence distinction: run 42332289318 was triggered from commit `e201d78`. The workflow file at that commit did **not** yet invoke `run_edge_vectors.py`. Therefore its successful result verifies the prior shared baseline, not the seven new edge vectors. The edge suite must not be marked PASS until a completed run executes the updated workflow.

## 2. Iteration 002 scope

The new normative specification is [NUMERIC-UNICODE-EDGE-VECTORS.md](NUMERIC-UNICODE-EDGE-VECTORS.md), Standard SCS-TEST-002, Edition 1.0.

| Vector ID | Input / operation | Expected result |
|---|---|---:|
| INT32-MIN | Minimum signed 32-bit value | `-2147483648` |
| INT32-MAX | Maximum signed 32-bit value | `2147483647` |
| UINT32-WRAP | `UINT32_MAX + 1`, modulo 2^32 | `0` |
| SIGNED-DIV | `-7 / 3`, truncating toward zero | `-2` |
| SIGNED-REM | `-7 % 3` | `-1` |
| UTF8-BYTES | UTF-8 byte length of `Aé😀` | `7` |
| UNICODE-CODEPOINTS | Unicode code-point count of `Aé😀` | `3` |

These observations are intentionally distinct: UTF-8 byte length, UTF-16 code-unit count, Unicode code-point count, and grapheme-cluster count are not interchangeable.

## 3. Implementations and runner

The committed suite contains:

- `comparison/tests/native/edge_vectors.c` — C11 implementation using exact-width integer types.
- `comparison/tests/native/edge_vectors.cpp` — C++17 implementation using exact-width integer types.
- `comparison/tests/java/EdgeVectors.java` — Java implementation using integer limits, UTF-8 encoding, and `String.codePointCount`.
- `comparison/tests/run_edge_vectors.py` — compiles and runs the three implementations, compares all seven output lines, and writes `comparison-edge-results.json` with platform and toolchain metadata.
- `comparison/NUMERIC-UNICODE-EDGE-VECTORS.md` — normative vector definitions and qualification boundaries.

The runner enables `-Wall -Wextra -Werror` for C and C++, uses C11 and C++17 modes, and compiles Java with UTF-8 source encoding. The C/C++ UTF-8 helpers are scoped to the known-valid fixture; they are not general-purpose malformed UTF-8 validators.

## 4. CI evidence ledger

### 4.1 Completed baseline run

**Run:** [42332289318](https://github.com/mearvk/SLeeLa/actions/runs/42332289318)  
**Conclusion:** success  
**Commit:** `e201d78ffc9aedd7aa291fbcb5e304118a6e60cd`

| Job | Conclusion |
|---|---|
| Shared vectors — Ubuntu latest | **PASS** |
| Shared vectors — macOS latest | **PASS** |
| Shared vectors — Windows latest | **PASS** |
| SLeeLa Syntax 3.0 VM vectors — Ubuntu | **PASS** |

The four jobs completed successfully. The VM job installed prerequisites, generated an ephemeral manifest, built the compiler/VM, executed the shared SLeeLa vector, and retained its output. The three native/Java jobs completed their platform-specific shared-vector steps.

**Scope limitation:** the workflow version at this run's commit did not yet invoke the numeric/Unicode edge runner. These results are baseline evidence only.

### 4.2 Edge-suite workflow run

**Run:** [42332366905](https://github.com/mearvk/SLeeLa/actions/runs/42332366905)  
**Commit:** `087b9c2d77d0a32cf345c9ec6289a98ea0da00dd`  
**Last recorded state:** QUEUED

| Job | Last recorded state |
|---|---|
| SLeeLa Syntax 3.0 VM vectors — Ubuntu | QUEUED |
| Shared vectors — Windows latest | QUEUED |
| Shared vectors — macOS latest | QUEUED |
| Shared vectors — Ubuntu latest | QUEUED |

The workflow at this revision invokes `comparison/tests/run_edge_vectors.py` on all three native/Java operating-system jobs and uploads `comparison-edge-results.json` alongside the baseline result record. No job conclusion was available at the last check. A queued run is not a test result.

Other workflow executions were triggered by intermediate commits while the batch was being assembled. They are not used as qualification evidence for the complete final suite.

## 5. Retained artifacts and evidence limits

Run 42332289318 retained four artifacts, all reported as unexpired when inspected:

| Artifact | Contents / role |
|---|---|
| `comparison-vectors-ubuntu-latest` | Existing shared native/Java baseline result |
| `comparison-vectors-macos-latest` | Existing shared native/Java baseline result |
| `comparison-vectors-windows-latest` | Existing shared native/Java baseline result |
| `comparison-vectors-sleela-vm` | SLeeLa VM baseline output |

These artifacts belong to the baseline run and do not establish edge-vector results. The edge-runner artifact `comparison-edge-results.json` must be inspected in the completed run before the new suite is recorded as PASS. GitHub artifact retention is time-limited.

## 6. Qualification matrix

| Capability | C11 | C++17 | Java 21+ | SLeeLa VM |
|---|---|---|---|---|
| Existing arithmetic / loop / ASCII baseline | PASS — three OS jobs | PASS — three OS jobs | PASS — three OS jobs | PASS — Ubuntu |
| Signed 32-bit minimum and maximum | Pending CI | Pending CI | Pending CI | Not implemented in this batch |
| Unsigned 32-bit wrap | Pending CI | Pending CI | Pending CI | Not implemented in this batch |
| Signed division and remainder | Pending CI | Pending CI | Pending CI | Not implemented in this batch |
| UTF-8 byte length | Pending CI | Pending CI | Pending CI | Not implemented in this batch |
| Unicode code-point count | Pending CI | Pending CI | Pending CI | Not implemented in this batch |

No result in this table implies complete language conformance.

## 7. Qualification boundaries

Even after the seven edge vectors pass, this batch will qualify only the listed observations for the tested compiler/runtime versions and CI platforms. It does not establish:

- SLeeLa VM parity for these new edge cases.
- Signed overflow behavior beyond the tested fixed-width boundary representations.
- Every integer width, conversion, shift, or overflow policy.
- Floating-point equivalence, NaN behavior, or rounding equivalence.
- Unicode normalization, grapheme segmentation, locale behavior, or malformed UTF-8 validation.
- Array bounds, memory ownership, garbage collection, or sanitizer qualification.
- C ABI or C++ ABI compatibility.
- Java process delegation, JNI, or embedded-JVM lifecycle behavior.
- Concurrency, performance, throughput, latency, or energy use.

The workflow is a correctness suite, not a benchmark. No speed ranking is inferred.

## 8. Next verification actions

1. Allow run 42332366905 to start and complete.
2. Confirm the Ubuntu, macOS, and Windows logs show successful execution of `run_edge_vectors.py`.
3. Inspect each `comparison-edge-results.json` artifact and confirm all seven expected outputs match for C11, C++17, and Java.
4. Confirm the SLeeLa VM baseline job remains successful; do not interpret it as edge-vector coverage.
5. Update [CONFORMANCE-MATRIX.md](CONFORMANCE-MATRIX.md) and [COMMON-TEST-VECTORS.md](COMMON-TEST-VECTORS.md) only after the edge-run evidence is complete.

## 9. Reproduction and authoritative links

Run locally from the repository root with Python 3, a C11 compiler, a C++17 compiler, and JDK 21 or compatible:

```sh
python3 comparison/tests/run_edge_vectors.py
```

- [Comparison Standards workflow](../.github/workflows/comparison-conformance.yml)
- [Comparison directory README](README.md)
- [Numeric and Unicode edge-vector specification](NUMERIC-UNICODE-EDGE-VECTORS.md)
- [Conformance matrix](CONFORMANCE-MATRIX.md)
- [Common test vectors](COMMON-TEST-VECTORS.md)
- [Completed baseline run 42332289318](https://github.com/mearvk/SLeeLa/actions/runs/42332289318)
- [Queued edge-suite run 42332366905](https://github.com/mearvk/SLeeLa/actions/runs/42332366905)
- [Prior baseline PASS record 001](RUNNING.PASS.JAVA.001.md)

---

**Record conclusion:** Iteration 002 is committed and documented. The prior cross-platform baseline remains PASS. The seven numeric/Unicode edge vectors are implemented and wired into CI, but their cross-platform execution is pending; this record intentionally does not claim that they have passed.
