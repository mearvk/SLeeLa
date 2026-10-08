# DEC Alpha Registers

## Integer

- R0-R30: 64-bit integer registers
- R31: constant zero

## Floating point

- F0-F30: floating-point registers
- F31: floating-point zero

## Program/control state

The implementation models:

- PC;
- processor status;
- PAL/system state;
- exception/trap state;
- MMU/translation state.

Alpha intentionally has a regular programmer-visible register organization rather than a register-window system.

## Special execution state

Atomic memory operations, PALcode, exception state, and implementation-specific control registers are represented outside the general register file.
