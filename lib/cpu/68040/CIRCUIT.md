# Motorola 68040 Circuit Model

## Functional scope

Architectural/functional model, not transistor reconstruction.

## Major blocks

1. Instruction fetch/decode.
2. Instruction cache.
3. Integer register file.
4. Address-generation unit.
5. Integer execution units.
6. Floating-point unit.
7. Data cache.
8. MMU/translation.
9. Branch/control.
10. Exception/interrupt control.
11. Bus/burst interface.
12. Issue/pipeline control.
13. Write-back/completion.

## Couplers

SLCoupler models:

- fetch -> I-cache/decode;
- decode -> issue;
- registers -> integer/FPU;
- address generation -> MMU;
- MMU -> I/D cache;
- D-cache -> bus;
- FPU -> completion;
- exception -> processor state;
- DMA -> bus arbitration.

## Accuracy boundary

No 68060-specific superscalar depth, branch prediction, or cache topology is projected backward onto the 68040.
