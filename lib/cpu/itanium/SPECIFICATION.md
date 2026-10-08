# Intel Itanium Specification

## Register architecture

IA-64 provides:

- 128 general registers, 64 bits each;
- 128 floating-point registers, 82 bits of physical storage;
- 64 predicate registers;
- 8 branch registers;
- application registers;
- control registers.

The architecture uses rotating register files to support software-pipelined loops and procedure contexts.

## Instruction bundles

IA-64 instructions are encoded in 128-bit bundles containing three 41-bit instruction slots plus a 5-bit template. Templates identify slot types and stop boundaries.

## Predication

Most instructions can use predicate registers, allowing conditional execution without conventional branch-only control flow.

## EPIC

The compiler communicates parallelism and instruction-group boundaries to the processor. Stop bits identify serialization/group boundaries.

## Memory and speculation

The architecture includes advanced load/store speculation and checking mechanisms, allowing software to expose memory-level parallelism.

## Register Stack Engine

The RSE moves register-stack frames between the rotating register file and backing storage as procedure nesting changes.
