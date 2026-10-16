# AMD Athlon (K7) Architecture

## Topology

SLAMDAthlonCPU

-> SLClock
-> SLRegisterFile
-> SLALU
-> SLInstructionDecoder
-> SLIOBus
-> SLIOTiming
-> SLInterruptController

### Register section

The architectural register section is a CISC named/split register set: IA-32 register model (K7 microarchitecture) with on-chip L1 and L2.

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

The external interface exposes 32 address lines, a 32-bit data path, read/write control, clock/phase signals, and reset/interrupt controls. Each observable memory transaction is modeled as a bus event (see BUS.md).

## Design principle

The SLeeLa representation is compositional: a AMD Athlon (K7) variant replaces only the couplers, I/O, timing, cache or opcode components that differ, rather than duplicating the whole CPU hierarchy.

## Sources

- https://en.wikipedia.org/wiki/Athlon
- https://en.wikipedia.org/wiki/List_of_AMD_processors
