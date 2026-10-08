# ARM Bus

## Architectural boundary

ARM processor cores connect to implementation-specific memory and peripheral systems. Modern SoCs commonly use AMBA-family interconnects, but the CPU architecture itself should not be equated with one physical bus.

SLeeLa represents the boundary through SLIOBus and SLBusArbiter.

## Transaction fields

- virtual address;
- physical address;
- read/write;
- transfer size;
- byte enables;
- data;
- memory attributes;
- privilege/security state;
- ordering;
- response/error;
- coherency;
- master identity.

## AXI/AMBA coupling

A concrete ARM SoC may expose AXI or another AMBA interface. The generic CPU object does not claim a particular AMBA revision; an SoC-specific profile can attach the appropriate bus implementation.

## DMA

External or integrated DMA is modeled as another bus master through SLBusArbiter.
