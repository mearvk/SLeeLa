# 6502 Circuit Model

## Scope

This document describes the architectural circuit blocks that SLeeLa can model reliably. It is not a transistor-for-transistor recreation.

## Major blocks

1. **Register section**
   - A accumulator
   - X and Y index registers
   - S stack pointer
   - PC high/low
   - status register

2. **ALU**
   - arithmetic;
   - logic;
   - shifts/rotates;
   - flag generation.

3. **Address path**
   - low/high address components;
   - address latches;
   - effective-address formation.

4. **Data path**
   - data latch;
   - internal data movement;
   - external data bus buffer.

5. **Control**
   - instruction register;
   - instruction decode;
   - PLA/control logic;
   - timing control;
   - interrupt logic.

6. **Clock**
   - clock input;
   - phi1/phi2 phase sequencing.

## Internal buses

The 6502 analysis literature identifies internal buses and latches such as:

- ADL/ABL;
- ADH/ABH;
- SB;
- DB;
- DL.

SLeeLa represents these as couplers or internal timing channels only when needed by a specific simulation.

## Accuracy boundary

Gate-level or transistor-level work should be added only from sufficiently reliable die analysis or a validated hardware recreation. The MAME 6502 implementation specifically targets exact bus-cycle behavior and has been compared against gate-level simulation, making it useful as a behavioral/timing reference.

Sources:
- https://tinymachines.ai/6502/archive/wiki/6502_datapath.html
- https://github.com/mamedev/mame/blob/master/docs/source/techspecs/m6502.rst
