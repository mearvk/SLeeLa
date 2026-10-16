# Xbox Series X (x86-64 Zen 2) Architecture

## Topology

SLXboxSeriesXCPU

-> SLClock
-> SLRegisterFile
-> SLALU
-> SLInstructionDecoder
-> SLIOBus
-> SLIOTiming
-> SLInterruptController

### Register section

The architectural register section is a CISC named/split register set: x86-64 register model; 8-core AMD Zen 2 cluster with shared L3.

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

The SLeeLa representation is compositional: a Xbox Series X (x86-64 Zen 2) variant replaces only the couplers, I/O, timing, cache or opcode components that differ, rather than duplicating the whole CPU hierarchy.

## Sources

- https://news.xbox.com/en-us/2020/03/16/xbox-series-x-tech/
- https://en.wikipedia.org/wiki/Zen_2
