# System/370 Timing and Performance

## Model-specific behavior

System/370 performance varies significantly by model, configuration, instruction, storage hierarchy, and I/O load. Do not publish one universal cycles-per-instruction value.

## Measurement dimensions

Track instruction execution, storage wait, translation overhead, interruption processing, and channel/device activity independently where the simulator supports them.

## Evidence classes

- `documented`: source data tied to a named model.
- `derived`: calculated from documented values and stated assumptions.
- `estimated`: simulation approximation.
- `unspecified`: no reliable value established.

## Reports

Include model identifier, architecture level, enabled facilities, storage configuration, and evidence class. Keep functional correctness distinct from cycle-accurate timing.

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
