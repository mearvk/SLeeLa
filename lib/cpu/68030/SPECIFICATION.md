# Motorola 68030 Specification

## Programmer model

- D0-D7: 32-bit data registers
- A0-A7: 32-bit address registers
- PC: 32-bit program counter
- SR: status register
- rich 68020-compatible effective addressing
- variable-length CISC instructions
- byte/word/long-word operations
- coprocessor interface

## Memory management

The MC68030 integrates a paged MMU with an address translation cache (ATC), multiple translation-table formats, protection controls, and 32-bit physical addressing. The documented ATC contains 22 entries. citeturn0search12

## EC030

The MC68EC030 is modeled separately because NXP identifies it as an embedded version with the MMU removed. citeturn0search0
