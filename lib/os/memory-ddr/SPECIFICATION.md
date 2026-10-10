# DDR SDRAM — Specification (memory-ddr)

Double Data Rate Synchronous DRAM transfers data on both the rising and falling
edges of the clock. The family is standardized by JEDEC and advances in discrete
generations; `lib/os/memory-ddr` models that finite set by name.

## Generations

| Generation | Representative module standard | Data rate (MT/s) | Typical VDD | Notes |
|---|---|---:|---:|---|
| DDR  | PC-3200   | 200–400   | 2.5 V | Original DDR; 184-pin DIMM |
| DDR2 | PC2-6400  | 400–1066  | 1.8 V | 240-pin DIMM; 4n prefetch |
| DDR3 | PC3-12800 | 800–2133  | 1.5 V | 240-pin DIMM; 8n prefetch; DDR3L at 1.35 V |
| DDR4 | PC4-25600 | 1600–3200 | 1.2 V | 288-pin DIMM; bank groups |
| DDR5 | PC5-44800 | 3200–8800 | 1.1 V | 288-pin DIMM; on-DIMM PMIC; two 32-bit subchannels |

Data rates above are the broad JEDEC ranges per generation; retail XMP/EXPO kits
often run above the base JEDEC bins. The values the SLeeLa classes carry are
representative defaults a machine description can override with `setDataRate`.

## Module form factors

- **DIMM** — desktop/server unbuffered or registered modules.
- **SO-DIMM** — laptop/small-form-factor modules.
- **RDIMM / LRDIMM** — registered / load-reduced server modules (buffered).

## Bus width and ECC

A standard channel is **64 bits** of data. **ECC** modules add 8 check bits for
a **72-bit** bus and detect/correct single-bit errors. `SLDDRModule.sized(mb, ecc)`
selects 64- vs 72-bit accordingly.

## Bandwidth

Peak transfer bandwidth ≈ data rate (MT/s) × channel width (bytes). For a single
64-bit DDR5-5600 channel: 5600 × 8 ≈ **44,800 MB/s** (hence `PC5-44800`).
`SLDDRModule.peakBandwidthMBps()` computes this.

## Manufacturers

DRAM die are produced by a small set of makers — **Samsung**, **SK Hynix**, and
**Micron** (whose consumer brand is **Crucial**) — with module assembly also by
**Kingston**, **Corsair**, **G.Skill**, and **Mushkin**. `SLDDRCatalog` exposes
each by name.

## Mapping into the Sleela VM

A module's capacity backs the VM's main memory. `SLDDRModule.wordsForWidth(bits)`
converts the module's byte capacity into the VM memory-word count that
`SLMachineModel` passes to `SLRAM.configure(...)`, so a named DDR part sizes the
emulated machine's RAM.
