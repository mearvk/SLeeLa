# Type and Value Mapping
Standard SCS-TYPE-001 | Edition 1.0

## 1. Purpose
Mappings identify conceptual counterparts. They do not assert identical memory layout, ABI, or behavior.

## 2. Primitive comparison
| Concept | SLeeLa | Java | C | C++ |
|---|---|---|---|---|
| Boolean | SLeeLa boolean contract | boolean | _Bool (C99+) | bool |
| 8-bit signed | Explicit SLeeLa width | byte | int8_t, if provided | std::int8_t, if provided |
| 16-bit signed | Explicit SLeeLa width | short | int16_t | std::int16_t |
| 32-bit signed | int32 | int | int32_t | std::int32_t |
| 64-bit signed | int64; verify int alias | long | int64_t | std::int64_t |
| 32-bit float | float | float | float | float |
| 64-bit float | double | double | double | double |
| Character | SLeeLa character contract | 16-bit UTF-16 code unit char | integer character types | char, char8_t, char16_t, char32_t, wchar_t |
| String | SLeeLa String contract | immutable UTF-16 String | no built-in string | library string types |

SLeeLa Syntax 3.0 documents signed and unsigned width families. Verify actual implementation and precision for each width; a type name alone does not prove a native slot or full-precision behavior.

## 3. Integers
For each type record bit width, signedness, range, literal rules, promotion, overflow, division/remainder, shifts, conversions, and serialization byte order.

Java primitive integer widths are fixed. C/C++ fundamental integer widths depend on implementation; exact-width types are optional where the implementation provides them. Unsigned C/C++ arithmetic is modulo the corresponding width. Signed overflow in C/C++ is not a portable wrapping contract. Java integer arithmetic has defined fixed-width behavior. SLeeLa rules must come from its normative syntax/VM specifications and tests.

## 4. Floating point
Test finite values, signed zero, infinities, NaN, subnormals, rounding, conversions, and fused operations separately. Record precision and tolerance. Bitwise equality requires an explicitly fixed representation and operation sequence.

## 5. Characters and strings
Do not equate a byte, character, code unit, Unicode code point, or grapheme cluster. Java char is a UTF-16 code unit; supplementary code points use surrogate pairs. C/C++ char is a byte-sized storage unit, not necessarily a Unicode character. UTF-8 is a sequence of encoded bytes.

SLeeLa must document encoding, indexing unit, length unit, normalization, invalid-sequence behavior, and whether indexing may split a multi-unit character. Convert explicitly at boundaries.

## 6. Arrays and collections
| Concern | SLeeLa | Java | C | C++ |
|---|---|---|---|---|
| Fixed array | SLeeLa array contract | allocated array object | native array | native array / std::array |
| Dynamic sequence | SLeeLa collection | ArrayList/collections | explicit allocation and length | std::vector/containers |
| Bounds | VM/runtime contract | checked for Java arrays | raw arrays not automatically checked | raw arrays not automatically checked |
| Ownership | VM/lifetime rules | managed references | programmer-defined | RAII and/or explicit |

Never pass a managed array as a raw C pointer without a documented pin/copy, lifetime, and bounds contract.

## 7. Records and objects
Java objects have identity and managed lifetime. C structs have ABI-defined layout, padding, and alignment. C++ classes may include constructors, destructors, virtual dispatch, inheritance, and ABI-sensitive features. SLeeLa mappings must state whether the boundary uses a copy, opaque handle, serialized record, or explicit C structure.

## 8. Conversion classes
Classify every conversion as exact/lossless, range-checked, narrowing, sign-changing, precision-losing, encoding conversion, ownership transfer, or prohibited. Never silently truncate, reinterpret pointers, change signedness, or transfer ownership.

## 9. Interchange
Portable files and network messages must define field widths, signedness, byte order, encoding, framing, version, and validation. Prefer versioned serialization to dumping native structures. Do not persist native padding or pointers.

## 10. Required tests
Cover min/max values, conversion boundaries, overflow, negative values, zero, array bounds, string encoding, and serialization with exact compiler/runtime versions.
