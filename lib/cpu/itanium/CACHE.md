# Intel Itanium Cache

Itanium implementations used different cache organizations across generations.

SLeeLa exposes:

- instruction cache;
- data cache;
- optional unified/intermediate cache;
- higher-level cache;
- TLB;
- translation structures;
- coherency/interconnect.

No generic Itanium cache size, associativity, line size, or replacement policy is invented.

IA-64's memory ordering, speculation, and cache behavior are represented as architectural/system properties with implementation-specific cache geometry.
