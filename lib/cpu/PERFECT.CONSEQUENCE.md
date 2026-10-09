# PERFECT.CONSEQUENCE

*The perfect consequence of the SLeeLa CPU model: every CPU in `lib/cpu` runs as
a CPU on the hardware model, and the **same** CPU runs — unchanged — on the
Native Sleela VM and on the Secondary Sleela Source VM, with a predictable,
monotone cost as it climbs the stack.*

This document carries two tables:

1. **Native speeds** — each modelled CPU and the real-silicon clock/throughput
   it is modelled after.
2. **VM-hosted speeds** — the same CPUs when their decoded instruction stream is
   executed **(a) on the Native Sleela VM** (`lib/cpu/SLSleelaVM`, the direct
   canonical-opcode substrate) and **(b) on the Secondary Sleela Source VM**
   (`lib/vm/SLVM`, the managed interpreter layered on top of the substrate).

## How to read these numbers

- **Native MIPS** is the representative real-hardware throughput of the chip the
  model is named after (millions of instructions/second at its typical clock).
  For historical machines this is the classic figure; for modern SoCs it is a
  single-core, single-thread order-of-magnitude figure.
- **Native VM throughput** is expressed as a *fraction of native* — how fast the
  modelled instruction stream retires when each CPU instruction is lowered to
  canonical opcodes (`SLOpcodeMap`) and run one-at-a-time by `SLSleelaVM`. The
  native VM is a tight stack machine over `SLRAM`, so the dominant cost is the
  opcode-expansion factor (one register-ISA op → a few substrate opcodes).
- **Secondary VM throughput** is the fraction of native when the **same** stream
  additionally crosses the Secondary Sleela Source VM's managed layer
  (`SLVMFrame` operand stacks, `SLVMValue` tagging, `SLVM.run()` dispatch, native
  bindings as VM-exits). It adds a bounded, constant per-opcode overhead on top
  of the native VM path.
- **Feasibility (0–100)** rates whether a **12-core 5.2 GHz host** can run the
  CPU's **own expected software** acceptably: **100** = comfortably at speed,
  **0** = absolutely out of the question, linear/sampled in between. It is the
  host's sustained capacity divided by the software's demand (the CPU's native
  MIPS, weighted up for hard-real-time, many-core-saturating workloads like
  modern console titles), clamped to 0–100. Two columns, for two stackings:
  - **Direct** — the CPU runs on the Secondary Sleela Source VM, and the
    Secondary VM runs directly on the host. One interpretation layer; host
    capacity ≈ 3,600 guest-MIPS across the 12 cores.
  - **Nested** — the full stack: Native Sleela VM on the host, the Secondary
    Sleela Source VM hosted **on the Native VM**, the CPU on the Secondary VM,
    and then that generation's software on the CPU. The two interpreter layers
    **compound** (the Secondary VM's own work is itself interpreted by the
    Native VM), so host capacity drops ≈ 8× to ≈ 450 guest-MIPS — and the
    demanding modern generations fall toward (and reach) 0.
- These are **model characterizations**, not measured benchmarks: they describe
  the designed, monotone cost of the stack (native ≥ native-VM ≥ secondary-VM)
  so workload sizing with `SLCPUResource` is predictable. The *result* of a
  workload is identical on all three paths (1:1 Turing effect); only the
  retire-rate changes.

Model constants used below (per the one-opcode-at-a-time substrate contract):

| Path | Relative throughput vs. native | Why |
| --- | --- | --- |
| Hardware model | 1.0× (native MIPS) | the chip itself |
| Native Sleela VM | ≈ 1/8 of native | ~4–6 substrate opcodes per CPU instruction, interpreted |
| Secondary Sleela Source VM | ≈ 1/20 of native | native-VM path + managed frame/value/dispatch overhead (~2.5×) |

Host used for the feasibility columns: **12 cores @ 5.2 GHz**.

