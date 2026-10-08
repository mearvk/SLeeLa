# Z80 Registers

| Register | Width | Role |
|---|---:|---|
| A | 8 | accumulator |
| F | 8 | flags |
| B/C | 8 each | general purpose |
| D/E | 8 each | general purpose |
| H/L | 8 each | general purpose |
| IX | 16 | index |
| IY | 16 | index |
| SP | 16 | stack pointer |
| PC | 16 | program counter |
| I | 8 | interrupt vector base |
| R | 8 | refresh register |

## Alternate set

The Z80 also provides AF', BC', DE', and HL'.

SLeeLa models these as an alternate register bank rather than duplicating unrelated register classes.

## Flags

F contains programmer-visible condition flags, including sign, zero, half-carry, parity/overflow, add/subtract, and carry semantics.
