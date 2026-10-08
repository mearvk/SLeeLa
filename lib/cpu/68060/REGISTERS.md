# Motorola 68060 Registers

## Integer state

- D0-D7
- A0-A7
- PC
- SR

## Floating point

The MC68060 includes integrated floating-point execution. The LC060 profile removes the FPU.

## MMU state

Translation/protection registers and MMU control state are represented independently from the integer register file.

## Control state

Cache, branch, exception, debug, and processor-control state are represented as subsystem resources.