| Feasibility stacking | Host sustained capacity | Why |
| --- | --- | --- |
| Direct (CPU → Secondary VM → host) | ≈ 3,600 guest-MIPS | one interpretation layer; ≈ 600 guest-MIPS/core × 6 effective cores |
| Nested (CPU → Secondary VM → Native VM → host) | ≈ 450 guest-MIPS | two layers compound: the ≈ 8× Native-VM overhead multiplies the Secondary-VM path |

The guest result is identical for both stackings and to native hardware; only
the sustainable rate — and therefore the feasibility of running real-time
software — changes.

---

## Table 1 — CPUs and their native speeds

| # | Folder | CPU model (`SL<ARCH>CPU`) | Width | Representative native clock | Native throughput (MIPS) |
| --- | --- | --- | --- | --- | --- |
| 1 | `intel4004` | Intel 4004 | 4-bit | 740 kHz | 0.09 |
| 2 | `6502` | MOS 6502 | 8-bit | 1 MHz | 0.43 |
| 3 | `6809` | Motorola 6809 | 8-bit | 1 MHz | 0.50 |
| 4 | `z80` | Zilog Z80 | 8-bit | 4 MHz | 0.58 |
| 5 | `z8000` | Zilog Z8000 | 16-bit | 6 MHz | 1.2 |
| 6 | `dspic` | Microchip dsPIC | 16-bit | 40 MHz | 40 |
| 7 | `tms320` | TI TMS320 (C1x) | 16-bit | 20 MHz | 5 |
| 8 | `dsp56000` | Motorola DSP56000 | 24-bit | 20 MHz | 10 |
| 9 | `x86` | Intel IA-32 (386/486 class) | 32-bit | 33 MHz | 12 |
| 10 | `motorola-68000` | Motorola 68000 | 32-bit | 8 MHz | 1.3 |
| 11 | `68020` | Motorola 68020 | 32-bit | 16 MHz | 2.5 |
| 12 | `68030` | Motorola 68030 | 32-bit | 25 MHz | 5 |
| 13 | `68040` | Motorola 68040 | 32-bit | 25 MHz | 20 |
| 14 | `68060` | Motorola 68060 | 32-bit | 50 MHz | 88 |
| 15 | `coldfire` | Motorola ColdFire | 32-bit | 66 MHz | 60 |
| 16 | `mips` | MIPS R3000 | 32-bit | 33 MHz | 30 |
| 17 | `sparc` | SPARC V8 | 32-bit | 40 MHz | 28 |
| 18 | `pa-risc` | HP PA-RISC | 32-bit | 66 MHz | 60 |
| 19 | `m88k` | Motorola 88000 | 32-bit | 25 MHz | 17 |
| 20 | `amd29k` | AMD Am29000 | 32-bit | 25 MHz | 17 |
| 21 | `clipper` | Fairchild Clipper | 32-bit | 33 MHz | 33 |
| 22 | `ns32000` | NS 32000 | 32-bit | 15 MHz | 2 |
| 23 | `openrisc` | OpenRISC 1000 | 32-bit | 50 MHz | 45 |
| 24 | `weitek` | Weitek | 32-bit | 20 MHz | 20 |
| 25 | `i860` | Intel i860 | 32-bit | 40 MHz | 40 |
| 26 | `i960` | Intel i960 | 32-bit | 25 MHz | 20 |
| 27 | `iapx432` | Intel iAPX 432 | 32-bit | 8 MHz | 0.5 |
| 28 | `ibm801` | IBM 801 | 32-bit | 15 MHz | 15 |
| 29 | `romp` | IBM ROMP | 32-bit | 10 MHz | 2 |
| 30 | `power` | IBM POWER1 | 32-bit | 25 MHz | 25 |
| 31 | `powerpc` | PowerPC 601 | 32-bit | 66 MHz | 60 |
| 32 | `cortex-m` | ARM Cortex-M | 32-bit | 100 MHz | 125 |
| 33 | `cortex-r` | ARM Cortex-R | 32-bit | 600 MHz | 750 |
| 34 | `arm` | ARMv4 (A32, e.g. ARM7) | 32-bit | 25 MHz | 20 |
| 35 | `superh` | Hitachi SuperH SH-4 | 32-bit | 200 MHz | 360 |
| 36 | `xtensa` | Tensilica Xtensa | 32-bit | 240 MHz | 300 |
| 37 | `sharc` | Analog Devices SHARC | 32-bit | 40 MHz | 120 |
| 38 | `transputer` | Inmos Transputer T800 | 32-bit | 20 MHz | 10 |
| 39 | `system-360` | IBM System/360 (Model 50) | 32-bit | 2 MHz | 0.13 |
| 40 | `system-370` | IBM System/370 (158) | 32-bit | 8.7 MHz | 1 |
| 41 | `system-390` | IBM ESA/390 | 32-bit | 60 MHz | 70 |
| 42 | `z-architecture` | IBM z/Architecture (z900) | 64-bit | 770 MHz | 900 |
| 43 | `alpha` | DEC Alpha 21064 | 64-bit | 200 MHz | 300 |
| 44 | `itanium` | Intel Itanium (IA-64) | 64-bit | 800 MHz | 1600 |
| 45 | `vax` | DEC VAX-11/780 | 32-bit | 5 MHz | 1 (the "1 VUP") |
| 46 | `tx0` | MIT TX-0 | 18-bit | 0.2 MHz | 0.08 |
| 47 | `linc8` | DEC LINC-8 | 12-bit | 0.6 MHz | 0.06 |
| 48 | `pdp1` | DEC PDP-1 | 18-bit | 0.2 MHz | 0.1 |
| 49 | `pdp4` | DEC PDP-4 | 18-bit | 0.12 MHz | 0.06 |
| 50 | `pdp5` | DEC PDP-5 | 12-bit | 0.17 MHz | 0.05 |
| 51 | `pdp6` | DEC PDP-6 | 36-bit | 0.4 MHz | 0.25 |
| 52 | `pdp7` | DEC PDP-7 | 18-bit | 0.57 MHz | 0.28 |
| 53 | `pdp8` | DEC PDP-8 | 12-bit | 0.67 MHz | 0.33 |
| 54 | `pdp9` | DEC PDP-9 | 18-bit | 1 MHz | 0.5 |
| 55 | `pdp10` | DEC PDP-10 | 36-bit | 1 MHz | 0.4 |
| 56 | `pdp11` | DEC PDP-11/70 | 16-bit | 15 MHz | 1.2 |
| 57 | `pdp12` | DEC PDP-12 | 12-bit | 0.67 MHz | 0.33 |
| 58 | `pdp14` | DEC PDP-14 | 12-bit | 0.5 MHz | 0.2 |
| 59 | `pdp15` | DEC PDP-15 | 18-bit | 1.15 MHz | 0.57 |
| 60 | `nintendo-nes` | Nintendo NES (Ricoh 2A03 / 6502) | 8-bit | 1.79 MHz | 0.5 |
| 61 | `nintendo-snes` | Nintendo SNES (65C816) | 16-bit | 3.58 MHz | 1.5 |
| 62 | `nintendo-64` | Nintendo 64 (NEC VR4300 / MIPS) | 64-bit | 93.75 MHz | 125 |
| 63 | `nintendo-gamecube` | Nintendo GameCube (Gekko / PowerPC) | 32-bit | 486 MHz | 1125 |
| 64 | `nintendo-wii` | Nintendo Wii (Broadway / PowerPC) | 32-bit | 729 MHz | 1700 |
| 65 | `nintendo-wii-u` | Nintendo Wii U (Espresso / PowerPC) | 32-bit | 1.24 GHz | 3600 |
| 66 | `nintendo-switch` | Nintendo Switch (ARM Cortex-A57) | 64-bit | 1.02 GHz | 3000 |
| 67 | `nintendo-switch-2` | Nintendo Switch 2 (ARM Cortex-A78C) | 64-bit | 1.1 GHz | 6000 |
| 68 | `sega-sg-1000` | Sega SG-1000 (Z80) | 8-bit | 3.58 MHz | 0.5 |
| 69 | `sega-mark-iii` | Sega Mark III (Z80) | 8-bit | 3.58 MHz | 0.5 |
| 70 | `sega-master-system` | Sega Master System (Z80) | 8-bit | 3.58 MHz | 0.5 |
| 71 | `sega-game-gear` | Sega Game Gear (Z80) | 8-bit | 3.58 MHz | 0.5 |
| 72 | `sega-genesis` | Sega Genesis (68000) | 32-bit | 7.6 MHz | 1.3 |
| 73 | `sega-nomad` | Sega Nomad (68000) | 32-bit | 7.6 MHz | 1.3 |
| 74 | `sega-mega-cd` | Sega Mega-CD (68000) | 32-bit | 12.5 MHz | 2.1 |
| 75 | `sega-pico` | Sega Pico (68000) | 32-bit | 7.6 MHz | 1.3 |
| 76 | `sega-32x` | Sega 32X (SH-2) | 32-bit | 23 MHz | 28 |
| 77 | `sega-saturn` | Sega Saturn (SH-2) | 32-bit | 28.6 MHz | 35 |
| 78 | `sega-dreamcast` | Sega Dreamcast (SH-4) | 32-bit | 200 MHz | 360 |
| 79 | `playstation-1` | PlayStation (MIPS R3000A) | 32-bit | 33.9 MHz | 30 |
| 80 | `playstation-2` | PlayStation 2 (Emotion Engine / MIPS) | 64-bit | 294 MHz | 550 |
| 81 | `playstation-3` | PlayStation 3 (Cell / PowerPC) | 64-bit | 3.2 GHz | 10200 |
| 82 | `playstation-4` | PlayStation 4 (x86-64 Jaguar) | 64-bit | 1.6 GHz | 6400 |
| 83 | `playstation-5` | PlayStation 5 (x86-64 Zen 2) | 64-bit | 3.5 GHz | 28000 |
| 84 | `xbox` | Xbox (x86 Pentium III) | 32-bit | 733 MHz | 1500 |
| 85 | `xbox-360` | Xbox 360 (Xenon / PowerPC) | 64-bit | 3.2 GHz | 9600 |
| 86 | `xbox-one` | Xbox One (x86-64 Jaguar) | 64-bit | 1.75 GHz | 7000 |
| 87 | `xbox-one-s` | Xbox One S (x86-64 Jaguar) | 64-bit | 1.75 GHz | 7000 |
| 88 | `xbox-one-x` | Xbox One X (x86-64 Jaguar) | 64-bit | 2.3 GHz | 9200 |
| 89 | `xbox-series-s` | Xbox Series S (x86-64 Zen 2) | 64-bit | 3.6 GHz | 29000 |
| 90 | `xbox-series-x` | Xbox Series X (x86-64 Zen 2) | 64-bit | 3.8 GHz | 30000 |
| 91 | `atari-2600` | Atari 2600 (6507 / 6502) | 8-bit | 1.19 MHz | 0.5 |
| 92 | `atari-5200` | Atari 5200 (6502C) | 8-bit | 1.79 MHz | 0.75 |
| 93 | `atari-7800` | Atari 7800 (6502C) | 8-bit | 1.79 MHz | 0.75 |
| 94 | `atari-xegs` | Atari XEGS (6502C) | 8-bit | 1.79 MHz | 0.75 |
| 95 | `atari-lynx` | Atari Lynx (65C02 / Mikey) | 8-bit | 4 MHz | 1.6 |
| 96 | `atari-jaguar` | Atari Jaguar (68000 + Tom/Jerry RISC) | 32-bit | 26.6 MHz | 4.4 |
| 97 | `atari-jaguar-cd` | Atari Jaguar CD (68000 + Tom/Jerry RISC) | 32-bit | 26.6 MHz | 4.4 |
| 98 | `atari-vcs` | Atari VCS (x86-64 AMD Ryzen) | 64-bit | 1.7 GHz | 6800 |
| 99 | `riscv` | RISC-V RV32I (reference) | 32-bit | 100 MHz | 100 |

