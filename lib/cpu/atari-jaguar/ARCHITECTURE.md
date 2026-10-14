# Atari Jaguar (68000 + Tom/Jerry RISC) Architecture

## Topology

SLAtariJaguarCPU

-> SLClock
-> SLRegisterFile
-> SLALU
-> SLInstructionDecoder
-> SLIOBus
-> SLIOTiming
-> SLInterruptController

### Register section

The architectural register section is a CISC named/split register set: 68000 register model (D0-D7, A0-A7, PC, SR) as the manager CPU plus Tom/Jerry RISC cores.

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

Address formation uses the architectural 24-bit address path.

### Bus

The external interface exposes 24 address lines, a 32-bit data path, read/write control, clock/phase signals, and reset/interrupt controls. Each observable memory transaction is modeled as a bus event (see BUS.md).

## Design principle

The SLeeLa representation is compositional: a Atari Jaguar (68000 + Tom/Jerry RISC) variant replaces only the couplers, I/O, timing, cache or opcode components that differ, rather than duplicating the whole CPU hierarchy.

## Sources

- https://en.wikipedia.org/wiki/Atari_Jaguar
- http://atarijaguar.freeservers.com/hardware.htm
