# Native Alphabet Runtime

This C++17 module supplies reusable Unicode-scalar UTF-8 validation and character-set membership for SLeeLa integrations. It is an additive library; it does not claim that every compiler/SLVM path already calls it.

## Build and test

    make -C lib/alphabet/native test

The module treats UTF-8 bytes, Unicode scalar values, catalogue IDs, and rendered glyphs as distinct things. Grapheme-cluster segmentation and Unicode normalization are intentionally delegated to a versioned Unicode library rather than approximated here.

## API

- decode_utf8: strict UTF-8 decoding to Unicode scalar values; rejects overlong encodings, surrogates, truncated sequences, and values above U+10FFFF.
- encode_utf8: encode a Unicode scalar value.
- codepoint_count_utf8: count scalar values, not bytes or grapheme clusters.
- CharacterSet: bounded ordered set of UTF-8 symbol strings with duplicate rejection and explicit capacity.
- parse_capacity_signature: validate C<n>, where 1 <= n <= 1,048,576.

UTF-4088 identifiers above U+10FFFF are not Unicode scalar values and must not be passed to this UTF-8 API. See ../../../../utf-4088.
