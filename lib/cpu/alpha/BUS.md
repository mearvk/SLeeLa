# DEC Alpha Bus

Alpha processors appeared in several system implementations, so no single physical system bus is treated as the universal Alpha bus.

SLeeLa normalizes the CPU boundary through SLIOBus and SLBusArbiter.

## Transaction

- physical/virtual address;
- read/write;
- transfer size;
- data;
- byte enables;
- memory attributes;
- ordering;
- coherency;
- response/error;
- master identity.

A concrete Alpha workstation/server profile can attach its chipset, memory controller, PCI/I/O bridge, or other interconnect.

DMA is modeled as a system-level bus master.
