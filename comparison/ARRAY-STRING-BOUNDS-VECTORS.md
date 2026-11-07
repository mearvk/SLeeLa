# Array, Bounds, and String Vectors
Standard: SCS-TEST-003 | Edition 1.0 | Status: pending CI

## Purpose
Portable observations for empty and populated arrays, checked index rejection, strict UTF-8 validation, byte-preserving round trips, and Unicode code-point counting. Target implementations: C11, C++17, and Java 21+.

## Vector ledger
| ID | Operation | Expected |
|---|---|---|
| ARRAY-EMPTY | Empty sequence length | 0 |
| ARRAY-SINGLE | One-element sequence length | 1 |
| ARRAY-FIRST | First element of [11,22,33] | 11 |
| ARRAY-LAST | Last element of [11,22,33] | 33 |
| ARRAY-NEGATIVE | Checked access at index -1 | Rejected |
| ARRAY-PAST-END | Checked access at index equal to length | Rejected |
| UTF8-ROUNDTRIP | Decode and re-encode valid fixture Aé😀 | PASS; identical bytes |
| UTF8-INVALID | Strictly validate overlong byte sequence C0 AF | Rejected |
| UTF8-BYTES | UTF-8 byte length of Aé😀 | 7 |
| UNICODE-CODEPOINTS | Unicode code-point count of Aé😀 | 3 |

## Contract
- C uses an explicit checked accessor; invalid indices are never dereferenced.
- C++ uses a checked sequence accessor and rejects negative indices before unsigned conversion.
- Java uses native array indexing and observes the bounds exception.
- C/C++ validators check lead-byte ranges, continuation bytes, overlong forms, surrogate encodings, and values above U+10FFFF for these vectors. Java uses a UTF-8 decoder configured to report malformed input.
- The fixture contains seven UTF-8 bytes and three Unicode code points. Byte length, UTF-16 code-unit count, code-point count, and grapheme count are different metrics.

## Run
`python3 comparison/tests/run_array_string_vectors.py`

The runner emits `comparison-array-string-results.json` with platform and per-implementation output. CI retains this result for each OS.

## Qualification boundary
Passing qualifies only these safe access and UTF-8 observations. It does not qualify allocation failure, slicing, mutation aliasing, normalization, grapheme segmentation, locale collation, or SLeeLa VM array/string semantics.
