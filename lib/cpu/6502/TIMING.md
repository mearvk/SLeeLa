# 6502 Timing

## Clock model

The 6502 uses a two-phase clocking model. SLeeLa treats the clock as an event source rather than assuming that one instruction equals one clock.

### Timing records

Each cycle can record:

- cycleNumber
- phase
- address
- readWrite
- data
- waitState
- interruptState
- completion

## Instruction timing

Instruction duration depends on:

- addressing mode;
- number of operand bytes;
- read/write sequence;
- branch result;
- page crossing;
- interrupt/reset state.

Therefore:

instruction latency != clock period

and

instruction cycles * clock period = approximate elapsed instruction time

when no external wait state is inserted.

## Circuit timing

The internal datapath uses clock-phase-sensitive transfers. Public transistor-level analyses show latches and buses whose values become effective on particular clock phases.

SLeeLa records this as observable/derived timing and does not claim an exact transistor simulation.

Sources:
- https://tinymachines.ai/6502/archive/wiki/6502_datapath.html
- https://github.com/mamedev/mame/blob/master/docs/source/techspecs/m6502.rst
