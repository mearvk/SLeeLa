# Motorola ColdFire Registers

## Programmer state

- D0-D7: 32-bit data registers
- A0-A7: 32-bit address registers
- PC: program counter
- SR: status register

## Optional execution state

Depending on profile:

- MAC accumulator/control state;
- FPU registers/control state;
- MMU translation state;
- cache control state;
- debug/trace state.

## Stack

A7 serves as the stack pointer within the 68K-derived programming model.
