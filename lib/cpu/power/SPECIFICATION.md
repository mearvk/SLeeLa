# IBM POWER Specification

## Programmer model

- 32-bit POWER architecture;
- 32 general-purpose registers;
- 32 floating-point registers;
- 32-bit fixed-length instructions;
- branch processor;
- fixed-point processor;
- floating-point processor;
- condition register;
- link register;
- count register;
- XER;
- POWER-family MQ register;
- processor/system control state.

IBM documents the POWER family and POWER2 as separate implementations, with POWER2 adding implementation-specific instructions and execution resources. citeturn0search0turn0search3

## Architectural boundary

This implementation does not silently substitute later PowerPC or Power ISA facilities for original POWER behavior.
