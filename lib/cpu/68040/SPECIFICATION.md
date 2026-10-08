# Motorola 68040 Specification

## Programmer model

- D0-D7: 32-bit data registers
- A0-A7: 32-bit address registers
- PC: 32-bit program counter
- SR: status register
- 68020/68030-compatible core instruction model
- variable-length CISC instructions
- rich effective-addressing modes
- byte/word/long-word operations

## Execution

The 68040 integrates integer and floating-point execution and supports superscalar instruction execution. Its pipeline is substantially more aggressive than the 68030.

## Memory

The 68040 integrates:

- instruction cache;
- data cache;
- MMU;
- address translation;
- bus interface.

The exact cache and pipeline behavior remains implementation-specific.
