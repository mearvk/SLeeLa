# Numeric and Unicode Edge Vectors
Standard: SCS-TEST-002 | Edition 1.0 | Status: executable cross-language batch

## Purpose
This suite extends the arithmetic/ASCII smoke test with explicitly defined fixed-width integer and Unicode observations. It currently tests C11, C++17, and Java 21+ on the configured operating systems. It does not yet claim SLeeLa VM coverage for these edge cases.

## Normative expectations
- Signed 32-bit minimum and maximum are exactly -2147483648 and 2147483647.
- Unsigned 32-bit addition wraps modulo 2^32; UINT32_MAX + 1 is zero.
- Signed integer division truncates toward zero for the selected operands: -7 / 3 is -2.
- Signed remainder follows the quotient identity for the selected operands: -7 % 3 is -1.
- The string Aé😀 encodes as seven UTF-8 bytes and contains three Unicode code points.
- A byte count, UTF-16 code-unit count, Unicode code-point count, and grapheme count are different measurements. The suite explicitly reports UTF-8 byte length and Unicode code-point count, not a generic “string length”.

## Vector ledger
| ID | Input / operation | Expected |
|---|---|---:|
| INT32-MIN | Minimum signed 32-bit value | -2147483648 |
| INT32-MAX | Maximum signed 32-bit value | 2147483647 |
| UINT32-WRAP | UINT32_MAX + 1 | 0 |
| SIGNED-DIV | -7 / 3, truncating toward zero | -2 |
| SIGNED-REM | -7 % 3 | -1 |
| UTF8-BYTES | UTF-8 byte length of Aé😀 | 7 |
| UNICODE-CODEPOINTS | Unicode code-point count of Aé😀 | 3 |

## Implementation notes
- C uses int32_t/uint32_t and a small UTF-8 leading-byte/continuation-byte counter for this known-valid test string.
- C++ uses std::int32_t/std::uint32_t and a UTF-8 byte string; it counts code points by UTF-8 sequence width for this known-valid test string.
- Java uses int, Integer.MIN_VALUE/MAX_VALUE, StandardCharsets.UTF_8, and String.codePointCount.
- The C/C++ UTF-8 helpers are intentionally scoped to the known-valid fixture. They are not general-purpose Unicode validators and do not test malformed, overlong, surrogate, or out-of-range sequences.

## Commands
Run locally with Python 3, a C11 compiler, a C++17 compiler, and JDK 21+:

```sh
python3 comparison/tests/run_edge_vectors.py
```

The runner emits comparison-edge-results.json with actual outputs, toolchain version strings, platform, and overall status. CI uploads this file per operating system.

## Qualification boundary
Passing this suite qualifies only these seven observations for the tested compiler/runtime versions and CI platforms. It does not establish SLeeLa VM behavior, floating-point equivalence, every integer width, overflow rules for signed values, Unicode normalization, grapheme segmentation, malformed UTF-8 handling, or ABI compatibility.
