# ESA/390 Registers and Program State

## General-purpose registers

The architecture retains sixteen 32-bit general-purpose registers. Preserve architectural widths and semantics independently of host-language integer widths.

## Control and access state

Represent control registers, access registers, floating-point registers, and other state only according to the selected ESA/390 profile and supported facilities. Enforce privilege and register-access restrictions.

## PSW and condition code

Use the ESA/390 PSW format and mode semantics applicable to the selected profile. Keep condition-code behavior tied to instruction definitions and interruption transitions.

## Debugger contract

Expose register names, widths, values, access restrictions, and profile availability. Do not expose later z/Architecture-only state as native ESA/390 state.
