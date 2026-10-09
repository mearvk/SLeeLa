# SLeeLa ARM Cortex-R CPU Family

**Path:** `/lib/cpu/cortex-r/`  
**Model status:** Profile-driven architectural foundation; not a cycle-accurate silicon emulator.

## Purpose

Cortex-R processors target real-time and safety-oriented embedded systems. This SLeeLa family model keeps core architecture, implementation-specific features, and SoC peripherals separate. A named core profile is required before instruction execution or timing estimates.

## Profile coverage

Profiles and feature sets must be selected from the actual documented core implementation. Cortex-R generations differ in instruction set, memory protection, cache and TCM options, floating-point/DSP features, virtualization support, and exception behavior. Do not infer all features from the Cortex-R name alone.

The implementation must distinguish supported architecture generations and core variants explicitly; unsupported profiles fail configuration rather than falling back silently to a different core.

## Core responsibilities

- Instruction decode and execution for the selected instruction-set profile.
- General-purpose and special-register state, status flags, and exception modes.
- Profile-specific exception, interrupt, and fault behavior.
- Memory protection and access validation.
- Optional caches, TCM, FPU/DSP, debug, and virtualization features where implemented.
- Configurable memory-system and SoC integration.

## Real-time modeling

Determinism depends on core configuration, memory placement, TCM/cache use, bus contention, interrupts, and device behavior. The model reports timing with evidence labels (`documented`, `derived`, `estimated`, `unspecified`) and does not claim universal worst-case execution times.

## Separation of concerns

Interrupt controllers, DMA engines, safety monitors, timers, flash controllers, and peripheral fabrics may be core-integrated interfaces or SoC components depending on the target. Model only the documented configuration.
