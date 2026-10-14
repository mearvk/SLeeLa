# WDC 65C816 Circuit Model

## Scope

This document describes the architectural circuit blocks SLeeLa models reliably for the WDC 65C816. It is not a transistor-for-transistor recreation.

## Major blocks

1. **Register section**
   - A (8/16), X/Y (8/16), 16-bit stack pointer, direct-page, 24-bit program address, status

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
   - 16-bit data latch;
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

- https://wikiwand.com/en/articles/65C816
