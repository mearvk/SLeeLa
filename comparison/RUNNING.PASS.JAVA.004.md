# Running Verification Record — Java/JVM, Native C/C++, and SLeeLa (Iteration 003 VM Extension)

**Record ID:** RUNNING.PASS.JAVA.004  
**Edition:** 1.0  
**Recorded:** 2026-11-07 (UTC)  
**Repository:** mearvk/SLeeLa · **Branch:** master · **SLeeLa baseline:** Syntax 3.0  
**Record status:** PENDING — implementation committed; GitHub Actions run queued at time of record

## 1. Purpose and qualification boundary

This extension closes part of the SLeeLa VM evidence gap left by Iteration 003. The earlier native/Java run qualified the ten SCS-TEST-003 observations in C11, C++17, and Java 21+ on Ubuntu, macOS, and Windows. Its separate SLeeLa job ran only the pre-existing arithmetic/control-flow baseline.

This extension adds executable SLeeLa VM checks for the array operations that the current language surface exposes, plus a narrow UTF-8 string-literal equality/emission check. It does not claim that the VM has a strict UTF-8 decoder, a code-point-count API, or a malformed-byte-input API suitable for the remaining UTF-8 vectors.

## 2. VM cases added

| Case | VM operation | Expected result |
|---|---|---|
| ARRAY-EMPTY | arrayLength(new int[0]) | 0 |
| ARRAY-SINGLE | arrayLength(new int[1]) | 1 |
| ARRAY-FIRST | Read index 0 from [11,22,33] | 11 |
| ARRAY-LAST | Read index 2 from [11,22,33] | 33 |
| UTF8-LITERAL-EQUALITY | Compare identical Aé😀 string literals | PASS |
| UTF8-LITERAL-EMISSION | Emit the Aé😀 fixture | Exact fixture |
| ARRAY-NEGATIVE | Read index -1 from a three-element array | Runtime rejection |
| ARRAY-PAST-END | Read index 3 from a three-element array | Runtime rejection |

The two invalid-index cases run as separate processes because the VM terminates the current program on a runtime bounds error. The runner requires a nonzero exit and the "array index out of range" diagnostic for each.

The six valid-case output lines and two expected-failure cases are checked by comparison/tests/run_sleela_array_string_vectors.py.

## 3. Source and runner inventory

- comparison/tests/sleela/array_string_valid.sleela — empty/single array lengths, valid first/last access, UTF-8 literal equality and emission.
- comparison/tests/sleela/array_negative_index.sleela — negative-index rejection.
- comparison/tests/sleela/array_past_end.sleela — one-past-end rejection.
- comparison/tests/run_sleela_array_string_vectors.py — executes all three programs and writes comparison-sleela-array-string-results.json.
- .github/workflows/comparison-conformance.yml — invokes the VM runner after the existing shared VM baseline and retains its JSON result in the SLeeLa VM artifact.

## 4. CI execution

The workflow was triggered by the push to master and is being tracked here:

- [Comparison Standards Conformance run 42336314226](https://github.com/mearvk/SLeeLa/actions/runs/42336314226)
- Workflow commit: fecbbb7e4e4cca7d993fc1af8782f769254d9d73
- At the time this record was written, the run was **queued** and had not produced job results.

A queued workflow is not a PASS. This record must be updated with the completed run conclusion, VM job log evidence, and retained artifact before the SLeeLa VM subset can be marked qualified.

## 5. What remains unqualified

Even after this subset passes, the following SCS-TEST-003 observations remain unqualified on the SLeeLa VM:

- Strict rejection of malformed UTF-8, including the overlong byte sequence C0 AF.
- VM-measured UTF-8 byte length of Aé😀.
- VM-measured Unicode code-point count of Aé😀.

The fixture's seven-byte UTF-8 encoding and three-code-point count are known test-data properties; this runner does not claim that the VM computed those metrics.

The existing native/Java results remain separately documented in [RUNNING.PASS.JAVA.003.md](RUNNING.PASS.JAVA.003.md). This record does not replace or broaden those platform results.

## 6. Reproduction

From the repository root, after building the VM with the same manifest and SHEET environment used by CI:

```sh
python3 comparison/tests/run_sleela_array_string_vectors.py
```

The runner expects impl/build/sleela, or an alternate executable supplied through SLEELA_BIN.

## 7. Evidence links

- [Iteration 003 native/Java and baseline VM record](RUNNING.PASS.JAVA.003.md)
- [SCS-TEST-003 normative vectors](ARRAY-STRING-BOUNDS-VECTORS.md)
- [Comparison conformance workflow](../.github/workflows/comparison-conformance.yml)
- [Tracked workflow run 42336314226](https://github.com/mearvk/SLeeLa/actions/runs/42336314226)

---

**Current conclusion:** implementation and workflow integration are committed. SLeeLa VM qualification is pending the completed CI run and inspection of its VM job logs and retained JSON artifact.
