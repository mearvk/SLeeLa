# PowerPC Cache

PowerPC does not imply one universal cache hierarchy.

The SLeeLa base model therefore exposes cache attachment points but does not assign generic sizes or associativity.

A concrete implementation may provide:

- separate instruction and data caches;
- unified caches;
- L2/L3;
- cache coherency;
- cache-control operations;
- tightly coupled or local memory.

The PowerPC VEA explicitly defines aspects of cache behavior and cache-control facilities, while implementation manuals specify the actual cache structures. citeturn0search20

For example, historical MPC603-family material documents on-chip instruction/data caches, demonstrating why cache belongs to the concrete CPU profile rather than the generic ISA object. citeturn0search8
