# 6502 Registers

| Register | Width | Class | Notes |
|---|---:|---|---|
| A | 8 | architectural | accumulator |
| X | 8 | architectural | index |
| Y | 8 | architectural | index |
| S | 8 | architectural | stack pointer |
| PC | 16 | architectural | program counter |
| P | 8 | status | N/V/B/D/I/Z/C semantics |

## Status flags

- N — negative
- V — overflow
- B — break/software-interrupt representation
- D — decimal mode
- I — interrupt disable
- Z — zero
- C — carry

Bit 5 is not modeled as an ordinary programmer-controlled flag.

## Internal state

The physical 6502 contains internal latches and buses used during address and data movement. These are not promoted to architectural registers merely because they exist physically.

SLeeLa may model an internal register/latch when it affects:

- bus visibility;
- cycle timing;
- interrupt restart;
- address formation;
- instruction sequencing.

Sources:
- https://tutorial-6502.sourceforge.io/specification/
- https://tinymachines.ai/6502/archive/wiki/6502_datapath.html
