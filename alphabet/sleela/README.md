# SLeeLa Alphabet

The alphabet documentation directory describes alphabet, character, and character-set behavior across SLeeLa source, libraries, and language tooling. It now includes an explicit Unicode/UTF-4088 boundary contract, examples, and conformance requirements.

## Directory guide

- PURPOSE.md — scope and responsibilities.
- SPECIFICATION.md — terminology and documentation rules.
- UNICODE-AND-CHARACTER-SETS.md — UTF-8, Unicode scalar values, grapheme clusters, C<n>, and UTF-4088 namespace boundaries.
- EXAMPLES.md — illustrative SLeeLa 1.9 usage.
- CONFORMANCE-TESTS.md — implemented native checks and remaining compiler/VM integration tests.

## Related implementation

- lib/alphabet/ — SLeeLa alphabet classes and native character runtime.
- lib/alphabet/native/ — C++17 UTF-8 validation/encoding, scalar counting, C<n> capacity parsing, and bounded set membership.
- utf-4088/ — experimental catalogue and SLeeLa adapter.

## Design principles

1. Never assume one UTF-8 byte equals one character.
2. Distinguish bytes, Unicode scalar values, grapheme clusters, glyphs, and catalogue identifiers.
3. Keep UTF-4088 experimental identifiers separate from Unicode scalar values.
4. Treat C<n> as catalogue capacity, not encoding width or populated-entry count.
5. Do not claim full compiler/SLVM support until end-to-end tests prove it.

Run the native C++ runtime tests with: make -C lib/alphabet/native test. Compiler parsing of C<n>, SLeeLa-facing native bindings, normalization, and grapheme segmentation remain separate integration work.
