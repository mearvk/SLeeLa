# 6502 Architecture

## Topology

SL6502CPU

-> SLClock
-> SLRegisterFile
-> SLPipeline (cycle sequencer, not a modern pipeline)
-> SLInstructionFetch
-> SLExecutionUnit
-> SLIOBus
-> SLIOTiming
-> SLInterruptController

### Register section

The architectural register section contains:

- A
- X
- Y
- S
- P
- PC high/low

The physical implementation also uses internal latches and data/address-path elements. SLeeLa models those only where their observable timing or behavior is useful.

### Control section

The control side is represented as:

- instruction register;
- instruction decode;
- control/timing sequencer;
- interrupt logic;
- clock-phase coordination.

The original 6502 is associated with PLA-based direct instruction decoding rather than a conventional modern microcode engine.

### Datapath

The major conceptual datapath elements are:

registers -> internal bus/latches -> ALU -> register/memory destination

Address formation uses high/low address components and indexed addressing logic.

### Bus

The external interface exposes:

- 16 address lines;
- 8 data lines;
- read/write control;
- clock/phase signals;
- reset and interrupt controls.

Each observable memory transaction is modeled as a bus event.

## Design principle

The SLeeLa representation is intentionally compositional. A 6502 variant can replace only the couplers, I/O, timing, or opcode-specific components that differ rather than duplicating the entire CPU hierarchy.

Source: https://github.com/mamedev/mame/blob/master/docs/source/techspecs/m6502.rst
