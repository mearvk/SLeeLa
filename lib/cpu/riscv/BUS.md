# RISC-V Bus

RISC-V is an ISA, not a single physical system bus.

SLeeLa's CPU boundary uses SLIOBus and SLBusArbiter and can attach an implementation-specific SoC/interconnect.

## Transaction model

- address;
- read/write;
- size;
- data;
- byte enables;
- memory attributes;
- privilege/security state;
- ordering;
- response/error;
- coherency;
- master identity.

## DMA

DMA controllers are system/SoC components and may act as independent bus masters. SLDMAController connects through SLBusArbiter and the selected memory/interconnect model.

A concrete RISC-V platform may use AXI, TileLink, AHB, or another fabric; the generic CPU object does not claim one.
