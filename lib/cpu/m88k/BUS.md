# Motorola 88000 Bus

The 88000 family used implementation/system-specific processor and memory interconnects.

SLeeLa normalizes the CPU boundary through SLIOBus and SLBusArbiter.

## Transaction

- address;
- read/write;
- size;
- data;
- byte enables;
- memory attributes;
- privilege;
- ordering;
- coherency;
- response/error;
- master identity.

DMA is represented as an external/system bus master.
