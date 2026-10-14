# Intel Pentium Timing

## Clock model

The model treats the clock as an event source rated at about 100 MHz, not as "one instruction per clock."

### Timing records

Each cycle can record:

- cycleNumber
- phase
- address
- readWrite
- data
- waitState
- interruptState
- completion

## Instruction timing

Instruction duration depends on addressing mode, operand transfers, read/write sequence, branch result, and interrupt/wait state. Therefore:

instruction latency != clock period

and

instruction cycles * clock period = approximate elapsed instruction time

when no external wait state is inserted.

## Circuit timing

Internal datapath transfers are clock-sensitive. SLeeLa records this as observable/derived timing and does not claim an exact transistor simulation.

## Sources

- https://en.wikipedia.org/wiki/Pentium

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
