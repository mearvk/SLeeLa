# Verification Record: Java/JVM, Native C/C++, and SLeeLa VM

**Record ID:** RUNNING.PASS.JAVA.004  
**Edition:** 1.1  
**Recorded:** 2026-11-07 (UTC)  
**Repository:** `mearvk/SLeeLa`  
**Branch:** `master`  
**Language baseline:** SLeeLa Syntax 3.0  
**Overall result:** **FAILURE - SLeeLa VM vector execution failed**

## 1. Executive summary

This record adds SLeeLa VM checks for array bounds and UTF-8 string-literal handling to the existing cross-platform comparison suite.

The GitHub Actions workflow completed. The SLeeLa compiler and VM built successfully, but the SLeeLa VM vector-execution step failed. The three shared-vector platform jobs (Ubuntu, macOS, and Windows) passed.

This is a mixed result, not a full PASS. The native/Java platform results and the SLeeLa VM result are reported separately below.

## 2. Scope and qualification boundary

The extension covers:

- Empty and single-element array lengths.
- Valid first-element and last-element reads.
- Rejection of a negative array index.
- Rejection of an index equal to the array length.
- Equality comparison of identical UTF-8 string literals.
- Exact emission of a UTF-8 test fixture.

The UTF-8 fixture is the literal text `Aé😀`. The non-ASCII characters are intentional test data, not corrupted text. Preserve this fixture exactly in source files and expected-output checks.

This test does not establish that the VM provides a strict UTF-8 decoder, a code-point-count API, or a malformed-byte-input API.

## 3. Test matrix

| Case | Operation | Expected result |
|---|---|---|
| ARRAY-EMPTY | Length of an empty integer array | `0` |
| ARRAY-SINGLE | Length of a one-element integer array | `1` |
| ARRAY-FIRST | Read index 0 from `[11,22,33]` | `11` |
| ARRAY-LAST | Read index 2 from `[11,22,33]` | `33` |
| UTF8-LITERAL-EQUALITY | Compare identical `Aé😀` literals | Equal |
| UTF8-LITERAL-EMISSION | Emit `Aé😀` | Exact fixture |
| ARRAY-NEGATIVE | Read index -1 from a three-element array | Runtime rejection |
| ARRAY-PAST-END | Read index 3 from a three-element array | Runtime rejection |

The invalid-index cases run in separate processes because a runtime bounds error terminates the current program. The runner expects a nonzero exit and the diagnostic `array index out of range`.

## 4. Test implementation

The repository test files are:

- `comparison/tests/sleela/array_string_valid.sleela`
- `comparison/tests/sleela/array_negative_index.sleela`
- `comparison/tests/sleela/array_past_end.sleela`
- `comparison/tests/run_sleela_array_string_vectors.py`

The runner executes the valid and invalid cases and writes `comparison-sleela-array-string-results.json`.

Workflow integration is in:

- `.github/workflows/comparison-conformance.yml`

## 5. CI result

**Workflow:** Comparison Standards Conformance  
**Run:** [GitHub Actions run 42336314226](https://github.com/mearvk/SLeeLa/actions/runs/42336314226)  
**Run number:** 54  
**Head commit:** `fecbbb7e4e4cca7d993fc1af8782f769254d9d73`  
**Commit message:** Correct SLeeLa VM assertion count  
**Final workflow conclusion:** `failure`

| Job | Result | Interpretation |
|---|---|---|
| SLeeLa Syntax 3.0 VM vectors (Ubuntu) | **FAIL** | Build succeeded; vector execution failed |
| Shared vectors (macOS) | **PASS** | Job completed successfully |
| Shared vectors (Windows) | **PASS** | Job completed successfully |
| Shared vectors (Ubuntu) | **PASS** | Job completed successfully |

The failed SLeeLa job completed these steps successfully:

1. Runner setup and source checkout.
2. Runtime build-prerequisite installation.
3. Ephemeral manifest generation.
4. SLeeLa compiler and VM build.
5. Retention of SLeeLa VM output.

The failing step was **Execute shared SLeeLa VM vectors**. The job summary establishes the failing step, but does not by itself identify which individual assertion caused the failure. Inspect the retained output and job logs before changing expected behavior or declaring a test defect.

## 6. Qualification status

### Qualified by this workflow run

- Shared comparison vectors on Ubuntu.
- Shared comparison vectors on macOS.
- Shared comparison vectors on Windows.

### Not qualified by this workflow run

- SLeeLa VM array and string vector suite as a whole.
- Negative-index and past-end runtime rejection on the SLeeLa VM.
- UTF-8 literal equality and exact emission on the SLeeLa VM.

A successful compiler/VM build is not evidence that the VM test suite passed.

### Still outside this test's coverage

- Strict rejection of malformed UTF-8, including the overlong byte sequence `C0 AF`.
- VM-measured UTF-8 byte length of `Aé😀`.
- VM-measured Unicode code-point count of `Aé😀`.

The fixture's UTF-8 representation is seven bytes and contains three Unicode code points. Those are properties of the test data; this record does not claim that the VM computed either measurement.

## 7. Reproduction

From the repository root, build the VM using the CI manifest and environment, then run:

```sh
python3 comparison/tests/run_sleela_array_string_vectors.py
```

The runner expects `impl/build/sleela`, or an alternate executable provided through `SLEELA_BIN`.

## 8. Next corrective actions

1. Open the [failed SLeeLa VM job](https://github.com/mearvk/SLeeLa/actions/runs/42336314226/job/128293940906).
2. Inspect the retained VM output and `comparison-sleela-array-string-results.json`.
3. Identify the exact failing case and distinguish a VM defect from a test expectation or runner defect.
4. Make the smallest justified correction.
5. Rerun the workflow and update this record only from the new run's completed results.

## 9. References

- [Previous comparison record: RUNNING.PASS.JAVA.003.md](RUNNING.PASS.JAVA.003.md)
- [Normative array and string vectors](ARRAY-STRING-BOUNDS-VECTORS.md)
- [Comparison conformance workflow](../.github/workflows/comparison-conformance.yml)
- [Workflow run 42336314226](https://github.com/mearvk/SLeeLa/actions/runs/42336314226)
- [Failed SLeeLa VM job](https://github.com/mearvk/SLeeLa/actions/runs/42336314226/job/128293940906)

---

**Final assessment:** The shared native/Java comparison jobs passed on all three operating-system runners. The SLeeLa VM compiled successfully, but its vector execution failed. Keep SLeeLa VM qualification open until the failing case is diagnosed and a subsequent run passes.
