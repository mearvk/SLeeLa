# Motorola 68020 CPU

SLeeLa models the Motorola MC68020 as the first major 68000-family implementation in this CPU series with a full 32-bit address bus, on-chip instruction cache, and a substantially expanded instruction/addressing architecture.

The 68020 remains a 32-bit CISC processor while preserving the 68000 programming model and exception architecture.

## Architectural profile

- 32-bit data path
- 32-bit address bus
- 32-bit programmer-visible registers
- eight data registers and eight address registers
- PC and SR
- on-chip instruction cache
- three-stage conceptual pipeline
- dynamic bus sizing
- 32-bit external data bus
- coprocessor interface
- virtual-memory/MMU boundary is implementation/system dependent

The 68020 should not be conflated with the later 68030, whose MMU and cache organization are materially different.
