# SuperH Circuit Model

## Functional scope

Architectural/functional model; not transistor reconstruction.

## Blocks

1. Instruction fetch.
2. Decoder.
3. Register file.
4. Status/control registers.
5. Integer ALU.
6. Multiply/accumulate unit.
7. Branch/control unit.
8. Load/store unit.
9. Optional DSP/FPU.
10. Optional MMU/TLB.
11. Optional I-cache/D-cache.
12. Exception/interrupt controller.
13. Bus interface.
14. Clock/pipeline control.

## Couplers

SLCoupler models:

- fetch -> decoder;
- decoder -> register file;
- registers -> ALU/MAC;
- branch -> PC/delay state;
- load/store -> MMU/cache;
- cache -> bus;
- FPU/DSP -> register state;
- exception -> control registers;
- DMA -> bus arbitration.

## Accuracy boundary

No SH-family profile receives undocumented cache size, pipeline depth, MMU, FPU, or DMA resources.
