# SLeeLa Alphabet

The `alphabet/sleela` directory documents alphabet and character representation within the SLeeLa language ecosystem.

Its purpose is to provide a clear home for alphabet-related conventions used by source files, language tools, educators, and future character-set libraries. This documentation is intended to distinguish language-level rules from implementation-specific behavior.

## Goals

- Explain the role of alphabet and character data in SLeeLa.
- Establish a place to document supported letters, symbols, ordering, and naming conventions.
- Clarify the relationship between textual source, character encoding, and character-set libraries.
- Provide guidance for educators and contributors creating examples or learning materials.
- Keep documented behavior aligned with the language specification and the actual implementation.

## Directory guide

- **[PURPOSE.md](PURPOSE.md)** — scope, intended audiences, and responsibilities of this directory.
- **[SPECIFICATION.md](SPECIFICATION.md)** — documentation checklist and proposed organization for alphabet and character rules.

These documents describe the intended documentation structure. They do not, by themselves, assert that a particular alphabet, encoding, parser feature, or runtime capability has already been implemented.

## Design principles

1. **Unicode-aware:** document characters as text/code points where appropriate; do not assume one byte equals one character.
2. **Encoding clarity:** distinguish source-file encoding, Unicode code points, grapheme clusters, and display glyphs.
3. **Explicit behavior:** specify case handling, normalization, ordering, and invalid-input behavior wherever the implementation defines them.
4. **Compatibility:** defer to the canonical SLeeLa language specification when rules are already defined there.
5. **Testability:** pair normative rules with examples and tests when the relevant implementation exists.

## Contribution workflow

When adding alphabet-related functionality:

1. Identify the canonical language-specification section that governs the behavior.
2. Document the intended rule and its compatibility implications.
3. Add valid and invalid examples.
4. Add or update tests in the appropriate test suite.
5. Update this directory's documents and any affected top-level language/API documentation.

Do not describe planned behavior as implemented until the corresponding code and tests support the claim.
