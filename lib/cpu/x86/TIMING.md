# x86 / x86-64 Timing

## Architectural versus microarchitectural timing

x86 instruction latency and throughput vary significantly between generations and implementations. Intel publishes separate optimization and microarchitecture material rather than one universal timing table. citeturn0search7

SLeeLa timing records therefore include:

- instruction bytes;
- decode length;
- decoded operation count;
- execution domain;
- issue/dispatch;
- dependencies;
- execution latency;
- throughput;
- load/store latency;
- cache hit/miss;
- TLB hit/miss;
- branch prediction/result;
- retirement/completion;
- exception/interrupt;
- serialization.

## Legacy timing

Older x86 implementations can use a more direct bus/cycle model.

## Modern timing

Modern implementations may include decode queues, micro-op caches, out-of-order execution, speculative execution, multiple execution ports, and retirement. These are concrete implementation properties, not universal ISA requirements.

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
