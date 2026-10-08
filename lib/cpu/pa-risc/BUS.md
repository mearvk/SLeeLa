# PA-RISC Bus

PA-RISC systems used HP-specific processor, memory, and I/O system designs rather than one universal external CPU bus. citeturn0search2

SLeeLa presents a normalized interface through SLIOBus and SLBusArbiter.

## Transaction

- address;
- read/write;
- transfer size;
- data;
- byte enables;
- memory attributes;
- privilege;
- ordering;
- response/error;
- coherency;
- master identity.

Concrete HP system profiles can attach their processor chipset and I/O bus.

DMA is represented as an external/system bus master.
