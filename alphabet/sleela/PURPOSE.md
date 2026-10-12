# Purpose of `alphabet/sleela`

## Mission

This directory is the documentation home for alphabet-related language concepts in SLeeLa: the written symbols used in source text, the character data represented by programs, and the conventions used by language tools and educational material.

It should make these concepts understandable without conflating source syntax, character encoding, linguistic alphabets, and rendered glyphs.

## Scope

Documentation may cover:

- Alphabet and symbol inventories when formally defined by the project.
- Character names, case distinctions, ordering, and classification.
- The relationship between source text and Unicode.
- Character-set types and libraries, when specified by the language.
- Examples for learners, implementers, and tooling authors.
- Validation and test expectations for supported behavior.

## Out of scope

This directory should not independently redefine the SLeeLa grammar, duplicate the complete API reference, or promise runtime features that have not been implemented. Compiler, VM, and library behavior should be linked to their canonical documentation where available.

## Audiences

- **Learners and educators:** concise explanations and carefully labeled examples.
- **Language implementers:** precise terminology, edge cases, and testable rules.
- **Library authors:** consistent conventions for character and character-set data.
- **Documentation maintainers:** a single place to orient readers to alphabet-related material.

## Source of truth

When a rule conflicts with the main SLeeLa language specification, the canonical language specification takes precedence. Implementation behavior should be verified against source code and tests before it is described as supported.

## Maintenance checklist

- Keep examples consistent with the current language version.
- Mark proposals and future work explicitly.
- Avoid claiming that a specific alphabet or encoding is supported without evidence.
- Link to relevant compiler, VM, library, and test documentation when those paths exist.
- Review this directory whenever character syntax, source encoding, or character-set APIs change.
