# Atari 7800 (6502C) Circuit Model

## Scope

This document describes the architectural circuit blocks SLeeLa models reliably for the Atari 7800 (6502C). It is not a transistor-for-transistor recreation.

## Major blocks

1. **Register section**
   - 6502-class model (A, X, Y, S, PC, P); MOS 6502C (SALLY)

2. **ALU**
   - arithmetic;
   - logic;
   - shifts/rotates;
   - flag generation.

3. **Address path**
   - 16-bit address formation;
   - address latches;
   - effective-address computation.

4. **Data path**
   - 8-bit data latch;
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

- https://en.wikipedia.org/wiki/Atari_7800
- https://en.wikipedia.org/wiki/MOS_Technology_6502
