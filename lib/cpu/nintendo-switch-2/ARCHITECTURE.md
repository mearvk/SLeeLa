# Nintendo Switch 2 (ARM Cortex-A78C) Architecture

## Topology

SLSwitch2CPU

-> SLClock
-> SLRegisterFile
-> SLALU
-> SLInstructionDecoder
-> SLIOBus
-> SLIOTiming
-> SLInterruptController

### Register section

The architectural register section is a RISC general-purpose register file: AArch64 register model; custom NVIDIA Tegra SoC with ARM Cortex-A78C-class cluster.

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

The SLeeLa representation is compositional: a Nintendo Switch 2 (ARM Cortex-A78C) variant replaces only the couplers, I/O, timing, cache or opcode components that differ, rather than duplicating the whole CPU hierarchy.

## Sources

- https://en.wikipedia.org/wiki/Nintendo_Switch_2
- https://en.wikipedia.org/wiki/ARM_Cortex-A78
