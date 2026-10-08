# Motorola 68060 Specification

## Programmer model

- D0-D7: 32-bit data registers
- A0-A7: 32-bit address registers
- PC: 32-bit program counter
- SR: status register
- variable-length 68K CISC instructions
- rich effective-addressing modes
- byte/word/long-word operations

## Execution

The 68060 uses a superscalar pipeline with multiple integer execution resources, branch processing, and integrated floating-point execution on the full MC68060.

## Memory

The processor integrates:

- instruction cache;
- data cache;
- MMU;
- translation resources;
- bus interface.

## Compatibility

The 68060 preserves the broad 68000-family software model but has implementation-specific differences from 68040 and earlier processors.