---

## Table 2 — The same CPUs under the two VMs

For each CPU this table shows the effective instruction throughput when the
modelled instruction stream is executed **(a) on the Native Sleela VM** and
**(b) on the Secondary Sleela Source VM**. The *result* is identical on both
paths and to native hardware; only the retire-rate differs, by the fixed stack
cost described above (native-VM ≈ 1/8 native; secondary-VM ≈ 1/20 native).

A third column rates **real-time feasibility (0–100)** of running each
CPU's *own expected software* on a **12-core, 5.2 GHz host** that executes
the CPU on top of the Secondary Sleela Source VM: **100** = the host runs
that software comfortably at speed, **0** = absolutely out of the question,
linear in between (sampled). It compares the host's sustained secondary-VM
capacity (~3,600 guest-MIPS across the 12 cores) against the CPU's software
demand (its native MIPS, weighted up for hard-real-time, many-core-
saturating workloads such as modern console AAA titles).

| # | CPU model (`SL<ARCH>CPU`) | Width | Native MIPS | (a) Native Sleela VM (MIPS) | (b) Secondary Sleela Source VM (MIPS) | Feasibility — direct: CPU on Secondary VM, on host (0–100) | Feasibility — nested: CPU on Secondary VM on Native VM, on host (0–100) |
| --- | --- | --- | --- | --- | --- | --- | --- |
| 1 | Intel 4004 | 4-bit | 0.09 | 0.011 | 0.0045 | 100 | 100 |
| 2 | MOS 6502 | 8-bit | 0.43 | 0.054 | 0.022 | 100 | 100 |
| 3 | Motorola 6809 | 8-bit | 0.50 | 0.063 | 0.025 | 100 | 100 |
| 4 | Zilog Z80 | 8-bit | 0.58 | 0.073 | 0.029 | 100 | 100 |
| 5 | Zilog Z8000 | 16-bit | 1.2 | 0.15 | 0.06 | 100 | 100 |
| 6 | Microchip dsPIC | 16-bit | 40 | 5.0 | 2.0 | 100 | 100 |
| 7 | TI TMS320 | 16-bit | 5 | 0.63 | 0.25 | 100 | 100 |
| 8 | Motorola DSP56000 | 24-bit | 10 | 1.25 | 0.50 | 100 | 100 |
| 9 | Intel IA-32 | 32-bit | 12 | 1.5 | 0.60 | 100 | 100 |
| 10 | Motorola 68000 | 32-bit | 1.3 | 0.16 | 0.065 | 100 | 100 |
| 11 | Motorola 68020 | 32-bit | 2.5 | 0.31 | 0.125 | 100 | 100 |
| 12 | Motorola 68030 | 32-bit | 5 | 0.63 | 0.25 | 100 | 100 |
| 13 | Motorola 68040 | 32-bit | 20 | 2.5 | 1.0 | 100 | 100 |
| 14 | Motorola 68060 | 32-bit | 88 | 11 | 4.4 | 100 | 100 |
| 15 | Motorola ColdFire | 32-bit | 60 | 7.5 | 3.0 | 100 | 100 |
| 16 | MIPS R3000 | 32-bit | 30 | 3.75 | 1.5 | 100 | 100 |
| 17 | SPARC V8 | 32-bit | 28 | 3.5 | 1.4 | 100 | 100 |
| 18 | HP PA-RISC | 32-bit | 60 | 7.5 | 3.0 | 100 | 100 |
| 19 | Motorola 88000 | 32-bit | 17 | 2.1 | 0.85 | 100 | 100 |
| 20 | AMD Am29000 | 32-bit | 17 | 2.1 | 0.85 | 100 | 100 |
| 21 | Fairchild Clipper | 32-bit | 33 | 4.1 | 1.65 | 100 | 100 |
| 22 | NS 32000 | 32-bit | 2 | 0.25 | 0.10 | 100 | 100 |
| 23 | OpenRISC 1000 | 32-bit | 45 | 5.6 | 2.25 | 100 | 100 |
| 24 | Weitek | 32-bit | 20 | 2.5 | 1.0 | 100 | 100 |
| 25 | Intel i860 | 32-bit | 40 | 5.0 | 2.0 | 100 | 100 |
| 26 | Intel i960 | 32-bit | 20 | 2.5 | 1.0 | 100 | 100 |
| 27 | Intel iAPX 432 | 32-bit | 0.5 | 0.063 | 0.025 | 100 | 100 |
| 28 | IBM 801 | 32-bit | 15 | 1.9 | 0.75 | 100 | 100 |
| 29 | IBM ROMP | 32-bit | 2 | 0.25 | 0.10 | 100 | 100 |
| 30 | IBM POWER1 | 32-bit | 25 | 3.1 | 1.25 | 100 | 100 |
| 31 | PowerPC 601 | 32-bit | 60 | 7.5 | 3.0 | 100 | 100 |
| 32 | ARM Cortex-M | 32-bit | 125 | 15.6 | 6.25 | 100 | 100 |
| 33 | ARM Cortex-R | 32-bit | 750 | 94 | 37.5 | 100 | 60 |
| 34 | ARMv4 (A32) | 32-bit | 20 | 2.5 | 1.0 | 100 | 100 |
| 35 | Hitachi SuperH SH-4 | 32-bit | 360 | 45 | 18 | 100 | 100 |
| 36 | Tensilica Xtensa | 32-bit | 300 | 37.5 | 15 | 100 | 100 |
| 37 | Analog Devices SHARC | 32-bit | 120 | 15 | 6.0 | 100 | 100 |
| 38 | Inmos Transputer T800 | 32-bit | 10 | 1.25 | 0.50 | 100 | 100 |
| 39 | IBM System/360 | 32-bit | 0.13 | 0.016 | 0.0065 | 100 | 100 |
| 40 | IBM System/370 | 32-bit | 1 | 0.125 | 0.05 | 100 | 100 |
| 41 | IBM ESA/390 | 32-bit | 70 | 8.75 | 3.5 | 100 | 100 |
| 42 | IBM z/Architecture | 64-bit | 900 | 112 | 45 | 100 | 50 |
| 43 | DEC Alpha 21064 | 64-bit | 300 | 37.5 | 15 | 100 | 100 |
| 44 | Intel Itanium (IA-64) | 64-bit | 1600 | 200 | 80 | 100 | 28 |
| 45 | DEC VAX-11/780 | 32-bit | 1 | 0.125 | 0.05 | 100 | 100 |
| 46 | MIT TX-0 | 18-bit | 0.08 | 0.010 | 0.004 | 100 | 100 |
| 47 | DEC LINC-8 | 12-bit | 0.06 | 0.0075 | 0.003 | 100 | 100 |
| 48 | DEC PDP-1 | 18-bit | 0.1 | 0.013 | 0.005 | 100 | 100 |
| 49 | DEC PDP-4 | 18-bit | 0.06 | 0.0075 | 0.003 | 100 | 100 |
| 50 | DEC PDP-5 | 12-bit | 0.05 | 0.0063 | 0.0025 | 100 | 100 |
| 51 | DEC PDP-6 | 36-bit | 0.25 | 0.031 | 0.0125 | 100 | 100 |
| 52 | DEC PDP-7 | 18-bit | 0.28 | 0.035 | 0.014 | 100 | 100 |
| 53 | DEC PDP-8 | 12-bit | 0.33 | 0.041 | 0.0165 | 100 | 100 |
| 54 | DEC PDP-9 | 18-bit | 0.5 | 0.063 | 0.025 | 100 | 100 |
| 55 | DEC PDP-10 | 36-bit | 0.4 | 0.05 | 0.02 | 100 | 100 |
| 56 | DEC PDP-11/70 | 16-bit | 1.2 | 0.15 | 0.06 | 100 | 100 |
| 57 | DEC PDP-12 | 12-bit | 0.33 | 0.041 | 0.0165 | 100 | 100 |
| 58 | DEC PDP-14 | 12-bit | 0.2 | 0.025 | 0.010 | 100 | 100 |
| 59 | DEC PDP-15 | 18-bit | 0.57 | 0.071 | 0.0285 | 100 | 100 |
| 60 | Nintendo NES (6502) | 8-bit | 0.5 | 0.063 | 0.025 | 100 | 100 |
| 61 | Nintendo SNES (65C816) | 16-bit | 1.5 | 0.19 | 0.075 | 100 | 100 |
| 62 | Nintendo 64 (VR4300) | 64-bit | 125 | 15.6 | 6.25 | 100 | 100 |
| 63 | GameCube (Gekko) | 32-bit | 1125 | 141 | 56 | 100 | 40 |
| 64 | Wii (Broadway) | 32-bit | 1700 | 212 | 85 | 100 | 26 |
| 65 | Wii U (Espresso) | 32-bit | 3600 | 450 | 180 | 100 | 12 |
| 66 | Switch (Cortex-A57) | 64-bit | 3000 | 375 | 150 | 40 | 5 |
| 67 | Switch 2 (Cortex-A78C) | 64-bit | 6000 | 750 | 300 | 15 | 2 |
| 68 | Sega SG-1000 (Z80) | 8-bit | 0.5 | 0.063 | 0.025 | 100 | 100 |
| 69 | Sega Mark III (Z80) | 8-bit | 0.5 | 0.063 | 0.025 | 100 | 100 |
| 70 | Sega Master System (Z80) | 8-bit | 0.5 | 0.063 | 0.025 | 100 | 100 |
| 71 | Sega Game Gear (Z80) | 8-bit | 0.5 | 0.063 | 0.025 | 100 | 100 |
| 72 | Sega Genesis (68000) | 32-bit | 1.3 | 0.16 | 0.065 | 100 | 100 |
| 73 | Sega Nomad (68000) | 32-bit | 1.3 | 0.16 | 0.065 | 100 | 100 |
| 74 | Sega Mega-CD (68000) | 32-bit | 2.1 | 0.26 | 0.105 | 100 | 100 |
| 75 | Sega Pico (68000) | 32-bit | 1.3 | 0.16 | 0.065 | 100 | 100 |
| 76 | Sega 32X (SH-2) | 32-bit | 28 | 3.5 | 1.4 | 100 | 100 |
| 77 | Sega Saturn (SH-2) | 32-bit | 35 | 4.4 | 1.75 | 100 | 100 |
| 78 | Sega Dreamcast (SH-4) | 32-bit | 360 | 45 | 18 | 100 | 100 |
| 79 | PlayStation (R3000A) | 32-bit | 30 | 3.75 | 1.5 | 100 | 100 |
| 80 | PlayStation 2 (EE) | 64-bit | 550 | 69 | 27.5 | 100 | 82 |
| 81 | PlayStation 3 (Cell PPE) | 64-bit | 10200 | 1275 | 510 | 6 | 1 |
| 82 | PlayStation 4 (Jaguar) | 64-bit | 6400 | 800 | 320 | 11 | 1 |
| 83 | PlayStation 5 (Zen 2) | 64-bit | 28000 | 3500 | 1400 | 1 | 0 |
| 84 | Xbox (Pentium III) | 32-bit | 1500 | 187 | 75 | 100 | 30 |
| 85 | Xbox 360 (Xenon) | 64-bit | 9600 | 1200 | 480 | 6 | 1 |
| 86 | Xbox One (Jaguar) | 64-bit | 7000 | 875 | 350 | 10 | 1 |
| 87 | Xbox One S (Jaguar) | 64-bit | 7000 | 875 | 350 | 10 | 1 |
| 88 | Xbox One X (Jaguar) | 64-bit | 9200 | 1150 | 460 | 7 | 1 |
| 89 | Xbox Series S (Zen 2) | 64-bit | 29000 | 3625 | 1450 | 1 | 0 |
| 90 | Xbox Series X (Zen 2) | 64-bit | 30000 | 3750 | 1500 | 1 | 0 |
| 91 | Atari 2600 (6507) | 8-bit | 0.5 | 0.063 | 0.025 | 100 | 100 |
| 92 | Atari 5200 (6502C) | 8-bit | 0.75 | 0.094 | 0.0375 | 100 | 100 |
| 93 | Atari 7800 (6502C) | 8-bit | 0.75 | 0.094 | 0.0375 | 100 | 100 |
| 94 | Atari XEGS (6502C) | 8-bit | 0.75 | 0.094 | 0.0375 | 100 | 100 |
| 95 | Atari Lynx (65C02) | 8-bit | 1.6 | 0.20 | 0.08 | 100 | 100 |
| 96 | Atari Jaguar (68000) | 32-bit | 4.4 | 0.55 | 0.22 | 100 | 100 |
| 97 | Atari Jaguar CD (68000) | 32-bit | 4.4 | 0.55 | 0.22 | 100 | 100 |
| 98 | Atari VCS (Ryzen) | 64-bit | 6800 | 850 | 340 | 11 | 1 |
| 99 | RISC-V RV32I | 32-bit | 100 | 12.5 | 5.0 | 100 | 100 |

