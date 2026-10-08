# SuperH Cache

Cache support is profile-specific.

- SH-1/SH-2: no generic cache is assumed.
- SH-3 and later: cache/MMU resources are modeled where implemented.
- SH-4: separate instruction and operand/data caches are supported.

Renesas documents SH-4 configurable instruction/data cache organizations and separate I-cache/D-cache blocks. citeturn1search23

SLeeLa therefore exposes I-cache, D-cache, cache control, TLB, and memory-interface components without assigning one universal geometry to all SuperH CPUs.
