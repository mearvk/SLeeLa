# SLeeLa Executor Components

Build-your-own hardware for a Sleela executor. These classes let a user run a
CPU model **at its own specification**, **overclock** it, and compose it with
real-world-spec'd **RAM, storage, optical drives, and a system bus** to form a
complete executor — which the Creator/SLVM then hosts and runs C/C++/Java/Sleela
programs on.

## Running at spec, and overclocking (CPU side, `lib/cpu`)

Every CPU model now carries its rated clock and can be pushed beyond it:

- `SLClockPolicy` — spec clock as `baseClock × multiplier`, a stability ceiling,
  and overclock operations (`overclockTo`, `setMultiplier`, `setBaseClock`).
- `SLCPURuntime` wires it in. A concrete model sets its rated clock in
  `configure()` (`setSpecClock(ratedMHz)`), so it runs at spec by default:
  - `cpu.specClockMHz()` — the rated clock.
  - `cpu.overclock(targetMHz)` — push higher; returns `false` (but still applies)
    if above the model's stability ceiling. `cpu.clockStable()` reports it.
  - `cpu.effectiveMHz()` / `cpu.overclocked()` — the current state.
  - `cpu.resetClock()` — back to spec.

Effective clock scales throughput linearly up to the stability ceiling, which is
exactly the lever behind the speed/feasibility tables in
`../PERFECT.CONSEQUENCE.md`.

## The parts bin (`lib/cpu/components`)

| Class | What you pick | Key spec it carries |
| --- | --- | --- |
| `SLMemoryModule` | main memory | technology (DRAM…DDR5), capacity, bus width, clock, CAS latency ⇒ bandwidth + first-word latency |
| `SLStorageDevice` | floppy / HDD / SSD / NVMe / tape | interface, capacity, RPM, seek, transfer ⇒ access-time model |
| `SLOpticalDrive` | CD / DVD / Blu-ray | medium 1× base rate × speed multiplier ⇒ read rate; writer flag |
| `SLSystemBus` | ISA / PCI / AGP / PCIe / FSB | clock, width, lanes ⇒ peak bandwidth (can bottleneck fast RAM) |
| `SLComponentCatalog` | **named presets** | ready-configured real parts (see below) |
| `SLExecutorBuild` | the assembled machine | CPU + RAM + storage + optical + bus, mounted and queryable |

### Catalog presets (authentic specs)

- **Memory:** `sram_fast`, `dram_fpm`, `sdram_pc100`, `ddr_400`, `ddr2_800`,
  `ddr3_1600`, `ddr4_3200`, `ddr5_6000` (each takes a capacity in MiB).
- **Storage:** `floppy144`, `mfmHdd`, `ideHdd`, `sataHdd`, `sataSsd`,
  `nvmeGen3`, `nvmeGen4`, `tapeLto`.
- **Optical:** `cdrom52`, `cdrw`, `dvd16`, `dvdrw`, `bluRay`, `bdxl`.
- **Bus:** `isa`, `pci33`, `agp8x`, `pcieGen3x16`, `pcieGen4x16`, `fsb(mhz)`.

## Building and running an executor

```
SLComponentCatalog parts = new SLComponentCatalog();

SL6502CPU cpu = new SL6502CPU(); cpu.configure();   // runs at its 1 MHz spec

SLExecutorBuild build = new SLExecutorBuild();
build.configure("my-rig");
build.installCpu(cpu);
build.installMemory(parts.ddr4_3200(16384));   // upgrade the RAM
build.installStorage(parts.nvmeGen4());
build.installOptical(parts.bluRay());
build.installBus(parts.pcieGen4x16());
build.setClock(14);                             // overclock the CPU to 14 MHz
build.assemble();                               // mounts the RAM into the CPU

// Host the built executor's CPU on an SLVM and run a C/C++/Java/Sleela program:
SleelaVMCreator creator = new SleelaVMCreator(); creator.configure();
SLWorkload job = new SLWorkload(); job.configure("app", "app.c", job.LANG_C, grant);
int out = creator.run(5, build.processor(), job, 1000000);
```

The CPU runs at its chosen (spec or overclocked) frequency; the RAM/storage/
optical/bus choices determine the memory bandwidth and access-time behavior of
the machine. See `examples/build-your-executor.sleela` for the full flow.
