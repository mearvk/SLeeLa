# Nintendo 64 (NEC VR4300 / MIPS) Architecture

## Topology

SLNintendo64CPU

-> SLClock
-> SLRegisterFile
-> SLALU
-> SLInstructionDecoder
-> SLIOBus
-> SLIOTiming
-> SLInterruptController

### Register section

The architectural register section is a RISC general-purpose register file: MIPS III register model (32 GP, r0 hardwired zero, HI/LO, 32 FP); NEC VR4300.

Internal latches and datapath elements are modeled only where their observable timing or behavior is useful.

### Control section

The control side is represented as:

- instruction register;
- instruction decode;
- control/timing sequencer;
- interrupt logic;
- clock coordination.

### Datapath

registers -> internal bus/latches -> ALU -> register/memory destination

Address formation uses the architectural 32-bit address path.

### Bus

The external interface exposes 32 address lines, a 64-bit data path, read/write control, clock/phase signals, and reset/interrupt controls. Each observable memory transaction is modeled as a bus event (see BUS.md).

## Design principle

The SLeeLa representation is compositional: a Nintendo 64 (NEC VR4300 / MIPS) variant replaces only the couplers, I/O, timing, cache or opcode components that differ, rather than duplicating the whole CPU hierarchy.

## Sources

- https://en.wikipedia.org/wiki/Nintendo_64
- https://en.wikipedia.org/wiki/NEC_VR4300
