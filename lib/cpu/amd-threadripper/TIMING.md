# AMD Ryzen Threadripper 9000 (Zen 5 x86-64) Timing

## Clock model

The model treats the clock as an event source rated at about 5400 MHz max boost, not as "one instruction per clock." Per-SKU base clocks (e.g. ~3.2 GHz on the 9980X, 4.0 GHz on the 9970X, 4.2 GHz on the 9960X) are configurations of the same model via setSpecClock.

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

- https://en.wikipedia.org/wiki/Zen_5
- https://en.wikipedia.org/wiki/Ryzen_Threadripper
- https://en.wikipedia.org/wiki/List_of_AMD_Ryzen_processors

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

## Threadripper 9000 ancillary presets (board, RAM timings, voltages)

The shared layer is wired to this family's actual operating point in
[`SLAMDThreadripperPlatform`](SLAMDThreadripperPlatform.sleela). It hands back
ready-configured objects so you can build a rig **at spec** without re-entering
raw numbers:

- **DDR5-6400 RAM timings** — `ddr5_6400Timing(name)` returns an `SLRamTiming`
  at JEDEC primaries (CL46-46-46, tRAS 104, tRC 150, 2T) on the Vddq rail, with
  `firstWordLatencyNs()` / `randomAccessLatencyNs()` derived and `honored()`
  enforcing the row invariants.
- **Voltage rails** (actual mV) — `vcore()` (nominal 1200 mV, 950–1450),
  `vsoc()` (1200 mV, 1050–1300, AMD's SoC cap), `vddq()` (DDR5's 1100 mV,
  1067–1166).
- **Boards** — `hedtBoard()` = TRX50 (sTR5, 4-channel, 88 PCIe 5.0 lanes),
  `proBoard()` = WRX90 (sTR5, 8-channel, 128 PCIe 5.0 lanes).
- **Populated memory** — `hedtMemory(capPerModule)` (4-channel DDR5-6400),
  `proMemory(capPerModule)` (8-channel DDR5-6400); `pcie5x16()` for the bus.

A complete assembly (CPU + board + DDR5-6400 + rails, HEDT and PRO WX) is shown
in
[`components/examples/threadripper-at-spec.sleela`](../components/examples/threadripper-at-spec.sleela).
