# Inmos Transputer Cache

Original Transputer implementations emphasize local memory and communication resources rather than a universal modern cache hierarchy.

SLeeLa therefore models:

- local memory;
- memory controller/interface;
- optional implementation-specific cache resources;
- link/channel access.

No universal L1/L2/L3 hierarchy is assigned to all Transputers.
