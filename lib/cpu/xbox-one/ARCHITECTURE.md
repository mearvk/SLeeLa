# Xbox One (x86-64 Jaguar) Architecture

## Topology

SLXboxOneCPU

-> SLClock
-> SLRegisterFile
-> SLALU
-> SLInstructionDecoder
-> SLIOBus
-> SLIOTiming
-> SLInterruptController

### Register section

The architectural register section is a CISC named/split register set: x86-64 register model; two quad-core AMD Jaguar modules (8 cores).

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

Address formation uses the architectural 48-bit address path.

### Bus

The external interface exposes 48 address lines, a 64-bit data path, read/write control, clock/phase signals, and reset/interrupt controls. Each observable memory transaction is modeled as a bus event (see BUS.md).

## Design principle

The SLeeLa representation is compositional: a Xbox One (x86-64 Jaguar) variant replaces only the couplers, I/O, timing, cache or opcode components that differ, rather than duplicating the whole CPU hierarchy.

## Sources

- https://en.wikipedia.org/wiki/Xbox_One
- https://en.wikipedia.org/wiki/Jaguar_(microarchitecture)
