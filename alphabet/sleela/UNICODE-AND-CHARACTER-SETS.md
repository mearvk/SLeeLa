# Unicode, Alphabets, and Character Sets

This document defines the integration contract for SLeeLa's general alphabet APIs and specialized catalogues such as ../../utf-4088/README.md.

## Separate concepts

- UTF-8 bytes: encoded storage units in source files and strings.
- Unicode scalar value: a Unicode code point excluding surrogate values U+D800–U+DFFF; maximum U+10FFFF.
- Grapheme cluster: one user-perceived character, which can contain multiple scalar values.
- Glyph: the rendered visual form.
- Catalogue ID: a stable identifier in a SLeeLa catalogue; it is not automatically a Unicode code point.
- UTF-4088 experimental ID: the separate >U+10FFFF experimental namespace described by the UTF-4088 specification.

Never treat byte count, scalar count, grapheme count, glyph count, and catalogue population as interchangeable.

## Native runtime contract

The additive C++17 module at lib/alphabet/native/ provides strict UTF-8 decode/encode, scalar-value counting, capacity-signature parsing, and bounded ordered-set membership. It rejects malformed, truncated, overlong UTF-8, surrogate values, and out-of-range scalar values. It intentionally does not implement Unicode normalization or grapheme segmentation; those require a versioned Unicode data implementation.

## C<n> capacity signatures

- Grammar: C followed by canonical decimal digits.
- Valid range: C1 through C1048576.
- Leading zeroes, signs, empty suffixes, non-decimal characters, zero, and values above the maximum are invalid.
- C<n> is the maximum number of catalogue entries, not an encoding, bit width, or assertion of actual population.
- Runtime catalogue population must be reported independently from declared capacity.

## Specialized UTF-4088 integration

UTF-4088 remains experimental and is not UTF-8, UTF-16, UTF-32, or a Unicode extension. IDs above U+10FFFF must remain in its own identifier type/namespace and must never be fed to Unicode UTF-8 encoders. Integrations should expose explicit operations such as is_valid_code_point, symbol_from_input, and registry lookup through the UTF-4088 library rather than coercing experimental IDs to char32_t.

## Library discovery and safety

Use tools/sleela-character-scan.py for read-only catalogue inventory. Scanning must not import or execute source files, follow library-provided generator commands, or treat a declared capacity as verified population. Any runtime loader must use an allowlisted root, canonicalize paths, reject path traversal, enforce file/record limits, and require explicit opt-in before loading procedural code.

## Compiler and SLVM integration status

This is the cross-component contract. It does not claim that the current compiler or SLVM already supports arbitrary C<n> declarations, grapheme segmentation, normalization, or UTF-4088 IDs as native language values. Before enabling source syntax, the parser/type checker must validate canonical C<n> values, and the runtime/serializer must preserve catalogue identity separately from Unicode scalar values. Unsupported conversions must return explicit errors rather than truncate.

## Minimum conformance cases

1. ASCII, two-byte, three-byte, and four-byte UTF-8.
2. Empty input and strings containing multiple scalar values.
3. Truncated continuation, invalid continuation, overlong forms, surrogates, and values above U+10FFFF.
4. Capacity bounds, duplicate symbol, empty symbol, invalid UTF-8 symbol, and capacity overflow.
5. UTF-4088 minimum/maximum experimental IDs and rejected out-of-range values.
6. Scanner confirms mode/capacity without executing sources.
