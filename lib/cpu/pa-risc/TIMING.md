# PA-RISC Timing

## Architectural timing

The timing model records:

- fetch;
- decode;
- issue;
- integer execution;
- shift/bit operations;
- branch;
- load/store;
- FPU;
- cache;
- TLB;
- trap/interrupt;
- retirement.

PA-RISC 1.0 was fundamentally scalar, while later PA-RISC 1.1 processors became superscalar. PA-RISC 2.0 implementations such as PA-8000 reached four-way superscalar execution with out-of-order scheduling. citeturn0search1turn0search3

The generic implementation therefore does not claim one universal cycle count or pipeline depth.

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
