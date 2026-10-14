# System/360 Timing and Performance

## Model-specific timing

There is no single cycle count or execution rate for the System/360 family. Timing depends on the machine model, instruction, operand location, installed options, storage technology, channel activity, and system load.

## Evidence labels

- `documented`: published for a specific model.
- `derived`: calculated from documented data and explicit assumptions.
- `estimated`: approximate simulation value.
- `unspecified`: reliable model-specific evidence is unavailable.

## Performance dimensions

Track instruction execution, storage waits, channel activity, interruption overhead, and device waits separately. Do not include unmodeled operating-system or I/O time as intrinsic CPU latency.

## Reports

Identify the machine model, options, storage configuration, and evidence class. Do not present estimates as guaranteed timing.

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