---

## The perfect consequence

Because every CPU in `lib/cpu` extends `SLCPURuntime`, and `SLCPURuntime` lowers
each decoded instruction onto the canonical `SLSleelaOpcode` substrate, the three
columns above are the **same machine** observed at three depths of the stack:

```
  hardware model   ──►  native MIPS          (Table 1)
        │
  Native Sleela VM ──►  ≈ 1/8 native          (Table 2, column a)   lib/cpu/SLSleelaVM
        │
  Secondary Sleela ──►  ≈ 1/20 native         (Table 2, column b)   lib/vm/SLVM
  Source VM
```

The cost is monotone and bounded, and — the perfect consequence — a workload in
C, C++, or Sleela produces the **identical result** on all three, on any of the
99 CPUs. Speed degrades predictably as you climb; correctness does not change at
all.

The feasibility columns then answer the practical question for two stackings on
a strong modern host (12 cores @ 5.2 GHz):

- **Direct** (CPU on the Secondary VM, Secondary VM on the host): the host runs
  the *own expected software* of **87 of 99** CPUs comfortably (score 100) —
  everything through the sixth-generation consoles — while the twelve most
  demanding modern SoCs taper down (Switch 40 → Switch 2 15 → PS4 11 → PS3 /
  Xbox 360 6 → PS5 / Xbox Series S and X 1).
