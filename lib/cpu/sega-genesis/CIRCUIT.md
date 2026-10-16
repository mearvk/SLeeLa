# Sega Genesis (68000) Circuit Model

## Scope

This document describes the architectural circuit blocks SLeeLa models reliably for the Sega Genesis (68000). It is not a transistor-for-transistor recreation.

## Major blocks

1. **Register section**
   - 68000 register model (D0-D7 data, A0-A7 address, PC, SR); Z80 sound sub-CPU

2. **ALU**
   - arithmetic;
   - logic;
   - shifts/rotates;
   - flag generation.

3. **Address path**
   - 24-bit address formation;
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

- https://en.wikipedia.org/wiki/Sega_Genesis
- https://segaretro.org/images/1/18/GenesisTechnicalOverview.pdf
