# PlayStation 5 (x86-64 Zen 2) Timing

## Clock model

The model treats the clock as an event source rated at about 3500 MHz, not as "one instruction per clock."

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

Instruction duration depends on addressing mode, operand transfers, read/write sequence, branch result, and interrupt/wait state. Therefore:

instruction latency != clock period

and

instruction cycles * clock period = approximate elapsed instruction time

when no external wait state is inserted.

## Circuit timing

Internal datapath transfers are clock-sensitive. SLeeLa records this as observable/derived timing and does not claim an exact transistor simulation.

## Sources

- https://www.cpu-monkey.com/en/cpu-sony_playstation_5
- https://en.wikipedia.org/wiki/Zen_2
