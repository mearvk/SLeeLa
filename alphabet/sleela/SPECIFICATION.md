# Alphabet and Character Documentation Specification

## 1. Purpose

This document defines what alphabet-related documentation should record for SLeeLa. It is a documentation checklist, not a claim that every listed capability is currently implemented.

## 2. Terminology

Use these terms consistently:

- **Alphabet:** a defined set of written letters for a language or notation.
- **Symbol:** a written unit used in source text or data; it may be a letter, digit, punctuation mark, or other character.
- **Character encoding:** a mapping between text data and encoded units, such as UTF-8 bytes.
- **Unicode code point:** a numeric value identifying a Unicode code point.
- **Grapheme cluster:** a sequence of code points perceived as one user-facing character in a given context.
- **Glyph:** the visual form used to render a character.
- **Character set:** a defined collection of characters, with membership rules where applicable.

These concepts are related but are not interchangeable. In particular, a visible character may contain multiple code points, and a UTF-8 character may occupy multiple bytes.

## 3. Required documentation for each formally supported set

When a set is implemented and ready to document, record:

1. Canonical name and stable identifier.
2. Intended purpose and supported language or notation.
3. Included characters and any exclusions.
4. Case behavior, if applicable.
5. Ordering rules, if any.
6. Encoding and normalization assumptions.
7. Membership and validation behavior.
8. Invalid-input and error-handling behavior.
9. Language-version compatibility.
10. Tests and representative examples.

If a property is not defined, label it unspecified rather than inventing a rule.

## 4. Source-text considerations

Documentation must distinguish the encoding of source files from the meaning of character literals and from runtime character-set data. UTF-8 byte length must not be presented as a universal count of characters.

Examples involving non-ASCII text should identify any assumptions about compiler input encoding, normalization, and display support.

## 5. Relationship to SLeeLa language features

If SLeeLa defines character or character-set types (including any version-specific `C<n>` forms), document them only in coordination with the canonical language specification and implementation. Record exact syntax, range limits, type checking, conversion rules, and runtime representation only after these have been verified.

Do not infer that a type name, size, or encoding is supported solely because it appears in a proposal or example.

## 6. Examples and tests

For each supported rule, provide:

- A minimal valid example.
- A boundary or edge-case example.
- An invalid example where rejection behavior is defined.
- A corresponding automated test or a reference to the test that verifies it.

Examples must be labeled as **implemented**, **proposed**, or **illustrative** as appropriate.

## 7. Versioning and change control

Changes to documented behavior should identify the applicable SLeeLa language version and update related compiler, VM, API, and library documentation. Compatibility-impacting changes should include migration notes.

## 8. Review checklist

- [ ] Terminology is precise and consistent.
- [ ] Implemented behavior is supported by code or tests.
- [ ] Proposals are clearly labeled.
- [ ] Encoding, code points, grapheme clusters, and glyphs are not conflated.
- [ ] Examples match the stated language version.
- [ ] Related documentation and tests are linked or updated.
