# SLeeLa Alphabet Library

This library contains the SLeeLa-facing alphabet classes and language-specific alphabet definitions. The native C++17 runtime and its regression tests are in native/.

## Main components

- Alphabet.sleela and AlphabetBuilder.sleela: ordered alphabet construction.
- Letter.sleela: per-letter metadata.
- CharacterSet.sleela: SLeeLa 1.9 API surface for bounded character catalogues; native binding is an integration boundary until connected to compiler/SLVM.
- native/: strict UTF-8 scalar decoding/encoding, scalar counts, C<n> capacity parsing, and bounded set membership.
- utf-4088/: experimental catalogue is maintained at the repository root in utf-4088/ and has its own adapter.

## Build the native foundation

    make -C lib/alphabet/native test

The library distinguishes UTF-8 byte strings, Unicode scalar values, grapheme clusters, glyphs, and UTF-4088 experimental IDs. The native runtime does not yet implement normalization or grapheme segmentation.
