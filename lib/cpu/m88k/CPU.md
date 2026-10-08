# Motorola 88000 CPU

SLeeLa models Motorola's 88000 (m88k) as a 32-bit RISC architecture family, with MC88100 and MC88110 kept as distinct microarchitectural profiles.

## Composition

SLM88KCPU composes instruction fetch/decode, general and control registers, integer/branch execution, floating-point execution, load/store, pipeline/scoreboard, MMU/TLB, cache hierarchy, trap/interrupt control, and system interconnect.

## Architectural boundary

The MC88100 and MC88110 differ substantially in cache integration, superscalar behavior, branch handling, and system organization. Those differences remain implementation profiles.
