# Sega Saturn (SH-2) Architecture

## Topology

SLSaturnCPU

-> SLClock
-> SLRegisterFile
-> SLALU
-> SLInstructionDecoder
-> SLIOBus
-> SLIOTiming
-> SLInterruptController

### Register section

The architectural register section is a RISC general-purpose register file: Hitachi SH-2 register model (R0-R15, PC, SR, GBR, VBR); two SH-2 cores in parallel.

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

The SLeeLa representation is compositional: a Sega Saturn (SH-2) variant replaces only the couplers, I/O, timing, cache or opcode components that differ, rather than duplicating the whole CPU hierarchy.

## Sources

- https://electronics.howstuffworks.com/dreamcast.htm
- https://en.wikipedia.org/wiki/SuperH
