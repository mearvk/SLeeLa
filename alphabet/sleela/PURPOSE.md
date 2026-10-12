# Purpose of alphabet/sleela

## Mission

This directory documents SLeeLa's general alphabet APIs and specialized character catalogues. It distinguishes source text, encodings, Unicode scalar values, grapheme clusters, glyphs, and catalogue-specific identifiers.

## Scope

- Alphabet and symbol inventories, ordering, naming, and classification.
- UTF-8 source handling and Unicode concepts.
- The C<n> character-set capacity signature family.
- Specialized catalogues such as UTF-4088 and their identifier namespaces.
- Educator examples, compiler/VM contracts, conformance tests, and migration notes.

## Out of scope

This directory does not independently redefine the whole SLeeLa grammar or assert that every documented API is already wired into the compiler and SLVM. Proposed or illustrative source syntax must be labelled accordingly.

## Related implementation

- lib/alphabet/: SLeeLa-facing alphabet classes.
- lib/alphabet/native/: tested C++17 Unicode-scalar UTF-8 and bounded character-set primitives.
- utf-4088/: experimental character catalogue and SLeeLa adapter.
- tools/sleela-character-scan.py: read-only source-folder inventory; it does not execute generators.

## Source of truth

The language specification governs source syntax. Implementation status must be verified against native tests and compiler/VM integration tests. C<n> is capacity, not a bit width; UTF-4088 identifiers above U+10FFFF are not Unicode scalar values.

## Maintenance checklist

- Keep docs synchronized with compiler, VM, and library changes.
- Distinguish implemented behavior from proposed language syntax.
- Add malformed-input and boundary tests.
- Never treat declared capacity as actual catalogue population.
