# Atari 7800 (6502C) Architecture

## Topology

SLAtari7800CPU

-> SLClock
-> SLRegisterFile
-> SLALU
-> SLInstructionDecoder
-> SLIOBus
-> SLIOTiming
-> SLInterruptController

### Register section

The architectural register section is a accumulator / indexed register machine: 6502-class model (A, X, Y, S, PC, P); MOS 6502C (SALLY).

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

Address formation uses the architectural 16-bit address path.

### Bus

The external interface exposes 16 address lines, a 8-bit data path, read/write control, clock/phase signals, and reset/interrupt controls. Each observable memory transaction is modeled as a bus event (see BUS.md).

## Design principle

The SLeeLa representation is compositional: a Atari 7800 (6502C) variant replaces only the couplers, I/O, timing, cache or opcode components that differ, rather than duplicating the whole CPU hierarchy.

## Sources

- https://en.wikipedia.org/wiki/Atari_7800
- https://en.wikipedia.org/wiki/MOS_Technology_6502
