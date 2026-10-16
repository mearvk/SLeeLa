# SiFive U74 (RISC-V RV64GC) Architecture

## Topology

SLSiFiveU74CPU

-> SLClock
-> SLRegisterFile
-> SLALU
-> SLInstructionDecoder
-> SLIOBus
-> SLIOTiming
-> SLInterruptController

### Register section

The architectural register section is a RISC general-purpose register file: RISC-V RV64GC register model (x0 hardwired zero, x1-x31, 32 FP), Sv39 MMU.

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

Address formation uses the architectural 39-bit address path.

### Bus

The external interface exposes 39 address lines, a 64-bit data path, read/write control, clock/phase signals, and reset/interrupt controls. Each observable memory transaction is modeled as a bus event (see BUS.md).

## Design principle

The SLeeLa representation is compositional: a SiFive U74 (RISC-V RV64GC) variant replaces only the couplers, I/O, timing, cache or opcode components that differ, rather than duplicating the whole CPU hierarchy.

## Sources

- https://www.scs.stanford.edu/~zyedidia/docs/sifive/sifive-u74.pdf
- https://en.wikipedia.org/wiki/RISC-V
