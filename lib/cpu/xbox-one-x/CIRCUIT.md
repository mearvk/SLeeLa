# Xbox One X (x86-64 Jaguar) Circuit Model

## Scope

This document describes the architectural circuit blocks SLeeLa models reliably for the Xbox One X (x86-64 Jaguar). It is not a transistor-for-transistor recreation.

## Major blocks

1. **Register section**
   - x86-64 register model; 8-core AMD Jaguar-evolved cluster at a higher clock

2. **ALU**
   - arithmetic;
   - logic;
   - shifts/rotates;
   - flag generation.

3. **Address path**
   - 48-bit address formation;
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

- https://en.wikipedia.org/wiki/Xbox_One_X
- https://en.wikipedia.org/wiki/Jaguar_(microarchitecture)
