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
| `SLMotherboard` | the board | socket, chipset, memory channels, DIMM slots, PCIe lane budget + generation ⇒ what a CPU/RAM/card can seat on |
| `SLMemoryModule` | main memory | technology (DRAM…DDR5), capacity, bus width, clock, CAS latency, **channel count** ⇒ aggregate bandwidth + first-word latency |
| `SLMemoryInterface` | a cadence-overlay adapter over a module | target-tech **refresh duty cycle** (tREFI/tRFC) ⇒ run the real module so it *refreshes as though it were* another tech (e.g. DDR5 refreshing like DDR2); Sleela-layer only, backing store untouched |
| `SLStorageDevice` | floppy / HDD / SSD / NVMe / tape | interface, capacity, RPM, seek, transfer ⇒ access-time model |
| `SLOpticalDrive` | CD / DVD / Blu-ray | medium 1× base rate × speed multiplier ⇒ read rate; writer flag |
| `SLSystemBus` | ISA / PCI / AGP / PCIe / FSB | clock, width, lanes ⇒ peak bandwidth (can bottleneck fast RAM) |
| `SLComponentCatalog` | **named presets** | ready-configured real parts (see below) |
| `SLExecutorBuild` | the assembled machine | CPU + board + RAM + storage + optical + bus, mounted and queryable |

### Catalog presets (authentic specs)

- **Memory:** `sram_fast`, `dram_fpm`, `sdram_pc100`, `ddr_400`, `ddr2_800`,
  `ddr3_1600`, `ddr4_3200`, `ddr5_6000`, `ddr5_6400` (each takes a capacity in
  MiB). Channel-aware DDR5-6400 for the Threadripper platforms:
  `ddr5_6400_quadChannel` (4-ch HEDT) and `ddr5_6400_octaChannel` (8-ch PRO WX),
  each taking a per-module capacity and reporting N× capacity and N× bandwidth.
- **Storage:** `floppy144`, `mfmHdd`, `ideHdd`, `sataHdd`, `sataSsd`,
  `nvmeGen3`, `nvmeGen4`, `tapeLto`.
- **Optical:** `cdrom52`, `cdrw`, `dvd16`, `dvdrw`, `bluRay`, `bdxl`.
- **Bus:** `isa`, `pci33`, `agp8x`, `pcieGen3x16`, `pcieGen4x16`, `pcieGen5x16`,
  `fsb(mhz)`.
- **Motherboard:** `trx50` (Threadripper HEDT: sTR5, 4-channel, 88 PCIe 5.0
  lanes), `wrx90` (Threadripper PRO WX: sTR5, 8-channel, 128 PCIe 5.0 lanes).
- **Memory interface (cadence overlay):** `ddr2CadenceInterface(module)`,
  `ddr3CadenceInterface(module)` (and the general `memInterface(name, module,
  tech)`). Wrap a real module so it refreshes as though it were the target tech;
  install it alongside the module via `build.installMemoryInterface(...)`.

### Running a module under a different refresh cadence

An `SLMemoryInterface` is an OOD adapter that sits in front of the real module
and imposes a **refresh duty cycle** from a chosen technology — for example,
running an actual DDR5-6400 module so it refreshes with a DDR2 cadence, for a
special-purpose run. The cadence is modelled **entirely in the Sleela layer**
(the modelled controller / the VM's view of memory availability); it does not
touch the host OS, and it does not change the module's data rate or its backing
store. Only the observed availability changes: effective bandwidth = native peak
× available duty, where available duty = (tREFI − tRFC) / tREFI.

```
SLComponentCatalog parts = new SLComponentCatalog();
SLMemoryModule ddr5 = parts.ddr5_6400_quadChannel(32768);  // the real RAM
SLMemoryInterface asDdr2 = parts.ddr2CadenceInterface(ddr5);

SLExecutorBuild build = new SLExecutorBuild();
build.configure("ddr5-as-ddr2");
build.installMemory(ddr5);                 // actual module (backing store stays DDR5)
build.installMemoryInterface(asDdr2);      // AND the cadence overlay — compose both
build.assemble();

build.memoryBandwidthMBs();   // duty-scaled: native peak × availableDutyPct()
asDdr2.disengage();           // pass the real DDR5 through untouched
```

See `examples/ddr5-as-ddr2-cadence.sleela` for the full A/B flow.

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

## Related VM-level options

Three capabilities toggled when the VM Creator builds the Secondary VM
(`lib/vm/creator/SleelaVMCreator`):

- `requestDMA()` — direct memory-access engine (bulk moves without the CPU loop).
- `requestGPU()` — native GPU access (offload compute kernels; host GPU over the
  bridge when present, else a modelled SIMT engine).
- `requestNativeRam()` — run the VM's linear memory on **real host RAM**
  (`SLNativeMemory`) instead of the simulated `SLRAM` store, for running outside
  simulation for procedural layment. Corresponds to the `NATIVE_RAM` feature bit;
  see `lib/vm/creator/examples/native-ram-vm.sleela`.
