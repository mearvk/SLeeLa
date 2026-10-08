# ARM Cache

ARM architecture supports implementation-specific cache hierarchies rather than one universal cache size or topology.

SLeeLa exposes:

- L1 instruction cache;
- L1 data cache;
- optional unified cache;
- L2;
- L3;
- TLBs;
- translation-table walk caches;
- coherency/interconnect coupling.

Arm's AMBA AXI documentation separates processor/cache masters from memory/interconnect infrastructure, while Cortex-A documentation describes concrete cache and memory-system implementations. citeturn0search22turn0search23

No cache size, associativity, line size, replacement policy, or coherency protocol is invented for generic ARM.
