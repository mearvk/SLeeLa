# Z80 Timing

## M and T cycles

The Z80 timing model is explicitly cycle based:

- T = individual clock period.
- M = machine cycle made from multiple T cycles.
- M1 = normal instruction-fetch cycle.

Zilog documents basic memory and I/O operations as taking multiple T cycles and permits WAIT to lengthen them.

## SLeeLa timing record

cycle = { machine, t, operation, address, data, wait, phase }

This lets the model distinguish clock speed, instruction latency, and bus transaction latency.

## External synchronization

WAIT is represented as an external timing condition rather than an assumed fixed delay. This is important for period-correct emulation and for modeling slower memory/peripherals.

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
