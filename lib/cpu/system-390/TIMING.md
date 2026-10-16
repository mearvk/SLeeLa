# ESA/390 Timing and Performance

## Model-specific timing

ESA/390 is an architecture level implemented by multiple System/390 models. Timing is model-, instruction-, storage-, and configuration-dependent; there is no single universal cycles-per-instruction value.

## Metrics

Where supported, report instruction execution, translation overhead, storage wait, interruption handling, and channel/device activity separately.

## Evidence classes

- `documented`: published for the named model.
- `derived`: computed from documented data and explicit assumptions.
- `estimated`: approximate simulation value.
- `unspecified`: no reliable value established.

## Reporting

Include model, architecture level, addressing mode, facilities, storage configuration, and evidence class. Do not describe a functional simulator as cycle-accurate without validation.

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
