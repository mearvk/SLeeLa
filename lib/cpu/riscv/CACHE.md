# RISC-V Cache

RISC-V defines the ISA interface while leaving cache hierarchy and many memory-system details to implementations.

SLeeLa therefore models:

- optional L1 instruction cache;
- optional L1 data cache;
- optional unified L2;
- optional L3;
- TLBs;
- page-table-walk structures;
- coherency/interconnect coupling.

No generic RISC-V cache size, associativity, line size, replacement policy, or coherence protocol is invented.

Cache maintenance, ordering, and coherence are connected to the selected platform and extension/profile.
