# Z80 Bus

## Primary external signals

The Z80 exposes a 16-bit address path and 8-bit data path together with control signals for memory, I/O, read/write, refresh, interrupt acknowledge, bus request/acknowledge, reset, wait, and clock.

## SLeeLa transaction model

A transaction records M-cycle, T-cycle, operation type, address, data, direction, wait state, bus ownership, and completion.

The model distinguishes memory operations from I/O operations and interrupt acknowledge.

## Bus ownership

External bus request behavior can be coupled to SLBusArbiter. A separate Z80 DMA peripheral is not treated as an internal CPU unit. Zilog documents the Z80 DMA as a separate bus-controlling peripheral.
