# IBM POWER Bus

POWER system interconnect is modeled as a processor/system boundary rather than as a modern standardized CPU bus.

Transactions expose:

- address;
- operation;
- size;
- data;
- byte enables;
- memory attributes;
- privilege;
- ordering;
- response/error;
- master identity.

The RS/6000 system architecture may supply memory controllers, I/O bridges, DMA engines, and additional system logic outside the CPU.
