# z/Architecture Registers

## General-purpose registers

The architecture provides 16 64-bit general-purpose registers, conventionally named R0-R15. R15 commonly serves as a stack pointer by software convention; the CPU model must not hardwire a universal stack behavior into the register itself.

## Condition code and PSW

Model the condition code and program status word as architecture-defined state. PSW fields, address mode interpretation, mask controls and reserved bits must follow the selected architecture level.

## Optional and privileged state

Access registers, floating-point registers, vector registers, control registers, prefix/lowcore-related state and other special state are facility- or system-dependent. Expose only the documented state for the selected profile.

## Register rules

1. Define width, access mode, privilege and reset/initialization source from the selected profile.
2. Implement architected side effects for special-register access.
3. Keep CPU registers separate from operating-system conventions.
4. Preserve reserved-bit behavior; never assign invented semantics.
5. Report absent facilities and inaccessible registers clearly to the debugger.