- **Nested** (CPU on the Secondary VM, Secondary VM on the **Native VM**, on the
  host): the two interpreter layers compound, so the demanding generations fall
  much harder and some reach genuine **0** — GameCube 40, Wii 26, Wii U 12,
  Switch 5, Switch 2 2, PlayStation 2 82, Xbox 30, and PlayStation 3/4,
  Xbox 360/One, Atari VCS at 1, with **PlayStation 5 and Xbox Series S/X at 0**:
  absolutely out of the question in real time. Everything through the fifth/
  sixth generation and all the historical/business/RISC CPUs still score 100.

The difference between the two columns is exactly the cost of the extra layer:
running the Secondary VM *on* the Native VM rather than on bare host.

> Figures are model characterizations for workload sizing with `SLCPUResource`,
> not measured benchmarks. Native clocks/MIPS reflect the representative real
> hardware each model is named after; the VM columns apply the fixed stack-cost
> factors (1/8 and 1/20 of native) defined above. The two feasibility columns
> compare the 12-core 5.2 GHz host's sustained capacity — **direct** (≈ 3,600
> guest-MIPS, Secondary VM on the host) and **nested** (≈ 450 guest-MIPS,
> Secondary VM on the Native VM) — against each CPU's (real-time-weighted)
> software demand, clamped to 0–100.
