# Intel Itanium Bus

Itanium systems used platform-specific processor and system interconnects.

SLeeLa normalizes the CPU boundary through SLIOBus and SLBusArbiter.

## Transaction state

- address;
- read/write;
- transfer size;
- data;
- byte enables;
- memory attributes;
- ordering;
- speculation/check status;
- coherency;
- response/error;
- master identity.

DMA is modeled as an independent system bus master.

## IA-32 compatibility

An IA-32 compatibility profile may attach a separate x86 execution subsystem and translation boundary rather than altering the IA-64 decoder.
