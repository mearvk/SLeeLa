# Motorola 68000 Bus

## External organization

| Resource | Width |
|---|---:|
| Address | 24-bit |
| Data | 16-bit |
| Operand/program state | 32-bit |

The external bus model includes address, data, read/write, size, function-code, bus-status, interrupt-acknowledge, reset, and arbitration conditions.

## Cycle classes

SLeeLa distinguishes byte read, word read, byte write, word write, read-modify-write, interrupt acknowledge, and bus arbitration.

The NXP M68000 manual documents separate timing diagrams and flowcharts for these operations.

## Bus ownership

External DMA or another bus master is coupled through SLBusArbiter; the base CPU is not given an invented internal DMA engine.
