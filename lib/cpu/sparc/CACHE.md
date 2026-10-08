# SPARC Cache

SPARC architecture does not prescribe one universal cache hierarchy.

SLeeLa models:

- optional L1 instruction cache;
- optional L1 data cache;
- optional L2;
- optional L3;
- TLB;
- translation structures;
- coherency/interconnect.

UltraSPARC documentation explicitly distinguishes architectural requirements from implementation dependencies. citeturn0search22

No generic cache size, associativity, line size, replacement policy, or coherence protocol is invented.
