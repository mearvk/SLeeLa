# Intel 4004 Chipset and Bus Model

The 4004 uses a multiplexed 4-bit data interface and external support chips.

- Program storage is modeled separately from data RAM.
- 4001-style ROM/I/O and 4002-style RAM/I/O devices may be configured as chipset modules.
- Instruction-byte fetches, data accesses, and I/O operations must preserve their distinct semantics.
- Unknown chip selections or unsupported device commands return structured errors and diagnostics.
- The 4004's physical interface must not be represented as a modern PCI-style bus.

Chip count, memory capacity, and device mapping are configuration values; do not assume every system used an identical chipset population.