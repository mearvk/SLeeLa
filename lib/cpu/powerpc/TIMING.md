# PowerPC Timing

## Timing model

PowerPC implementations differ substantially in pipeline depth, dispatch width, execution latency, cache latency, and completion behavior.

SLeeLa therefore records timing as implementation data rather than asserting a universal cycle count.

The timing record can include:

- fetch cycle;
- decode/dispatch;
- execution unit;
- dependency/issue wait;
- cache hit/miss;
- memory transaction;
- branch result;
- completion;
- exception/interrupt;
- bus arbitration.

## Fixed instruction size

Instructions are 32-bit words, giving a stable fetch/decode granularity. citeturn0search9

## Branch timing

Branch processing is modeled independently because CR/CTR/LR state can affect control flow. Conditional branch forms may also update LR when LK is set. citeturn0search11

## Accuracy boundary

A PowerPC 601-style timing model must not be presented as a 604/750/970 timing model. Each concrete implementation gets its own timing profile.

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
