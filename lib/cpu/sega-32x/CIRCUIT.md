# Sega 32X (SH-2) Circuit Model

## Scope

This document describes the architectural circuit blocks SLeeLa models reliably for the Sega 32X (SH-2). It is not a transistor-for-transistor recreation.

## Major blocks

1. **Register section**
   - Hitachi SH-2 register model (R0-R15, PC, SR, GBR, VBR); two SH-2 cores

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
   - 32-bit data latch;
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

- https://en.wikipedia.org/wiki/32X
- https://treasure.fandom.com/wiki/Sega_32X/Technical_Specifications
