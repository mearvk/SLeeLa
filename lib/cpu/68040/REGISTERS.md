# Motorola 68040 Registers

## Integer state

- D0-D7
- A0-A7
- PC
- SR

## Floating point

The MC68040 contains integrated floating-point execution and its associated architectural state. The LC040 profile removes the floating-point unit.

## Memory-management state

The integrated MMU contains translation/protection state, represented separately from the integer register file.

## Control

Cache, MMU, exception, and processor-control state are represented as dedicated subsystem state.
