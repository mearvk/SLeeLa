# UTF-4088 encoder and decoder

Status: experimental V1 implementation contract.

## Identifier namespaces

The 16,606 front-end records retain identifiers
`0x110000..0x1140DD`. V1 packed input-state identifiers use the separate
interval beginning at `0x200000`. This prevents collisions between the
front-end registry and encoded input states.

## Input packing

A 21-bit packed input contains x (3 bits), y (4 bits), pressure-minus-one
(6 bits), and normalized voltage (8 bits):

| Bits | Field | Valid values |
|---|---|---|
| 0–2 | x | 0–7 |
| 3–6 | y | 0–11 |
| 7–12 | pressure minus one | 0–49 |
| 13–20 | normalized voltage | 0–255 |

Bits 21–31 are reserved and must be zero. Values are logical normalized
application inputs, not raw electrical readings or physical pressure.

The identifier is `0x200000 + packed_input`. Decode subtracts the base and
validates every field. Invalid coordinates, pressure values, reserved bits,
front-end IDs, and out-of-range identifiers are rejected.

## API

Include `encoder_decoder.hpp` and use `pack_input`, `unpack_input`,
`encode_input`, and `decode_input`. `symbol_from_input` accepts a packed
V1 input word and returns the disjoint experimental identifier.

## Limits

This is an identifier mapping, not a Unicode encoding and not a 4,088-bit
integer. It does not encode semantic meaning. Semantic annotations remain
separate registry data with provenance and review status.
