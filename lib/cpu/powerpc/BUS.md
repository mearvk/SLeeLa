# PowerPC Bus

## Architectural boundary

PowerPC defines memory/storage behavior at the architecture level, while the physical processor bus varies by implementation.

SLeeLa therefore separates the CPU core from the system bus through SLIOBus and SLBusArbiter.

## Transfer model

The bus abstraction carries:

- effective/physical address;
- read/write;
- transfer size;
- byte enables;
- data;
- response/wait;
- exception/error;
- ownership/arbitration;
- coherency attributes when implemented.

## Load/store

PowerPC's fixed-point load/store instructions form the principal path between GPRs and storage. Floating-point loads/stores use FPRs where the implementation supports them.

## External masters

DMA and other bus masters use SLBusArbiter and are not modeled as hidden CPU execution units.
