# SiFive U74 (RISC-V RV64GC) Circuit Model

## Scope

This document describes the architectural circuit blocks SLeeLa models reliably for the SiFive U74 (RISC-V RV64GC). It is not a transistor-for-transistor recreation.

## Major blocks

1. **Register section**
   - RISC-V RV64GC register model (x0 hardwired zero, x1-x31, 32 FP), Sv39 MMU

2. **ALU**
   - arithmetic;
   - logic;
   - shifts/rotates;
   - flag generation.

3. **Address path**
   - 39-bit address formation;
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

- https://www.scs.stanford.edu/~zyedidia/docs/sifive/sifive-u74.pdf
- https://en.wikipedia.org/wiki/RISC-V
