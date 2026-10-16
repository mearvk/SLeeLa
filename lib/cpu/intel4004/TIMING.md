# Intel 4004 Timing

The baseline SLeeLa 4004 model does not claim cycle accuracy without a validated timing profile.

- **Functional mode:** deterministic instruction semantics and correct instruction-byte consumption.
- **Timed mode:** per-instruction timing metadata from a documented 4004 source.
- Two-byte instructions must account for both instruction bytes and the correct PC advance.
- Chipset access timing may be modeled separately when reliable data is available.
- Missing timing data is reported as unavailable; do not infer timing from later Intel processors.

Host execution speed is not guest processor timing.

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
