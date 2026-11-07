# Running PASS Record — Java/JVM, Native C/C++, and SLeeLa (Iteration 002)

**Record ID:** RUNNING.PASS.JAVA.002  
**Edition:** 1.0  
**Recorded:** 2026-11-07 (UTC, GitHub Actions timestamps)  
**Repository:** mearvk/SLeeLa · **Branch:** master · **SLeeLa baseline:** Syntax 3.0  
**Record status:** PASS — numeric/Unicode edge batch verified on Ubuntu, macOS, and Windows; SLeeLa VM baseline verified on Ubuntu

## 1. Executive result

Iteration 002 adds seven deterministic numeric and Unicode edge vectors to the C11, C++17, and Java comparison suite. The source files, runner, specification, and workflow integration are committed to `master`.

The available evidence supports the following conclusion: run [42332366905](https://github.com/mearvk/SLeeLa/actions/runs/42332366905) completed successfully against commit `087b9c2d77d0a32cf345c9ec6289a98ea0da00dd`. All four jobs passed. The Ubuntu, macOS, and Windows jobs each executed the new edge runner, and each reported seven passing assertions for C11, C++17, and Java 21+. The SLeeLa Syntax 3.0 VM baseline job also passed on Ubuntu.

The earlier run [42332289318](https://github.com/mearvk/SLeeLa/actions/runs/42332289318) is retained as baseline evidence only: its workflow revision predated the edge-runner invocation and did not execute the seven new vectors.

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
**Conclusion:** SUCCESS  
**Commit:** `087b9c2d77d0a32cf345c9ec6289a98ea0da00dd`

| Job | Conclusion | Edge-vector evidence |
|---|---|---|
| Shared vectors — Ubuntu latest | **PASS** | C11, C++17, Java 21+: seven assertions each |
| Shared vectors — macOS latest | **PASS** | C11, C++17, Java 21+: seven assertions each |
| Shared vectors — Windows latest | **PASS** | C11, C++17, Java 21+: seven assertions each |
| SLeeLa Syntax 3.0 VM vectors — Ubuntu | **PASS** | Existing shared VM baseline executed successfully; no new edge vectors |

The Ubuntu, macOS, and Windows logs explicitly report `PASS C11: 7 edge-vector assertions`, `PASS C++17: 7 edge-vector assertions`, and `PASS Java 21+: 7 edge-vector assertions`. Each platform job uploaded both `comparison-results.json` and `comparison-edge-results.json`. The VM job built the compiler/VM and passed the existing SLeeLa shared-vector output check.

Other workflow executions were triggered by intermediate commits while the batch was being assembled. They are not used as qualification evidence for the complete final suite.

## 5. Retained artifacts and evidence limits

Run 42332289318 retained four artifacts, all reported as unexpired when inspected:

| Artifact | Contents / role |
|---|---|
| `comparison-vectors-ubuntu-latest` | Existing shared native/Java baseline result |
| `comparison-vectors-macos-latest` | Existing shared native/Java baseline result |
| `comparison-vectors-windows-latest` | Existing shared native/Java baseline result |
| `comparison-vectors-sleela-vm` | SLeeLa VM baseline output |

The completed edge-suite run retained the same four artifact groups. Each of the three platform artifacts contains the baseline result and `comparison-edge-results.json`; the SLeeLa VM artifact contains the VM output. The artifacts were reported as unexpired when inspected and are available from the [completed workflow run](https://github.com/mearvk/SLeeLa/actions/runs/42332366905). GitHub artifact retention is time-limited.

## 6. Qualification matrix

| Capability | C11 | C++17 | Java 21+ | SLeeLa VM |
|---|---|---|---|---|
| Existing arithmetic / loop / ASCII baseline | PASS — three OS jobs | PASS — three OS jobs | PASS — three OS jobs | PASS — Ubuntu |
| Signed 32-bit minimum and maximum | PASS — three OS jobs | PASS — three OS jobs | PASS — three OS jobs | Not implemented in this batch |
| Unsigned 32-bit wrap | PASS — three OS jobs | PASS — three OS jobs | PASS — three OS jobs | Not implemented in this batch |
| Signed division and remainder | PASS — three OS jobs | PASS — three OS jobs | PASS — three OS jobs | Not implemented in this batch |
| UTF-8 byte length | PASS — three OS jobs | PASS — three OS jobs | PASS — three OS jobs | Not implemented in this batch |
| Unicode code-point count | PASS — three OS jobs | PASS — three OS jobs | PASS — three OS jobs | Not implemented in this batch |

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

1. Preserve run 42332366905 and its three platform edge-result artifacts as the iteration 002 evidence.
2. Implement equivalent SLeeLa VM edge vectors before claiming VM parity for these seven observations.
3. Expand the next qualification stage to additional integer widths, conversions, floating-point behavior, malformed UTF-8, and Unicode normalization/grapheme tests.

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
- [Completed edge-suite run 42332366905](https://github.com/mearvk/SLeeLa/actions/runs/42332366905)
- [Prior baseline PASS record 001](RUNNING.PASS.JAVA.001.md)

---

**Record conclusion:** Iteration 002 is PASS for the seven defined numeric/Unicode observations in C11, C++17, and Java 21+ on Ubuntu, macOS, and Windows. The SLeeLa Syntax 3.0 VM baseline also passed on Ubuntu. SLeeLa VM parity for the seven new edge cases remains unqualified.
