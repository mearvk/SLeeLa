# UTF-4088 Code-Point and Serialization Contract

Status: experimental implementation contract, version 1.

## 1. Scope and representation

The current implementation stores both `InputState` and `CodePoint` in unsigned 64-bit integers. It does not implement a 4088-bit integer or a byte-stream encoding of that width. The accepted experimental identifier interval is:

- Minimum: `0x110000` (inclusive).
- Maximum: `0x1FFFFFFFF` (inclusive).
- Width required for this interval: 33 bits.

The interval deliberately starts above Unicode's maximum code point. These values are UTF-4088 project identifiers, not Unicode scalar values. APIs must not label them Unicode characters or silently pass them to Unicode encoders.

## 2. Canonical registry record

A registry record contains `integer_id`, `stage`, `language`, `codepoint`, `shape_id`, and `meaning_id`. Registry generation is deterministic under `UTF4088-RG-1`. The manifest is the source of truth for the record count, enumeration order, and hash seeds.

## 3. Canonical glyph representation

A glyph is 12 rows of 8 bits each. Row order is top-to-bottom; within each row, bit `x` represents horizontal pixel `x`, where `x=0` is the least significant bit. Unused bits are not permitted because each row is exactly 8 bits. This stores exactly 96 bitmap bits.

Seed-derived glyphs are reproducible experimental placeholders. They are not automatically valid writing, language-specific characters, or historically attested symbols.

## 4. Determinism and validation

Implementations must:
- Use the fixed integer widths and limits documented above.
- Reject experimental identifiers outside the defined interval at the public code-point boundary.
- Avoid undefined behavior when floating-point inputs are NaN or infinite.
- Clamp values before quantization and mask each packed field before shifting.
- Preserve directed edge identity as the ordered `(from, to)` pair; a hash value is an index aid, never the edge's identity.
- Keep generated IDs stable within a manifest version.
- Record source and licensing/provenance details before labeling a glyph or meaning as historically sourced.

## 5. Versioning

Any change to identifier bounds, field widths, bit order, registry count, hash seeds, or record interpretation requires a new format/manifest version. This experimental contract makes no claim of Unicode compatibility or standards approval.


## 6. Input-map traversal metadata

Input-map coordinates and pressure iterations are separate from registry identifiers and glyph serialization. Follow [UTF-4088-INPUT-MAP-AND-PRESSURE.md](UTF-4088-INPUT-MAP-AND-PRESSURE.md) for top-left, left-to-right, top-to-bottom, then next-z traversal and pressure levels 1–50. The National Lien subject order is specified separately in [UTF-4088-NATIONAL-LIEN-REGISTER.md](UTF-4088-NATIONAL-LIEN-REGISTER.md). Do not encode coordinates, alphabetic ranks, or pressure levels into an identifier unless a future version explicitly defines a versioned serialization format.


## Canonical source and relay

This is a synchronized mirror in the alphabet/sleela directory. Its canonical source is [utf-4088/CODE-POINT-AND-SERIALIZATION.md](../../utf-4088/CODE-POINT-AND-SERIALIZATION.md). Use [UTF-4088 relay index](UTF-4088-RELAY.md) to navigate the complete mirrored series. Update the canonical source and its mirror together when changing the protocol.
