# Motorola ColdFire Specification

## Programmer model

ColdFire retains the core 68K-style programming model:

- D0-D7 data registers;
- A0-A7 address registers;
- PC;
- SR;
- variable-length instructions;
- byte/word/long-word operations;
- load/store and memory-access operations.

ColdFire instruction encodings are derived from the 68K family but are not a binary-identical implementation of every 68000-family instruction.

## Embedded extensions

Depending on core generation, ColdFire may provide:

- hardware multiply-accumulate/DSP;
- FPU;
- MMU;
- instruction/data caches;
- branch acceleration;
- debug support.

Each is modeled as a profile capability.
