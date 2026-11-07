# Running PASS Record — Java/JVM, Native C/C++, and SLeeLa (Iteration 003)

**Record ID:** RUNNING.PASS.JAVA.003  
**Edition:** 1.0  
**Recorded:** 2026-11-07 (UTC, GitHub Actions timestamps)  
**Repository:** mearvk/SLeeLa · **Branch:** master · **SLeeLa baseline:** Syntax 3.0  
**Record status:** PASS — array/bounds/UTF-8 batch verified on Ubuntu, macOS, and Windows; existing SLeeLa VM baseline verified on Ubuntu

## 1. Executive result

Iteration 003 qualifies ten deterministic array, bounds, and UTF-8 observations in C11, C++17, and Java 21+ on Ubuntu, macOS, and Windows. The specification, implementations, runner, and workflow integration are present on `master`.

The complete workflow run [42333554130](https://github.com/mearvk/SLeeLa/actions/runs/42333554130) completed successfully against commit `b1eeb62b50f8819fdf62f747881c536e1332ff47`. Its three platform jobs each executed the array/string runner. The job logs explicitly report ten passing assertions for each of C11, C++17, and Java 21+ on every platform: 30 implementation assertions per platform and 90 implementation assertions across the three platforms.

The separate SLeeLa Syntax 3.0 VM baseline job also passed. It built the compiler/VM and executed the existing shared SLeeLa vector. It did **not** execute the new array/string vectors; SLeeLa VM parity for the Iteration 003 cases therefore remains unqualified.

## 2. Iteration 003 scope

The normative specification is [ARRAY-STRING-BOUNDS-VECTORS.md](ARRAY-STRING-BOUNDS-VECTORS.md), Standard SCS-TEST-003, Edition 1.0.

| Vector ID | Operation | Expected result |
|---|---|---|
| ARRAY-EMPTY | Empty sequence length | 0 |
| ARRAY-SINGLE | One-element sequence length | 1 |
| ARRAY-FIRST | First element of [11,22,33] | 11 |
| ARRAY-LAST | Last element of [11,22,33] | 33 |
| ARRAY-NEGATIVE | Checked access at index -1 | Rejected |
| ARRAY-PAST-END | Checked access at index equal to length | Rejected |
| UTF8-ROUNDTRIP | Decode and re-encode valid fixture Aé😀 | PASS; identical bytes |
| UTF8-INVALID | Strict validation of overlong byte sequence C0 AF | Rejected |
| UTF8-BYTES | UTF-8 byte length of Aé😀 | 7 |
| UNICODE-CODEPOINTS | Unicode code-point count of Aé😀 | 3 |

The fixture `Aé😀` occupies seven UTF-8 bytes and represents three Unicode code points. Byte length, UTF-16 code-unit count, code-point count, and grapheme-cluster count are distinct measurements.

## 3. Implementations and runner

The committed suite contains:

- `comparison/tests/native/array_string_vectors.c` — C11 checked array access and UTF-8 validation/round-trip vectors.
- `comparison/tests/native/array_string_vectors.cpp` — C++17 checked sequence access and UTF-8 validation/round-trip vectors.
- `comparison/tests/java/ArrayStringVectors.java` — Java array-bound behavior and strict UTF-8 decoder behavior.
- `comparison/tests/run_array_string_vectors.py` — compiles and runs all three implementations, compares their output with the ten expected result lines, and writes `comparison-array-string-results.json`.
- `comparison/ARRAY-STRING-BOUNDS-VECTORS.md` — normative vector definitions and qualification boundaries.

The runner compiles C in C11 mode and C++ in C++17 mode with `-Wall -Wextra -Werror`; Java is compiled with UTF-8 source encoding and `-Xlint:all`. Each implementation must produce the exact ten expected output lines or the runner fails.

## 4. CI evidence ledger

### 4.1 Iteration 003 workflow run

**Run:** [42333554130](https://github.com/mearvk/SLeeLa/actions/runs/42333554130)  
**Conclusion:** SUCCESS  
**Run number:** 46  
**Commit:** `b1eeb62b50f8819fdf62f747881c536e1332ff47`  
**Commit message:** `Add iteration 003 array and string suite to comparison README`

| Platform/job | Conclusion | Array/string runner evidence |
|---|---|---|
| Shared vectors — Ubuntu latest | **PASS** | C11: 10; C++17: 10; Java 21+: 10 assertions |
| Shared vectors — macOS latest | **PASS** | C11: 10; C++17: 10; Java 21+: 10 assertions |
| Shared vectors — Windows latest | **PASS** | C11: 10; C++17: 10; Java 21+: 10 assertions |
| SLeeLa Syntax 3.0 VM vectors — Ubuntu | **PASS** | Existing shared VM baseline executed; new array/string vectors not included |

The three platform job logs explicitly print:

- `PASS C11: 10 array/string assertions`
- `PASS C++17: 10 array/string assertions`
- `PASS Java 21+: 10 array/string assertions`

All three platform jobs completed successfully, as did the separate VM baseline job. The workflow's array/string runner invocation is present in the committed workflow for both POSIX and Windows paths.

## 5. Retained artifacts

Run 42333554130 retained four unexpired artifacts when inspected:

| Artifact | Role |
|---|---|
| `comparison-vectors-ubuntu-latest` | Ubuntu native/Java comparison outputs, including `comparison-array-string-results.json` |
| `comparison-vectors-macos-latest` | macOS native/Java comparison outputs, including `comparison-array-string-results.json` |
| `comparison-vectors-windows-latest` | Windows native/Java comparison outputs, including `comparison-array-string-results.json` |
| `comparison-vectors-sleela-vm` | Existing SLeeLa VM baseline output |

The three platform job logs identify `comparison-array-string-results.json` as a generated and retained file. The workflow run page lists all four artifacts. GitHub artifact retention is time-limited; availability may expire.

## 6. Qualification matrix

| Capability | C11 | C++17 | Java 21+ | SLeeLa VM |
|---|---|---|---|---|
| Empty and one-element sequence lengths | PASS — three OS jobs | PASS — three OS jobs | PASS — three OS jobs | Not tested in this batch |
| First and last valid array indices | PASS — three OS jobs | PASS — three OS jobs | PASS — three OS jobs | Not tested in this batch |
| Negative index rejection | PASS — three OS jobs | PASS — three OS jobs | PASS — three OS jobs | Not tested in this batch |
| One-past-end rejection | PASS — three OS jobs | PASS — three OS jobs | PASS — three OS jobs | Not tested in this batch |
| Valid UTF-8 round trip | PASS — three OS jobs | PASS — three OS jobs | PASS — three OS jobs | Not tested in this batch |
| Invalid overlong UTF-8 rejection | PASS — three OS jobs | PASS — three OS jobs | PASS — three OS jobs | Not tested in this batch |
| UTF-8 byte length | PASS — three OS jobs | PASS — three OS jobs | PASS — three OS jobs | Not tested in this batch |
| Unicode code-point count | PASS — three OS jobs | PASS — three OS jobs | PASS — three OS jobs | Not tested in this batch |
| Existing shared SLeeLa VM baseline | N/A | N/A | N/A | PASS — Ubuntu |

These results qualify only the listed observations and tested implementations/platforms. They do not establish complete language or runtime conformance.

## 7. Qualification boundaries

Iteration 003 does not establish:

- SLeeLa VM parity for the new array and string cases.
- Allocation failure behavior, arbitrary array sizes, slicing, mutation aliasing, or ownership semantics.
- General-purpose UTF-8 validation beyond the specified test vectors.
- Unicode normalization, grapheme segmentation, locale collation, or case mapping.
- Every possible malformed UTF-8 sequence or every Unicode scalar boundary.
- Garbage collection, memory safety under sanitizers, or concurrency behavior.
- C ABI/C++ ABI compatibility or Java process/JNI integration.
- Floating-point equivalence, performance, throughput, latency, or energy use.

The workflow is a correctness suite, not a benchmark. No speed ranking is inferred.

## 8. Next verification actions

1. Preserve run 42333554130 and its three platform result artifacts as the Iteration 003 evidence.
2. Add equivalent SLeeLa VM vectors for the ten array/string observations before claiming VM parity.
3. Continue to the next unqualified conformance area only after the applicable vectors and evidence are defined.

## 9. Reproduction and authoritative links

Run locally from the repository root with Python 3, a C11 compiler, a C++17 compiler, and JDK 21 or compatible:

```sh
python3 comparison/tests/run_array_string_vectors.py
```

- [Comparison Standards workflow](../.github/workflows/comparison-conformance.yml)
- [Comparison directory README](README.md)
- [Array, bounds, and string vector specification](ARRAY-STRING-BOUNDS-VECTORS.md)
- [Conformance matrix](CONFORMANCE-MATRIX.md)
- [Common test vectors](COMMON-TEST-VECTORS.md)
- [Completed Iteration 003 workflow run 42333554130](https://github.com/mearvk/SLeeLa/actions/runs/42333554130)
- [Prior Iteration 002 PASS record](RUNNING.PASS.JAVA.002.md)

---

**Record conclusion:** Iteration 003 is PASS for the ten defined array, bounds, and UTF-8 observations in C11, C++17, and Java 21+ on Ubuntu, macOS, and Windows. The existing SLeeLa Syntax 3.0 VM baseline also passed on Ubuntu. SLeeLa VM parity for the ten new Iteration 003 observations remains unqualified.
