# Motorola 88000 Cache

Cache architecture differs between 88000 implementations.

The MC88100 system used external cache resources, while the MC88110 integrated instruction/data cache resources on the processor.

SLeeLa exposes:

- instruction cache;
- data cache;
- optional unified higher-level cache;
- TLB;
- translation structures;
- coherency/interconnect.

No single cache size, associativity, line size, or replacement policy is assigned to generic 88000.
