# Z80 Architecture

## Topology

SLZ80CPU -> SLClock -> SLRegisterFile -> SLPipeline (machine-cycle sequencer) -> SLInstructionFetch -> SLExecutionUnit -> SLIOBus -> SLIOTiming -> SLInterruptController

## Register organization

The Z80 has primary and alternate register sets. AF, BC, DE and HL can be exchanged with AF', BC', DE' and HL', allowing rapid context changes without memory traffic.

IX and IY provide indexed addressing.

## Control and sequencing

Instruction execution is divided into machine cycles. Each machine cycle contains T cycles. The first machine cycle is normally opcode fetch. External WAIT can lengthen operations.

## Bus domains

SLeeLa separates memory transactions, I/O transactions, interrupt acknowledge, refresh-related activity, and external wait/ready timing.

## Design principle

The Z80 profile reuses common SLeeLa CPU objects but does not force modern cache/MMU abstractions onto an architecture that does not have them.
