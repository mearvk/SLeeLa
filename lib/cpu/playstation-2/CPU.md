# SLeeLa CPU Profile — PlayStation 2

**Library:** `/lib/cpu/playstation-2`  
**Model:** PlayStation 2  
**Era:** 2000

## CPU

- **CPU:** 128-bit Emotion Engine (MIPS-derived)
- **Clock:** 294.912 MHz
- **Memory:** 32 MB RDRAM; 4 MB eDRAM graphics memory
- **Launch price (US):** $299 launch (US)
- **Architecture / notes:** Emotion Engine with vector units; Graphics Synthesizer; DVD/CD media

## SLeeLa Representation

This profile records the historical console CPU/platform characteristics for SLeeLa's CPU library. It is a **hardware reference model**, not a claim that SLeeLa executes the original proprietary console firmware or software.

The model may be used by SLeeLa tooling for architecture comparison, emulation-oriented studies, historical performance profiles, memory budgeting, instruction-family research, and platform-aware workload simulation.

## Memory Model

The RAM figures above distinguish the principal system memory from graphics or auxiliary memory where the original platform used separate pools. SLeeLa should preserve those distinctions when a platform-aware simulation is requested.

## Cost Model

The launch price is the historical US launch MSRP and is included as a reference value. It is not a current market valuation.

## Compatibility Intent

The profile is suitable for:

1. CPU/platform inventory and documentation.
2. SLeeLa architecture and instruction-set research.
3. VM or emulator research where a separate implementation supplies the actual execution semantics.
4. Historical hardware comparison.
5. Game or simulation workload estimation.

## Sources / Accuracy Note

Specifications are based on widely documented manufacturer-era hardware specifications and launch information. Some historical console specifications have platform-specific nuances; this document intentionally records the principal CPU and memory figures rather than every coprocessor, cache, bus, or revision.
