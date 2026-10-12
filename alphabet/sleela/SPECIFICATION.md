# Alphabet and Character Documentation Specification

## 1. Purpose

This document defines documentation requirements for SLeeLa alphabet-related behavior. Native runtime behavior and language-level/compiler support are tracked separately.

## 2. Terminology

- Alphabet: a defined set of written letters for a language or notation.
- Symbol: a written unit used in source text or data.
- UTF-8 encoding: a mapping between Unicode scalar values and bytes.
- Unicode scalar value: a Unicode code point excluding surrogate values U+D800–U+DFFF; maximum U+10FFFF.
- Grapheme cluster: a sequence of code points perceived as one user-facing character.
- Glyph: the visual form used to render a character.
- Catalogue ID: an identifier within a particular catalogue; it is not automatically a Unicode code point.
- Character set: a defined collection of symbols with membership rules.

These concepts are not interchangeable. One visible character may contain multiple scalar values; a UTF-8 scalar value may occupy multiple bytes.

## 3. C<n> character-set capacity signatures

SLeeLa's source contract defines C<n> for decimal n in the range 1..1048576. C<n> is a maximum catalogue-entry capacity, not a bit width, encoding, or populated count. Canonical forms disallow leading zeroes, signs, zero, and values above the maximum.

The native runtime validates these signatures. Compiler/type-checker acceptance of C<n> declarations must not be claimed until the parser, type checker, runtime representation, and serialization path have end-to-end tests.

## 4. UTF-8 behavior

The native runtime in lib/alphabet/native/ performs strict UTF-8 decoding and encoding. It rejects malformed/truncated sequences, overlong encodings, surrogates, and values above U+10FFFF. Its count API counts Unicode scalar values, not grapheme clusters. Normalization and grapheme segmentation are not implemented by this module and require versioned Unicode data.

## 5. Specialized UTF-4088 catalogue

UTF-4088 is an experimental SLeeLa-specific identifier/catalogue system, not a standard UTF encoding or Unicode extension. Values above U+10FFFF belong to a separate identifier namespace and must not be passed to Unicode UTF-8 encoding functions. The catalogue's declared capacity and current registry population are distinct.

The adapter in utf-4088/ performs exact lookup against the published front-end registry. This does not automatically enable UTF-4088 identifiers as native SLeeLa source-language values.

## 6. Library discovery and loading

The read-only scanner tools/sleela-character-scan.py inventories literal and source files without importing or executing them. Its record count is an estimate, not authoritative validation. Runtime loaders must use allowlisted roots, canonicalize paths, reject traversal, enforce file/record limits, and require explicit opt-in before running procedural generators.

## 7. Examples and tests

See EXAMPLES.md and CONFORMANCE-TESTS.md. Native UTF-8, capacity parsing, and bounded-set behavior have C++ regression tests. Compiler C<n> syntax, SLVM native bindings, Unicode normalization/grapheme segmentation, and cross-platform end-to-end integration remain follow-up tests.

## 8. Change control

Changes to character syntax or catalogue serialization must update the language source contract, compiler, VM, API/library docs, and tests. Unsupported conversions must fail explicitly rather than truncate or silently reinterpret identifiers.


## 9. UTF-4088 input maps and ordered subject indexes

UTF-4088 defines a separate experimental spatial traversal protocol in [INPUT-MAP-AND-PRESSURE.md](../../utf-4088/INPUT-MAP-AND-PRESSURE.md). Traversal starts at the top-left, moves right, proceeds down rows, and advances to the next z-layer after completing the current layer. Each element processes pressure iteration levels 1–50 in ascending order. These levels are abstract algorithmic values, not physical pressure or Unicode code points.

The [National Lien Alphabetic Register](../../utf-4088/NATIONAL-LIEN-REGISTER.md) separates permanent catalogue IDs, alphabetic rank, map coordinates, registry ordinal, and pressure level. Its subject meanings must be sourced or explicitly marked provisional/experimental.


## 10. Mirrored UTF-4088 protocol documents

For the complete specialized workflow, see the [UTF-4088 relay index](UTF-4088-RELAY.md). Its mirrors define input-map traversal and bounded depth iterations, the National Lien Alphabetic Register, match-driven probe priority, the serialization boundary, and the SLeeLa adapter. The canonical companion index is [utf-4088/ALPHABET-RELAY.md](../../utf-4088/ALPHABET-RELAY.md). The relay describes proposed protocols; executable engine and end-to-end conformance must be demonstrated separately.
