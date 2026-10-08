# PA-RISC Cache

Cache organization is implementation-specific.

Examples range from early PA-RISC processors with external/cache-chip designs to later PA-8x00 processors with substantial on-chip cache implementations. PA-8000 itself initially used no on-chip caches, while later PA-RISC 2.0 processors integrated larger caches. citeturn0search3turn0search1

SLeeLa exposes:

- instruction cache;
- data cache;
- optional unified higher-level cache;
- TLB;
- translation structures;
- coherency/interconnect.

No single cache size or topology is assigned to generic PA-RISC.
