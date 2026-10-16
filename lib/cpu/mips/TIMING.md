# MIPS Timing

## Pipeline timing

The classic SLeeLa abstraction uses IF, ID, EX, MEM, and WB stages. This gives a useful cycle-level model while avoiding the false claim that every MIPS processor has identical physical pipeline timing.

## Branches

Classic MIPS has a branch delay slot. The instruction immediately following a branch/jump is architecturally executed in the classic model. citeturn0search20

## Loads

A load has address-generation, memory-access, and register-writeback timing. Concrete implementations may add stalls or forwarding constraints.

## Exceptions

Exceptions and interrupts can redirect control flow and alter normal pipeline progression. CP0 state participates in that transition.

## Timing evidence

Record stage, instruction PC, operand dependencies, memory request/response, stall/forward decision, branch-delay state, and exception state.

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
