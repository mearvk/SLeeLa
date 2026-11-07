# Verification Record: Java/JVM, Native C/C++, and SLeeLa VM

**Record ID:** RUNNING.PASS.JAVA.004  
**Edition:** 1.2  
**Updated:** 2026-11-07 (UTC)  
**Repository:** `mearvk/SLeeLa`  
**Branch:** `master`  
**Language baseline:** SLeeLa Syntax 3.0  
**Overall result:** **CI verification pending for the USEFFUL fixture update**

## 1. Executive summary

This record tracks cross-platform array and string checks, including the SLeeLa VM subset. The SLeeLa VM test string is now the plain ASCII literal `USEFFUL`, replacing the previous mixed-Unicode fixture. This makes the VM test's purpose straightforward: verify literal equality and exact string emission without conflating it with Unicode decoding or display behavior.

The independent C/C++ and Java comparison vectors continue to test UTF-8 validation and Unicode metrics. Their Unicode fixture is intentionally unchanged because those tests specifically exercise UTF-8 behavior.

## 2. SLeeLa VM test scope

The SLeeLa VM subset covers:

- Empty and single-element array lengths.
- Valid first-element and last-element reads.
- Rejection of a negative array index.
- Rejection of an index equal to the array length.
- Equality comparison of identical `USEFFUL` string literals.
- Exact emission of the string `USEFFUL`.

Expected string output:

```text
STRING-LITERAL-EQUALITY=PASS
STRING-FIXTURE=USEFFUL
```

The test makes no claim about UTF-8 decoding, Unicode code-point counting, or malformed-byte rejection in the SLeeLa VM.

## 3. SLeeLa VM test matrix

| Case | Operation | Expected result |
|---|---|---|
| ARRAY-EMPTY | Length of an empty integer array | `0` |
| ARRAY-SINGLE | Length of a one-element integer array | `1` |
| ARRAY-FIRST | Read index 0 from `[11,22,33]` | `11` |
| ARRAY-LAST | Read index 2 from `[11,22,33]` | `33` |
| STRING-LITERAL-EQUALITY | Compare identical `USEFFUL` literals | Equal |
| STRING-LITERAL-EMISSION | Emit `USEFFUL` | Exact fixture |
| ARRAY-NEGATIVE | Read index -1 from a three-element array | Runtime rejection |
| ARRAY-PAST-END | Read index 3 from a three-element array | Runtime rejection |

The invalid-index cases run in separate processes because a runtime bounds error terminates the current program. The runner expects a nonzero exit and the diagnostic `array index out of range`.

## 4. Test implementation

- `comparison/tests/sleela/array_string_valid.sleela`
- `comparison/tests/sleela/array_negative_index.sleela`
- `comparison/tests/sleela/array_past_end.sleela`
- `comparison/tests/run_sleela_array_string_vectors.py`

The runner filters only documented `[memory-manager]` telemetry before comparing exact program output. It writes `comparison-sleela-array-string-results.json`.

Workflow integration: `.github/workflows/comparison-conformance.yml`.

## 5. CI status

The preceding output-filtering correction was committed as `a4e9488ab474ea9f212661ec41855e98be79b32e` and triggered [workflow run 42337502338](https://github.com/mearvk/SLeeLa/actions/runs/42337502338).

This edition changes the VM string fixture and expected output. Its CI result must be taken from the new workflow run triggered by this edition; prior results do not qualify the updated fixture.

## 6. Qualification boundary

The SLeeLa VM checks qualify only the array operations and the `USEFFUL` literal equality/emission behavior covered above.

The separate native/Java shared vectors continue to cover UTF-8 round-tripping, malformed UTF-8 rejection, UTF-8 byte length, and Unicode code-point count. Their `Aé😀` fixture remains unchanged and is not part of the SLeeLa VM fixture.

## 7. Reproduction

From the repository root:

```sh
python3 comparison/tests/run_sleela_array_string_vectors.py
```

The runner expects `impl/build/sleela`, or an alternate executable supplied through `SLEELA_BIN`.

## 8. References

- [Previous comparison record: RUNNING.PASS.JAVA.003.md](RUNNING.PASS.JAVA.003.md)
- [Normative array and string vectors](ARRAY-STRING-BOUNDS-VECTORS.md)
- [Comparison conformance workflow](../.github/workflows/comparison-conformance.yml)
- [Previous output-filter correction](https://github.com/mearvk/SLeeLa/commit/a4e9488ab474ea9f212661ec41855e98be79b32e)

---

**Assessment:** The SLeeLa VM test string is now `USEFFUL`. The new fixture remains unqualified until its CI run completes successfully.
