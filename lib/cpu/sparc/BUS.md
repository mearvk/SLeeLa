# SPARC Bus

SPARC defines processor architecture rather than one mandatory physical system bus.

SLeeLa exposes the processor through SLIOBus and SLBusArbiter.

## Transaction

- address;
- read/write;
- size;
- data;
- byte enables;
- memory attributes;
- privilege;
- response/error;
- ordering;
- coherency;
- master identity.

A concrete SPARC implementation can attach its processor/system bus and memory controller without changing the generic ISA model.

DMA is represented as an external or integrated system master.
