# DEC Alpha Cache

Alpha cache hierarchy is implementation-specific.

Examples include the EV4/EV5/EV6 families' differing instruction/data cache organizations and later integrated cache designs.

SLeeLa exposes:

- L1 instruction cache;
- L1 data cache;
- optional L2;
- optional L3;
- TLB;
- translation-walk state;
- coherency/interconnect.

No single Alpha cache size, associativity, line size, or replacement algorithm is imposed on the architecture.

## Memory ordering

Cache and memory operations connect to an explicit ordering/coherency layer so Alpha-specific ordering behavior can be modeled without assuming a modern generic memory model.
