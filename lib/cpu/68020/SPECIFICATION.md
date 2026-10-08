# Motorola 68020 Specification

## Programmer model

- D0-D7: 32-bit data registers
- A0-A7: 32-bit address registers
- PC: 32-bit program counter
- SR: status register
- USP/SSP/MASP stack-state mechanisms as applicable to supervisor/user operation
- 32-bit instruction/data operations
- variable-length CISC instructions
- rich effective-addressing modes
- byte, word, and long-word operations

## Addressing

The 68020 expands the 68000 effective-address system with powerful indexed and memory-indirect addressing forms.

## Coprocessor boundary

The architecture provides explicit coprocessor interface facilities. Floating-point hardware therefore remains a profile/system component rather than being silently assumed as an on-chip 68020 execution unit.
