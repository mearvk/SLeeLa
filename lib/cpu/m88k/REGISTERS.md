# Motorola 88000 Registers

## General registers

- R0-R31
- 32-bit registers
- R0 is the architectural zero register.

## Control registers

The architecture includes separate control state for:

- processor status;
- exception/trap handling;
- MMU/translation;
- cache/system control;
- interrupt state.

## Floating point

Floating-point operations use the general register resources and implementation-defined execution facilities rather than a separate SPARC-style register-window system.

## No register windows

The 88000 programmer model does not use the SPARC register-window mechanism. Procedure linkage and stack state are software conventions.
