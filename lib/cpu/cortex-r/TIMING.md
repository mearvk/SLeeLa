# Cortex-R Timing and Real-Time Analysis

## No universal cycle count

Cycle timing depends on the exact core, instruction mix, cache/TCM configuration, memory placement, bus contention, interrupts, and peripheral behavior. A generic Cortex-R profile cannot guarantee worst-case execution time.

## Evidence levels

- `documented`: published for the selected core or SoC.
- `derived`: computed from documented facts and explicit assumptions.
- `estimated`: approximate simulation value.
- `unspecified`: insufficient target evidence.

## Sources of variability

- Cache hits, misses, line fills and write-buffer behavior.
- TCM versus external memory access.
- Flash and SRAM wait states.
- Interconnect arbitration and DMA traffic.
- Exception entry/return and interrupt nesting.
- FPU/DSP instructions and optional extensions.
- Debug, trace, and implementation-specific stalls.

## Analysis report

Every timing report identifies the exact core profile, clock, memory regions, cache and TCM configuration, interrupt assumptions, bus model, and evidence class. Distinguish average latency from a justified upper bound. Never label an estimate as a guaranteed WCET.

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
