# Cortex-M Timing and Exceptions

## Timing policy

Timing depends on core variant, instruction mix, memory placement, wait states, bus contention, debug configuration, and optional extensions. The simulator should separate architectural correctness from cycle-accurate timing.

## Evidence classes

- `documented`: from the selected core and system documentation.
- `derived`: calculated from documented rules with assumptions stated.
- `estimated`: approximate model for planning or simulation.
- `unspecified`: insufficient target evidence.

## Sources of variable latency

- Flash/SRAM/peripheral wait states and bus arbitration.
- Branches, instruction fetch boundaries, and pipeline refill.
- Exception entry/return, stacking, lazy FPU context preservation, and tail-chaining when supported.
- Cache misses and maintenance where caches exist.
- DSP/FPU/MVE instructions according to the selected implementation.

## Interrupt behavior

Priorities and masking rules follow the configured NVIC/profile. Model architectural exception state precisely; model latency only to the evidence level available. Late arrival and tail-chaining are not universal timing constants.

## Output

Timing reports must identify profile, variant, clock, memory assumptions, enabled extensions, and evidence level. Do not publish a single family-wide cycles-per-instruction figure.

## Expert timing, voltages, and variable clocks (shared across all CPU models)

The per-CPU timing above is the architectural/behavioral view for this part.
The adjustable, quality-first timing system is shared by every CPU model in
`lib/cpu` and is documented once, in the package guide:

- [`lib/cpu/TIMING.md`](../../TIMING.md) — Voltages, Variable Clocks, and Expert
  Timings: `SLVoltageRail` (actual voltages in mV), `SLCpuTiming`,
  `SLBusTiming`, `SLRamTiming`, `SLIOTiming`, the Novice/Basic/Advanced/Expert
  `SLTimingClass` tiers, and the `SLTimingProfile` -> `SLTimingManifest` a
  compiled `.sleela` image carries so the compiler/VM honor the **declared**
  operating point.
- [`lib/cpu/components/README.md`](../../components/README.md) — running a CPU
  at spec and **overclocking** it (`SLClockPolicy` / `setSpecClock` /
  `overclock`), plus the RAM / storage / optical / system-bus parts bin and the
  DMA / GPU / native-RAM options.

Every CPU model (this one included) inherits the clock/overclock surface from
`SLCPURuntime` and can be assembled with those shared expert-timing, voltage,
and component options; they are a build-layer concern, not re-declared per CPU.
