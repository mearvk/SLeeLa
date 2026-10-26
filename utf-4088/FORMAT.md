# UTF-4088 Compact Catalogue Format (Experimental)

This binary format is for the SLeeLa UTF-4088 experiment. UTF-4088 is not a
standard Unicode encoding and these records are not Unicode characters.

## Version 1 layout

All integers are little-endian. The header is 28 bytes, followed by fixed-size
16-byte records. There is no per-record text or variable-length metadata.

| Offset | Size | Field |
|---:|---:|---|
| 0 | 8 | Magic bytes: `UTF4088\0` |
| 8 | 4 | Format version (1) |
| 12 | 4 | Entry count |
| 16 | 4 | Record size (16) |
| 20 | 8 | Deterministic generation seed |
| 28 | 4 | Packed input state |
| 32 | 12 | Glyph bitmap (96 bits, row-major 8×12) |

The record layout repeats until the declared entry count is reached. A glyph is
12 bytes; each bit represents one pixel. Entry ordinal is its stable catalogue
ID (zero-based), so IDs need not be redundantly stored in every record.

## Packed input word

Bits 0–2: x coordinate (0–7); bits 3–6: y coordinate (0–11); bits 7–12:
pressure minus one (pressure 1–50); bits 13–20: normalized voltage (0–255).
Bits 21–31 are reserved and must be zero in version 1. The z-layer is fixed to
zero for this first catalogue version and is recorded in the generation
summary. These normalized values are digital model inputs, not instructions
to read physical electrical voltage directly.

## Capacity and size

The catalogue capacity target is 1,048,576 entries. At 16 bytes per record plus
the 28-byte header, the full file is 16,777,244 bytes (about 16.0 MiB), below
the strict 20,000,000-byte ceiling. The generator refuses a requested output
whose predicted size is greater than or equal to that ceiling.

The 8×12 glyph space has 96 bits per glyph. Version 1 deterministically hashes
the packed input and ordinal with a fixed, versioned seed to derive each bitmap.
The generator checks glyph uniqueness as it writes and fails rather than
claiming success if a duplicate occurs.

## Reproducibility and validation

Use the same Python 3 standard library implementation and seed to reproduce the
same bytes. The generated JSON summary includes the SHA-256 digest, byte count,
entry count, format version, and input constraints. The binary file is generated
as a build artifact rather than committed as a multi-megabyte generated blob.
