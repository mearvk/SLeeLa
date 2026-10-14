# Xbox (x86 Pentium III) Circuit Model

## Scope

This document describes the architectural circuit blocks SLeeLa models reliably for the Xbox (x86 Pentium III). It is not a transistor-for-transistor recreation.

## Major blocks

1. **Register section**
   - IA-32 register model; customised Intel Pentium III (Coppermine-based)

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

- https://en.wikipedia.org/wiki/Xbox_technical_specifications
- https://www.copetti.org/writings/consoles/xbox/
