# Intel i860 Specification

## Programmer state

- 32 integer registers
- 32 floating-point registers
- program counter
- control/status state
- floating-point condition/control state

The i860 combines scalar RISC operations with floating-point/vector-style operations and a tightly coupled instruction scheduling model.

## Instruction organization

The architecture supports distinct instruction classes and dual-issue operation under appropriate pairing and scheduling rules. The compiler/software scheduler is an important part of expected performance.
