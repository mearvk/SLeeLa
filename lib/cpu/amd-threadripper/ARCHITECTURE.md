# AMD Ryzen Threadripper 9000 (Zen 5 x86-64) Architecture

## Topology

SLAMDThreadripperCPU

-> SLClock
-> SLRegisterFile
-> SLALU
-> SLInstructionDecoder
-> SLIOBus
-> SLIOTiming
-> SLInterruptController

### Register section

The architectural register section is a CISC named/split register set: x86-64 register model (Zen 5 microarchitecture) with per-core L1/L2 and shared L3 per CCD, aggregated across multiple CCDs on a central I/O die.

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

The external interface exposes 48 address lines, a 64-bit data path, read/write control, clock/phase signals, and reset/interrupt controls. On the physical part, off-die I/O is DDR5 (4-channel HEDT, 8-channel PRO) plus PCIe 5.0. Each observable memory transaction is modeled as a bus event (see BUS.md).

## Design principle

The SLeeLa representation is compositional: an AMD Ryzen Threadripper 9000 variant replaces only the couplers, I/O, timing, cache or opcode components that differ, rather than duplicating the whole CPU hierarchy. The HEDT (X) and workstation (PRO WX) sub-families differ in memory channels, PCIe lane count and maximum core/CCD count, not in the core programmer model.

## Sources

- https://en.wikipedia.org/wiki/Zen_5
- https://en.wikipedia.org/wiki/Ryzen_Threadripper
- https://en.wikipedia.org/wiki/List_of_AMD_Ryzen_processors
