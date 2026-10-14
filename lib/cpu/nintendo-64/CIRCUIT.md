# Nintendo 64 (NEC VR4300 / MIPS) Circuit Model

## Scope

This document describes the architectural circuit blocks SLeeLa models reliably for the Nintendo 64 (NEC VR4300 / MIPS). It is not a transistor-for-transistor recreation.

## Major blocks

1. **Register section**
   - MIPS III register model (32 GP, r0 hardwired zero, HI/LO, 32 FP); NEC VR4300

2. **ALU**
   - arithmetic;
   - logic;
   - shifts/rotates;
   - flag generation.

3. **Address path**
   - 32-bit address formation;
   - address latches;
   - effective-address computation.

4. **Data path**
   - 64-bit data latch;
   - internal data movement;
   - external data bus buffer.

5. **Control**
   - instruction register;
   - instruction decode;
   - timing control;
   - interrupt logic.

6. **Clock**
   - clock input;
   - phase sequencing.

## Accuracy boundary

Gate-level or transistor-level detail should be added only from sufficiently reliable die analysis or a validated hardware recreation. This profile stays at the architectural/behavioral/timing level.

## Sources

- https://en.wikipedia.org/wiki/Nintendo_64
- https://en.wikipedia.org/wiki/NEC_VR4300
