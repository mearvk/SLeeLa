# Renesas SuperH CPU

SLeeLa models the SuperH (SH) family as a 32-bit RISC architecture family with distinct SH-1, SH-2, SH-3, and SH-4 implementation profiles.

Renesas documents SH-4 as a 32-bit RISC processor with upward object-code compatibility from SH-1 through SH-3, 16-bit fixed-length instructions, load/store operation, delayed branches, and a five-stage pipeline. citeturn1search20

## Profiles

- SH-1
- SH-2 / SH-2E
- SH-3 / SH-3E / SH3-DSP
- SH-4 / SH-4A
- SH2A / SH2A-FPU

The generic model does not force later MMU, cache, FPU, or DSP resources onto earlier SH implementations.
