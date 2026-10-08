# x86 / x86-64 Bus

## Architectural boundary

Modern x86 does not expose one universal external processor bus across all generations. Older processors, chipsets, sockets, integrated memory controllers, PCIe root complexes, and SoC designs differ.

SLeeLa therefore separates:

- core/interconnect transactions;
- memory-controller interface;
- device/MMIO interface;
- external DMA;
- interrupt delivery.

## Transaction fields

The SLeeLa bus abstraction records:

- linear address;
- physical address;
- read/write;
- width;
- byte enables;
- data;
- memory type;
- cacheability;
- privilege;
- ordering;
- completion/error;
- bus-master identity.

## DMA

DMA is a system-level bus-master function and is not inserted into the architectural x86 instruction core.
