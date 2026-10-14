#!/usr/bin/env python3
"""
tools/generate-cpu-docs.py

Generate the nine per-folder markdown documents (ARCHITECTURE, BUS, CACHE,
CIRCUIT, CPU, DMA, REGISTERS, SPECIFICATION, TIMING) for every CPU folder under
lib/cpu/ that ships a runnable SL<ARCH>CPU.sleela but does not yet have the
documentation set. This brings the "stub" folders up to the same finished shape
as the hand-authored reference folders (6502, tx0, the PDP line, x86, arm, ...).

Design:
  - The per-CPU identity/width/register/clock data is the single source of
    truth already encoded in tools/generate-cpu-models.py (SPECS + CLOCK). This
    script imports that table so the docs can never drift from the .sleela
    models.
  - Each CPU's higher-level characteristics (register-file model, cache tier,
    integrated DMA, MMU/TLB, era) are DERIVED from that data plus a small,
    reviewed ATTR table, so the docs state accurate, non-invented facts and use
    the repository's evidence-labelling convention.
  - Folders that already contain markdown (the reference folders) are skipped,
    so hand-authored documentation is never overwritten.

Run:
  python3 tools/generate-cpu-docs.py            # write docs for stub folders
  python3 tools/generate-cpu-docs.py --check     # report, write nothing

  python3 tools/generate-cpu-docs.py --force     # DANGER: (re)write EVERY folder,
                                                 # including the hand-authored
                                                 # reference folders (6502, tx0,
                                                 # the PDP line, x86, arm, ...).
                                                 # Those docs are hand-written and
                                                 # should NOT be regenerated; use
                                                 # --force only if you have a
                                                 # clean working tree and intend
                                                 # to overwrite them.
"""
import os
import sys
import importlib.util

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
CPU = os.path.join(ROOT, "lib", "cpu")

# ---- import SPECS / CLOCK from the model generator (single source of truth) ---
_spec = importlib.util.spec_from_file_location(
    "cpu_models", os.path.join(ROOT, "tools", "generate-cpu-models.py")
)
_mod = importlib.util.module_from_spec(_spec)
_spec.loader.exec_module(_mod)
SPECS = _mod.SPECS
CLOCK = _mod.CLOCK
RISC, ACC, CISC = _mod.RISC, _mod.ACC, _mod.CISC

DOC_NAMES = [
    "ARCHITECTURE.md", "BUS.md", "CACHE.md", "CIRCUIT.md", "CPU.md",
    "DMA.md", "REGISTERS.md", "SPECIFICATION.md", "TIMING.md",
]

# ---------------------------------------------------------------------------
# Per-CPU reviewed attributes keyed by folder. Only non-derivable facts live
# here: the cache tier, whether DMA/MMU are integrated, an era label, a short
# programmer-register description, a family-lineage note, and sources. Anything
# omitted falls back to a width/clock-derived default (see attrs()).
#   cache: "none" | "l1" | "l1l2" | "l1l2l3"
#   dma:   True if the CPU/SoC integrates a DMA engine (vs. system-level only)
#   mmu:   True if the CPU integrates an MMU/TLB
# ---------------------------------------------------------------------------
ATTR = {
    # Intel desktop/server line
    "intel-8080":  dict(cache="none", dma=False, mmu=False, era="8-bit microprocessor",
                        regs="A, B, C, D, E, H, L accumulator/register file, SP, PC, flags",
                        lineage="Intel 8080 -> 8085 -> 8086 (x86). Derivatives (8085, Z80) are separate profiles.",
                        src=["https://en.wikipedia.org/wiki/Intel_8080", "https://en.wikipedia.org/wiki/X86"]),
    "intel-8086":  dict(cache="none", dma=False, mmu=False, era="16-bit microprocessor",
                        regs="AX, BX, CX, DX, SI, DI, BP, SP, IP, segment registers (CS/DS/SS/ES), FLAGS",
                        lineage="Intel 8086/8088 is the origin of the x86 ISA. 80186/80286 are separate profiles.",
                        src=["https://en.wikipedia.org/wiki/X86", "https://en.wikipedia.org/wiki/Intel_8086"]),
    "intel-80286": dict(cache="none", dma=False, mmu=True, era="16-bit protected-mode microprocessor",
                        regs="8086 register model plus protected-mode segment descriptors and MSW",
                        lineage="80286 adds protected mode and 24-bit addressing over the 8086. 80386 is a separate profile.",
                        src=["https://en.wikipedia.org/wiki/Intel_80286", "https://en.wikipedia.org/wiki/X86"]),
    "intel-80386": dict(cache="none", dma=False, mmu=True, era="32-bit microprocessor",
                        regs="EAX..ESP 32-bit general registers, segment registers, EFLAGS, EIP, CR0-CR3, paging MMU",
                        lineage="80386 (IA-32) adds 32-bit registers and paging. The 80486 integrates cache/FPU and is a separate profile.",
                        src=["https://en.wikipedia.org/wiki/I386", "https://en.wikipedia.org/wiki/X86"]),
    "intel-80486": dict(cache="l1", dma=False, mmu=True, era="32-bit pipelined microprocessor",
                        regs="IA-32 register model (EAX..ESP, EFLAGS, EIP, control registers) with integrated FPU",
                        lineage="First tightly pipelined x86 with on-chip L1 cache and integrated FPU. Pentium is a separate profile.",
                        src=["https://en.wikipedia.org/wiki/I486"]),
    "intel-pentium": dict(cache="l1", dma=False, mmu=True, era="32-bit superscalar microprocessor",
                        regs="IA-32 register model with two integer pipelines (U/V) and split code/data L1 caches",
                        lineage="Superscalar extension of the 80486 with a 64-bit external data bus. Later P6 cores are separate profiles.",
                        src=["https://en.wikipedia.org/wiki/Pentium"]),
    "intel-celeron": dict(cache="l1l2", dma=False, mmu=True, era="32/64-bit value microprocessor",
                        regs="IA-32/x86-64 register model with reduced cache versus the contemporary Core/Pentium parts",
                        lineage="Value-segment derivative sharing the contemporary Intel core with reduced cache/features.",
                        src=["https://en.wikipedia.org/wiki/Celeron"]),
    "intel-core":  dict(cache="l1l2l3", dma=False, mmu=True, era="64-bit multi-core microprocessor",
                        regs="x86-64 register model (16 GP, 16 SIMD) with per-core L1/L2 and shared L3",
                        lineage="Intel Core (x86-64) multi-core line. Xeon shares the microarchitecture at server scale (separate profile).",
                        src=["https://en.wikipedia.org/wiki/Intel_Core_(microarchitecture)", "https://en.wikipedia.org/wiki/X86-64"]),
    "intel-xeon":  dict(cache="l1l2l3", dma=False, mmu=True, era="64-bit server multi-core microprocessor",
                        regs="x86-64 register model with large shared L3, more cores and ECC/RAS features",
                        lineage="Server/workstation sibling of the Intel Core microarchitecture.",
                        src=["https://en.wikipedia.org/wiki/Xeon", "https://en.wikipedia.org/wiki/X86-64"]),
    # AMD desktop/server line
    "amd-athlon":  dict(cache="l1l2", dma=False, mmu=True, era="32-bit superscalar microprocessor",
                        regs="IA-32 register model (K7 microarchitecture) with on-chip L1 and L2",
                        lineage="AMD K7. The x86-64 Opteron/Athlon 64 and Ryzen are separate profiles.",
                        src=["https://en.wikipedia.org/wiki/Athlon", "https://en.wikipedia.org/wiki/List_of_AMD_processors"]),
    "amd-opteron": dict(cache="l1l2l3", dma=False, mmu=True, era="64-bit server microprocessor",
                        regs="x86-64 register model; first processor line to implement x86-64",
                        lineage="First x86-64 server line (K8+). EPYC is the Zen-era successor (separate profile).",
                        src=["https://en.wikipedia.org/wiki/Opteron", "https://en.wikipedia.org/wiki/X86-64"]),
    "amd-ryzen":   dict(cache="l1l2l3", dma=False, mmu=True, era="64-bit multi-core microprocessor",
                        regs="x86-64 register model (Zen microarchitecture) with per-core L1/L2 and shared L3 per CCX/CCD",
                        lineage="AMD Zen desktop line. EPYC is the server sibling (separate profile).",
                        src=["https://en.wikipedia.org/wiki/Zen_(microarchitecture)", "https://en.wikipedia.org/wiki/List_of_AMD_Ryzen_processors"]),
    "amd-epyc":    dict(cache="l1l2l3", dma=False, mmu=True, era="64-bit server multi-core microprocessor",
                        regs="x86-64 register model (Zen) with chiplet CCDs, large shared L3 and ECC/RAS",
                        lineage="Server/datacenter sibling of Ryzen using the same Zen microarchitecture.",
                        src=["https://en.wikipedia.org/wiki/Epyc", "https://en.wikipedia.org/wiki/Zen_(microarchitecture)"]),
    # Apple Silicon (ARM64)
    "apple-a-series": dict(cache="l1l2", dma=True, mmu=True, era="64-bit mobile SoC (ARM64)",
                        regs="AArch64 register model (31 GP x0-x30, SP, PC, 32 SIMD/FP) in a big.LITTLE SoC",
                        lineage="Apple A-series mobile SoCs; the desktop M-series derives from the same core families (separate profiles).",
                        src=["https://en.wikipedia.org/wiki/Apple_silicon", "https://en.wikipedia.org/wiki/AArch64"]),
    "apple-m1":    dict(cache="l1l2", dma=True, mmu=True, era="64-bit desktop SoC (ARM64)",
                        regs="AArch64 register model with Firestorm (P) + Icestorm (E) cores and shared system-level cache",
                        lineage="First Apple M desktop SoC. M2/M3/M4 are separate profiles.",
                        src=["https://en.wikipedia.org/wiki/Apple_M1", "https://en.wikipedia.org/wiki/Apple_silicon"]),
    "apple-m2":    dict(cache="l1l2", dma=True, mmu=True, era="64-bit desktop SoC (ARM64)",
                        regs="AArch64 register model with Avalanche (P) + Blizzard (E) cores and shared system-level cache",
                        lineage="Second-generation Apple M desktop SoC.",
                        src=["https://en.wikipedia.org/wiki/Apple_M2", "https://en.wikipedia.org/wiki/Apple_silicon"]),
    "apple-m3":    dict(cache="l1l2", dma=True, mmu=True, era="64-bit desktop SoC (ARM64)",
                        regs="AArch64 register model with updated P/E cores and shared system-level cache",
                        lineage="Third-generation Apple M desktop SoC.",
                        src=["https://en.wikipedia.org/wiki/Apple_M3", "https://en.wikipedia.org/wiki/Apple_silicon"]),
    "apple-m4":    dict(cache="l1l2", dma=True, mmu=True, era="64-bit desktop SoC (ARM64)",
                        regs="AArch64 register model with updated P/E cores and shared system-level cache",
                        lineage="Fourth-generation Apple M desktop SoC.",
                        src=["https://en.wikipedia.org/wiki/Apple_M4", "https://en.wikipedia.org/wiki/Apple_silicon"]),
    # Qualcomm / ARM / embedded SoCs
    "qualcomm-snapdragon": dict(cache="l1l2l3", dma=True, mmu=True, era="64-bit mobile SoC (ARM64)",
                        regs="AArch64 register model; big.LITTLE / Kryo-Oryon core clusters with shared L3",
                        lineage="Qualcomm Snapdragon mobile SoC family (ARM or in-house Oryon cores).",
                        src=["https://en.wikipedia.org/wiki/Qualcomm_Snapdragon", "https://en.wikipedia.org/wiki/AArch64"]),
    "mediatek-dimensity": dict(cache="l1l2l3", dma=True, mmu=True, era="64-bit mobile SoC (ARM64)",
                        regs="AArch64 register model; ARM Cortex-class big.LITTLE clusters with shared L3",
                        lineage="MediaTek Dimensity mobile SoC family built on ARM Cortex/C-series cores.",
                        src=["https://en.wikipedia.org/wiki/List_of_MediaTek_systems_on_chips", "https://en.wikipedia.org/wiki/AArch64"]),
    "arm-cortex-a": dict(cache="l1l2", dma=False, mmu=True, era="64-bit application core (ARM64)",
                        regs="AArch64 register model (31 GP, SP, PC, SIMD/FP); application-class core with MMU",
                        lineage="ARM Cortex-A application cores licensed across many SoCs. Neoverse is the server line (separate profile).",
                        src=["https://en.wikipedia.org/wiki/Comparison_of_ARM_processors", "https://en.wikipedia.org/wiki/ARM_Cortex-A"]),
    "arm-neoverse": dict(cache="l1l2l3", dma=False, mmu=True, era="64-bit server core (ARM64)",
                        regs="AArch64 register model; server-class core with large private L2 and mesh-connected shared cache",
                        lineage="ARM Neoverse server core line (N/V/E series) used by cloud and HPC SoCs.",
                        src=["https://en.wikipedia.org/wiki/ARM_Neoverse", "https://en.wikipedia.org/wiki/AArch64"]),
    "rockchip":    dict(cache="l1l2", dma=True, mmu=True, era="64-bit embedded SoC (ARM64)",
                        regs="AArch64 register model; ARM Cortex-A big.LITTLE clusters in an application SoC",
                        lineage="Rockchip ARM application SoCs (e.g. RK3399-class) built on licensed Cortex-A cores.",
                        src=["https://en.wikipedia.org/wiki/Rockchip", "https://en.wikipedia.org/wiki/AArch64"]),
    "nvidia-grace": dict(cache="l1l2l3", dma=False, mmu=True, era="64-bit server CPU (ARM Neoverse)",
                        regs="AArch64 register model; many Neoverse-V2 cores with a coherent mesh and large shared cache",
                        lineage="NVIDIA Grace datacenter CPU built on ARM Neoverse V2 cores.",
                        src=["https://www.nvidia.com/en-us/data-center/grace-cpu/", "https://en.wikipedia.org/wiki/ARM_Neoverse"]),
    "ampere-altra": dict(cache="l1l2l3", dma=False, mmu=True, era="64-bit server CPU (ARM64)",
                        regs="AArch64 register model; many single-threaded Neoverse-class cores with large shared cache",
                        lineage="Ampere Altra cloud-native server CPU built on ARM Neoverse N1-class cores.",
                        src=["https://en.wikipedia.org/wiki/Ampere_Computing", "https://en.wikipedia.org/wiki/ARM_Neoverse"]),
    # IBM POWER / Sun server
    "ibm-power9":  dict(cache="l1l2l3", dma=False, mmu=True, era="64-bit server CPU (Power ISA)",
                        regs="Power ISA v3.0 register model (32 GP, 32 VSR), SMT4/SMT8, large on-chip L2/L3",
                        lineage="IBM POWER9 (Power ISA v3.0). POWER10 is the successor (separate profile).",
                        src=["https://en.wikipedia.org/wiki/POWER9", "https://en.wikipedia.org/wiki/Power_ISA"]),
    "ibm-power10": dict(cache="l1l2l3", dma=False, mmu=True, era="64-bit server CPU (Power ISA)",
                        regs="Power ISA v3.1 register model (32 GP, 32 VSR), SMT8, large on-chip L2/L3",
                        lineage="IBM POWER10 (Power ISA v3.1), successor to POWER9.",
                        src=["https://en.wikipedia.org/wiki/POWER10", "https://en.wikipedia.org/wiki/Power_ISA"]),
    "sun-ultrasparc": dict(cache="l1l2", dma=False, mmu=True, era="64-bit workstation/server CPU (SPARC V9)",
                        regs="SPARC V9 register windows (global + windowed in/out/local), g0 hardwired zero",
                        lineage="Sun UltraSPARC (SPARC V9) workstation/server line.",
                        src=["https://en.wikipedia.org/wiki/UltraSPARC", "https://en.wikipedia.org/wiki/SPARC"]),
    # SiFive RISC-V
    "sifive-u74":  dict(cache="l1l2", dma=False, mmu=True, era="64-bit application core (RISC-V RV64GC)",
                        regs="RISC-V RV64GC register model (x0 hardwired zero, x1-x31, 32 FP), Sv39 MMU",
                        lineage="SiFive U74 application core (RV64GC) used in Linux-capable RISC-V SoCs.",
                        src=["https://www.scs.stanford.edu/~zyedidia/docs/sifive/sifive-u74.pdf", "https://en.wikipedia.org/wiki/RISC-V"]),
    # Classic gaps
    "motorola-6800": dict(cache="none", dma=False, mmu=False, era="8-bit microprocessor",
                        regs="A, B accumulators, X index, SP, PC, condition-code register",
                        lineage="Motorola 6800 (origin of the 68xx line). The 6809 is a separate profile.",
                        src=["https://en.wikipedia.org/wiki/Motorola_6800"]),
    "wdc-65c816":  dict(cache="none", dma=False, mmu=False, era="8/16-bit microprocessor",
                        regs="A (8/16), X/Y (8/16), 16-bit stack pointer, direct-page, 24-bit program address, status",
                        lineage="WDC 65C816, a 16-bit follow-up to the 65C02 with 24-bit addressing.",
                        src=["https://wikiwand.com/en/articles/65C816"]),
    "zilog-ez80":  dict(cache="none", dma=False, mmu=False, era="8-bit microprocessor",
                        regs="Z80-compatible register set (AF, BC, DE, HL, IX, IY, SP, PC) with 24-bit addressing",
                        lineage="Zilog eZ80, a pipelined Z80-compatible core addressing up to 16 MB without an MMU.",
                        src=["https://en.wikipedia.org/wiki/Zilog_eZ80"]),
    # Consoles
    "nintendo-nes": dict(cache="none", dma=True, mmu=False, era="8-bit console main CPU",
                        regs="6502-class programmer model (A, X, Y, S, PC, P); Ricoh 2A03 integrates audio and DMA",
                        lineage="Nintendo NES main CPU is a Ricoh 2A03 (6502 core without decimal mode).",
                        src=["https://en.wikipedia.org/wiki/Ricoh_2A03", "https://www.nesdev.org/wiki/CPU"]),
    "nintendo-snes": dict(cache="none", dma=True, mmu=False, era="16-bit console main CPU",
                        regs="65C816 programmer model (A, X, Y 8/16-bit, 16-bit SP, direct page, 24-bit PBR/addr)",
                        lineage="Nintendo SNES main CPU is a Ricoh 5A22 based on the WDC 65C816.",
                        src=["https://en.wikipedia.org/wiki/Ricoh_5A22", "https://snes.nesdev.org/wiki/Timing"]),
    "nintendo-64": dict(cache="l1", dma=True, mmu=True, era="64-bit console main CPU (MIPS)",
                        regs="MIPS III register model (32 GP, r0 hardwired zero, HI/LO, 32 FP); NEC VR4300",
                        lineage="Nintendo 64 main CPU is a NEC VR4300 (MIPS R4300i-class).",
                        src=["https://en.wikipedia.org/wiki/Nintendo_64", "https://en.wikipedia.org/wiki/NEC_VR4300"]),
    "nintendo-gamecube": dict(cache="l1l2", dma=True, mmu=True, era="32-bit console main CPU (PowerPC)",
                        regs="PowerPC 750-class register model (32 GP, 32 FP) with on-chip L1/L2; IBM Gekko",
                        lineage="GameCube main CPU is IBM Gekko, a customised PowerPC 750CXe.",
                        src=["https://en.wikipedia.org/wiki/Gekko_(microprocessor)", "https://en.wikipedia.org/wiki/PowerPC_7xx"]),
    "nintendo-wii": dict(cache="l1l2", dma=True, mmu=True, era="32-bit console main CPU (PowerPC)",
                        regs="PowerPC 750-class register model (32 GP, 32 FP) with on-chip L1/L2; IBM Broadway",
                        lineage="Wii main CPU is IBM Broadway, a higher-clocked evolution of the GameCube Gekko.",
                        src=["https://en.wikipedia.org/wiki/Broadway_(processor)", "https://en.wikipedia.org/wiki/PowerPC_7xx"]),
    "nintendo-wii-u": dict(cache="l1l2", dma=True, mmu=True, era="32-bit console main CPU (PowerPC, tri-core)",
                        regs="PowerPC 750-class tri-core register model (32 GP, 32 FP per core); IBM Espresso",
                        lineage="Wii U main CPU is IBM Espresso, a tri-core PowerPC 750-derived design.",
                        src=["https://en.wikipedia.org/wiki/Espresso_(processor)", "https://en.wikipedia.org/wiki/PowerPC_7xx"]),
    "nintendo-switch": dict(cache="l1l2", dma=True, mmu=True, era="64-bit console SoC (ARM64)",
                        regs="AArch64 register model; NVIDIA Tegra X1 with ARM Cortex-A57 cluster",
                        lineage="Nintendo Switch SoC is an NVIDIA Tegra X1 (ARM Cortex-A57).",
                        src=["https://en.wikipedia.org/wiki/Nintendo_Switch", "https://en.wikipedia.org/wiki/Tegra"]),
    "nintendo-switch-2": dict(cache="l1l2l3", dma=True, mmu=True, era="64-bit console SoC (ARM64)",
                        regs="AArch64 register model; custom NVIDIA Tegra SoC with ARM Cortex-A78C-class cluster",
                        lineage="Nintendo Switch 2 SoC is a custom NVIDIA Tegra (ARM Cortex-A78C-class).",
                        src=["https://en.wikipedia.org/wiki/Nintendo_Switch_2", "https://en.wikipedia.org/wiki/ARM_Cortex-A78"]),
    "sega-sg-1000": dict(cache="none", dma=False, mmu=False, era="8-bit console main CPU (Z80)",
                        regs="Z80 register model (AF, BC, DE, HL + alternates, IX, IY, SP, PC)",
                        lineage="Sega SG-1000 main CPU is a Zilog Z80.",
                        src=["https://en.wikipedia.org/wiki/SG-1000", "https://en.wikipedia.org/wiki/Zilog_Z80"]),
    "sega-mark-iii": dict(cache="none", dma=False, mmu=False, era="8-bit console main CPU (Z80)",
                        regs="Z80 register model (AF, BC, DE, HL + alternates, IX, IY, SP, PC)",
                        lineage="Sega Mark III (Master System in Japan) main CPU is a Zilog Z80.",
                        src=["https://en.wikipedia.org/wiki/Sega_Mark_III", "https://en.wikipedia.org/wiki/Zilog_Z80"]),
    "sega-master-system": dict(cache="none", dma=False, mmu=False, era="8-bit console main CPU (Z80)",
                        regs="Z80 register model (AF, BC, DE, HL + alternates, IX, IY, SP, PC)",
                        lineage="Sega Master System main CPU is a Zilog Z80.",
                        src=["https://classic.copetti.org/writings/consoles/master-system/", "https://en.wikipedia.org/wiki/Zilog_Z80"]),
    "sega-game-gear": dict(cache="none", dma=False, mmu=False, era="8-bit handheld main CPU (Z80)",
                        regs="Z80 register model (AF, BC, DE, HL + alternates, IX, IY, SP, PC)",
                        lineage="Sega Game Gear shares Master System hardware with a Zilog Z80 CPU.",
                        src=["https://en.wikipedia.org/wiki/Game_Gear", "https://en.wikipedia.org/wiki/Zilog_Z80"]),
    "sega-genesis": dict(cache="none", dma=True, mmu=False, era="16/32-bit console main CPU (68000)",
                        regs="68000 register model (D0-D7 data, A0-A7 address, PC, SR); Z80 sound sub-CPU",
                        lineage="Sega Genesis main CPU is a Motorola 68000 with a Z80 sound controller.",
                        src=["https://en.wikipedia.org/wiki/Sega_Genesis", "https://segaretro.org/images/1/18/GenesisTechnicalOverview.pdf"]),
    "sega-nomad":  dict(cache="none", dma=True, mmu=False, era="16/32-bit handheld main CPU (68000)",
                        regs="68000 register model (D0-D7, A0-A7, PC, SR); Z80 sound sub-CPU",
                        lineage="Sega Nomad is a portable Genesis; main CPU is a Motorola 68000.",
                        src=["https://en.wikipedia.org/wiki/Sega_Nomad", "https://en.wikipedia.org/wiki/Sega_Genesis"]),
    "sega-mega-cd": dict(cache="none", dma=True, mmu=False, era="16/32-bit add-on CPU (68000)",
                        regs="68000 register model (D0-D7, A0-A7, PC, SR); second 68000 in the Mega-CD unit",
                        lineage="Sega Mega-CD adds a second Motorola 68000 alongside the Genesis 68000.",
                        src=["https://en.wikipedia.org/wiki/Sega_CD", "https://en.wikipedia.org/wiki/Sega_Genesis"]),
    "sega-pico":   dict(cache="none", dma=True, mmu=False, era="16/32-bit edutainment main CPU (68000)",
                        regs="68000 register model (D0-D7, A0-A7, PC, SR)",
                        lineage="Sega Pico is Genesis-class hardware; main CPU is a Motorola 68000.",
                        src=["https://en.wikipedia.org/wiki/Sega_Pico", "https://en.wikipedia.org/wiki/Sega_Genesis"]),
    "sega-32x":    dict(cache="l1", dma=True, mmu=False, era="32-bit add-on CPU (dual SH-2)",
                        regs="Hitachi SH-2 register model (R0-R15, PC, SR, GBR, VBR); two SH-2 cores",
                        lineage="Sega 32X adds two Hitachi SH-2 processors to the Genesis.",
                        src=["https://en.wikipedia.org/wiki/32X", "https://treasure.fandom.com/wiki/Sega_32X/Technical_Specifications"]),
    "sega-saturn": dict(cache="l1", dma=True, mmu=False, era="32-bit console main CPU (dual SH-2)",
                        regs="Hitachi SH-2 register model (R0-R15, PC, SR, GBR, VBR); two SH-2 cores in parallel",
                        lineage="Sega Saturn uses two Hitachi SH-2 processors working in parallel.",
                        src=["https://electronics.howstuffworks.com/dreamcast.htm", "https://en.wikipedia.org/wiki/SuperH"]),
    "sega-dreamcast": dict(cache="l1", dma=True, mmu=True, era="32-bit console main CPU (SH-4)",
                        regs="Hitachi SH-4 register model (R0-R15 banked, PC, SR, FP bank) with on-chip cache and MMU",
                        lineage="Sega Dreamcast main CPU is a Hitachi SH-4.",
                        src=["https://en.wikipedia.org/wiki/Dreamcast", "https://en.wikipedia.org/wiki/SuperH"]),
    "playstation-1": dict(cache="l1", dma=True, mmu=False, era="32-bit console main CPU (MIPS)",
                        regs="MIPS I register model (32 GP, r0 hardwired zero, HI/LO); MIPS R3000A-class",
                        lineage="PlayStation main CPU is a MIPS R3000A-compatible core (LSI CoreWare).",
                        src=["https://en.wikipedia.org/wiki/PlayStation_technical_specifications", "https://www.psdevwiki.com/ps1/CPU"]),
    "playstation-2": dict(cache="l1l2", dma=True, mmu=True, era="64-bit console main CPU (MIPS)",
                        regs="MIPS III/IV register model (32 128-bit GP, r0 zero) in the Emotion Engine",
                        lineage="PlayStation 2 main CPU is the Emotion Engine (MIPS R5900-based).",
                        src=["https://en.wikipedia.org/wiki/Emotion_Engine", "https://en.wikipedia.org/wiki/Comparison_of_PlayStation_consoles"]),
    "playstation-3": dict(cache="l1l2", dma=True, mmu=True, era="64-bit console main CPU (Cell/PowerPC)",
                        regs="Power ISA PPE register model (32 GP, 32 FP, AltiVec) plus SPE array; Cell BE",
                        lineage="PlayStation 3 main CPU is the Cell Broadband Engine (PowerPC PPE + SPEs).",
                        src=["https://en.wikipedia.org/wiki/Cell_(processor)", "https://en.wikipedia.org/wiki/Comparison_of_PlayStation_consoles"]),
    "playstation-4": dict(cache="l1l2", dma=True, mmu=True, era="64-bit console APU (x86-64 Jaguar)",
                        regs="x86-64 register model; two quad-core AMD Jaguar modules (8 cores)",
                        lineage="PlayStation 4 CPU is an 8-core AMD Jaguar (x86-64) in a semi-custom APU.",
                        src=["https://en.wikipedia.org/wiki/PlayStation_4_technical_specifications", "https://en.wikipedia.org/wiki/Jaguar_(microarchitecture)"]),
    "playstation-5": dict(cache="l1l2l3", dma=True, mmu=True, era="64-bit console APU (x86-64 Zen 2)",
                        regs="x86-64 register model; 8-core AMD Zen 2 cluster with shared L3",
                        lineage="PlayStation 5 CPU is an 8-core AMD Zen 2 (x86-64) in a semi-custom APU.",
                        src=["https://www.cpu-monkey.com/en/cpu-sony_playstation_5", "https://en.wikipedia.org/wiki/Zen_2"]),
    "xbox":        dict(cache="l1l2", dma=True, mmu=True, era="32-bit console main CPU (x86 Pentium III)",
                        regs="IA-32 register model; customised Intel Pentium III (Coppermine-based)",
                        lineage="Original Xbox CPU is a customised Intel Pentium III at 733 MHz.",
                        src=["https://en.wikipedia.org/wiki/Xbox_technical_specifications", "https://www.copetti.org/writings/consoles/xbox/"]),
    "xbox-360":    dict(cache="l1l2", dma=True, mmu=True, era="64-bit console main CPU (PowerPC tri-core)",
                        regs="Power ISA register model (32 GP, 32 FP, VMX128) across three SMT2 cores; IBM Xenon",
                        lineage="Xbox 360 CPU is IBM Xenon, a triple-core PowerPC design (PPE-derived).",
                        src=["https://en.wikipedia.org/wiki/Xenon_(processor)", "https://en.wikipedia.org/wiki/Xbox_360_technical_specifications"]),
    "xbox-one":    dict(cache="l1l2", dma=True, mmu=True, era="64-bit console APU (x86-64 Jaguar)",
                        regs="x86-64 register model; two quad-core AMD Jaguar modules (8 cores)",
                        lineage="Xbox One CPU is an 8-core AMD Jaguar (x86-64) in a semi-custom APU.",
                        src=["https://en.wikipedia.org/wiki/Xbox_One", "https://en.wikipedia.org/wiki/Jaguar_(microarchitecture)"]),
    "xbox-one-s":  dict(cache="l1l2", dma=True, mmu=True, era="64-bit console APU (x86-64 Jaguar)",
                        regs="x86-64 register model; 8-core AMD Jaguar at a slightly higher clock than Xbox One",
                        lineage="Xbox One S CPU is an 8-core AMD Jaguar (x86-64), a die-shrink refresh of Xbox One.",
                        src=["https://en.wikipedia.org/wiki/Xbox_One", "https://en.wikipedia.org/wiki/Jaguar_(microarchitecture)"]),
    "xbox-one-x":  dict(cache="l1l2", dma=True, mmu=True, era="64-bit console APU (x86-64 Jaguar)",
                        regs="x86-64 register model; 8-core AMD Jaguar-evolved cluster at a higher clock",
                        lineage="Xbox One X CPU is an 8-core AMD Jaguar-evolved design (x86-64).",
                        src=["https://en.wikipedia.org/wiki/Xbox_One_X", "https://en.wikipedia.org/wiki/Jaguar_(microarchitecture)"]),
    "xbox-series-s": dict(cache="l1l2l3", dma=True, mmu=True, era="64-bit console APU (x86-64 Zen 2)",
                        regs="x86-64 register model; 8-core AMD Zen 2 cluster with shared L3",
                        lineage="Xbox Series S CPU is an 8-core AMD Zen 2 (x86-64) in a semi-custom APU.",
                        src=["https://en.wikipedia.org/wiki/Xbox_Series_X_and_Series_S", "https://en.wikipedia.org/wiki/Zen_2"]),
    "xbox-series-x": dict(cache="l1l2l3", dma=True, mmu=True, era="64-bit console APU (x86-64 Zen 2)",
                        regs="x86-64 register model; 8-core AMD Zen 2 cluster with shared L3",
                        lineage="Xbox Series X CPU is an 8-core AMD Zen 2 (x86-64) in a semi-custom APU.",
                        src=["https://news.xbox.com/en-us/2020/03/16/xbox-series-x-tech/", "https://en.wikipedia.org/wiki/Zen_2"]),
    "atari-2600":  dict(cache="none", dma=False, mmu=False, era="8-bit console main CPU (6507)",
                        regs="6502-class model (A, X, Y, S, PC, P); MOS 6507 with a 13-bit address bus",
                        lineage="Atari 2600 CPU is a MOS 6507, a reduced-pin 6502 with 8 KiB addressing.",
                        src=["https://en.wikipedia.org/wiki/Atari_2600_hardware", "https://en.wikipedia.org/wiki/MOS_Technology_6507"]),
    "atari-5200":  dict(cache="none", dma=True, mmu=False, era="8-bit console main CPU (6502C)",
                        regs="6502-class model (A, X, Y, S, PC, P); MOS 6502C with ANTIC/GTIA DMA",
                        lineage="Atari 5200 CPU is a MOS 6502C (SALLY) as in the Atari 8-bit computers.",
                        src=["https://en.wikipedia.org/wiki/Atari_5200", "https://en.wikipedia.org/wiki/MOS_Technology_6502"]),
    "atari-7800":  dict(cache="none", dma=True, mmu=False, era="8-bit console main CPU (6502C)",
                        regs="6502-class model (A, X, Y, S, PC, P); MOS 6502C (SALLY)",
                        lineage="Atari 7800 CPU is a MOS 6502C (SALLY).",
                        src=["https://en.wikipedia.org/wiki/Atari_7800", "https://en.wikipedia.org/wiki/MOS_Technology_6502"]),
    "atari-xegs":  dict(cache="none", dma=True, mmu=False, era="8-bit console main CPU (6502C)",
                        regs="6502-class model (A, X, Y, S, PC, P); MOS 6502C (SALLY)",
                        lineage="Atari XEGS is a console form of the Atari 8-bit computer; CPU is a MOS 6502C.",
                        src=["https://en.wikipedia.org/wiki/Atari_XEGS", "https://en.wikipedia.org/wiki/MOS_Technology_6502"]),
    "atari-lynx":  dict(cache="none", dma=True, mmu=False, era="8-bit handheld main CPU (65C02)",
                        regs="65C02 programmer model (A, X, Y, S, PC, P) with the Mikey/Suzy custom chips",
                        lineage="Atari Lynx CPU is a 4 MHz WDC 65C02 paired with custom blitter hardware.",
                        src=["https://en.wikipedia.org/wiki/Atari_Lynx", "https://en.wikipedia.org/wiki/WDC_65C02"]),
    "atari-jaguar": dict(cache="none", dma=True, mmu=False, era="32/64-bit console (68000 + RISC)",
                        regs="68000 register model (D0-D7, A0-A7, PC, SR) as the manager CPU plus Tom/Jerry RISC cores",
                        lineage="Atari Jaguar uses a Motorola 68000 manager plus custom Tom/Jerry RISC processors.",
                        src=["https://en.wikipedia.org/wiki/Atari_Jaguar", "http://atarijaguar.freeservers.com/hardware.htm"]),
    "atari-jaguar-cd": dict(cache="none", dma=True, mmu=False, era="32/64-bit console add-on (68000 + RISC)",
                        regs="68000 register model (D0-D7, A0-A7, PC, SR) plus the Jaguar Tom/Jerry RISC cores",
                        lineage="Atari Jaguar CD extends the Jaguar; the same 68000 + Tom/Jerry processors apply.",
                        src=["https://en.wikipedia.org/wiki/Atari_Jaguar_CD", "https://en.wikipedia.org/wiki/Atari_Jaguar"]),
    "atari-vcs":   dict(cache="l1l2", dma=True, mmu=True, era="64-bit console APU (x86-64 AMD Ryzen)",
                        regs="x86-64 register model; AMD Ryzen-embedded (Bristol Ridge-class) APU",
                        lineage="The modern Atari VCS uses an AMD Ryzen-embedded x86-64 APU.",
                        src=["https://en.wikipedia.org/wiki/Atari_VCS_(2021_console)", "https://en.wikipedia.org/wiki/List_of_AMD_processors"]),
}


def style_name(style):
    return {RISC: "RISC general-purpose register file",
            ACC: "accumulator / indexed register machine",
            CISC: "CISC named/split register set"}[style]


def attrs(folder, identity, data, addr, regc, style, zero):
    """Return a dict of derived + reviewed attributes for a folder."""
    a = dict(ATTR.get(folder, {}))
    clock = CLOCK.get(folder, 1)
    modern = data >= 64 or clock >= 100
    a.setdefault("era", ("%d-bit processor" % data))
    # cache default: modern 64-bit/high-clock parts have at least L1/L2
    a.setdefault("cache", "l1l2" if (data >= 64) else ("l1" if clock >= 100 else "none"))
    a.setdefault("dma", bool(data >= 64))
    a.setdefault("mmu", bool(data >= 32 and clock >= 25))
    a.setdefault("regs", "%s with %d addressable registers" % (style_name(style), regc))
    a.setdefault("lineage", "%s. Variant-specific behaviour belongs to a separate profile." % identity)
    a.setdefault("src", [])
    a["clock"] = clock
    a["modern"] = modern
    a["data"] = data
    a["addr"] = addr
    a["regc"] = regc
    a["style"] = style_name(style)
    a["zero"] = zero
    a["identity"] = identity
    a["space"] = human_space(addr)
    return a


def human_space(addr):
    bytes_ = 1 << addr
    for unit, size in (("EiB", 1 << 60), ("PiB", 1 << 50), ("TiB", 1 << 40),
                       ("GiB", 1 << 30), ("MiB", 1 << 20), ("KiB", 1 << 10)):
        if bytes_ >= size:
            v = bytes_ // size
            return "%d %s" % (v, unit)
    return "%d bytes" % bytes_


def cache_rows(cache):
    present = {
        "none":   dict(l1i=False, l1d=False, l2=False, l3=False),
        "l1":     dict(l1i=True,  l1d=True,  l2=False, l3=False),
        "l1l2":   dict(l1i=True,  l1d=True,  l2=True,  l3=False),
        "l1l2l3": dict(l1i=True,  l1d=True,  l2=True,  l3=True),
    }[cache]
    return present


def yn(b):
    return "Yes" if b else "No"


def sources_block(src):
    if not src:
        return ""
    lines = "\n".join("- %s" % u for u in src)
    return "\n## Sources\n\n%s\n" % lines


# =============================== doc bodies ================================

def doc_cpu(a):
    c = cache_rows(a["cache"])
    cache_summary = "None" if a["cache"] == "none" else \
        ("L1" if a["cache"] == "l1" else ("L1 + L2" if a["cache"] == "l1l2" else "L1 + L2 + L3"))
    return """# {identity} CPU

The SLeeLa {identity} profile models the processor as a {era} with a {style} and an architectural, timing-oriented view of its execution.

## Identity

| Field | Value | Evidence |
|---|---|---|
| Family | {identity} | documented |
| Data width | {data}-bit | documented |
| External address width | {addr}-bit | documented |
| Address space | {space} | derived from {addr}-bit addressing |
| Programmer registers | {regs} | documented |
| Rated clock (model) | about {clock} MHz | documented/representative |
| Cache (model) | {cache_summary} | documented/architecture model |
| Integrated DMA | {dma_yn} | documented/architecture model |
| MMU/TLB | {mmu_yn} | documented/architecture model |

The profile is a functional model: enough register, memory and timing structure to run the CPU and host SLeeLa/C/C++ workloads, not a cycle-accurate silicon recreation.

## SLeeLa modeling

The profile composes the shared CPU objects:

- SL{tag}CPU (extends SLCPURuntime)
- SLClock
- SLRegisterFile
- SLALU
- SLInstructionDecoder
- SLIOBus
- SLIOTiming
- SLInterruptController

Cache, DMA and MMU objects are instantiated only where this part actually integrates them (see CACHE.md, DMA.md).

## Variant rule

{lineage} Do not silently merge variant-specific I/O, cache, addressing or opcode behavior into this base model.

See SPECIFICATION.md for the detailed model.
{sources}""".format(
        tag=a["_tag"],
        cache_summary=cache_summary,
        dma_yn=yn(a["dma"]), mmu_yn=yn(a["mmu"]),
        sources=sources_block(a["src"]),
        **a)


def tag_of(identity, a):
    # recover the class tag from the folder's class name list in SPECS
    return a.get("_tag", "")


def doc_specification(a):
    c = cache_rows(a["cache"])
    cache_lines = "\n".join([
        "- L1 instruction cache: %s" % ("present" if c["l1i"] else "none"),
        "- L1 data cache: %s" % ("present" if c["l1d"] else "none"),
        "- L2: %s" % ("present" if c["l2"] else "none"),
        "- L3: %s" % ("present" if c["l3"] else "none"),
    ])
    dma_text = ("The processor/SoC integrates a DMA engine; SLeeLa may instantiate SLDMAController for it."
                if a["dma"] else
                "The base CPU has no integrated DMA controller; DMA is a system-level bus-master concern and SLDMAController is left uninstantiated for the base profile.")
    mmu_text = ("The processor integrates an MMU/TLB; address translation is modeled as virtual -> physical through that unit."
                if a["mmu"] else
                "The base CPU has no MMU/TLB; address translation is CPU address -> external system address, and any mapping belongs to a surrounding system or later derivative.")
    return """# {identity} Specification

## 1. Identity

**Model:** {identity}  
**Architecture class:** {era}  
**Data path:** {data}-bit  
**Addressing:** {addr}-bit external address space ({space})  
**Register model:** {style}

The profile exposes a programmer model while using the shared SLeeLa CPU objects (clock, register file, ALU, decoder, bus) to sequence each instruction.

## 2. Programmer-visible state

{regs}.

The SLeeLa model stores these in SLRegisterFile and exposes {regc} addressable register slots to the runnable model.

## 3. Address and data path

- Data path: {data} bits.
- Address path: {addr} bits.
- Directly addressable space: {space}.
- The model's SLRAM arena is bounded for simulation; the architectural address width is recorded above.

## 4. Clock

Rated model clock: about {clock} MHz (overclockable in the model via setSpecClock).

The model records nominal frequency, cycle number, bus direction, and completion phase. Clock frequency is not instruction latency: one instruction may consume multiple cycles.

## 5. Execution model

The SLeeLa model sequences each instruction as:

1. instruction fetch;
2. opcode decode;
3. operand/address acquisition;
4. ALU/register operation;
5. memory read/write;
6. next-instruction transition.

## 6. Cache

{cache_lines}

cache coherence protocol: {coherence}

## 7. DMA

{dma_text}

## 8. Interrupts

The model records interrupt request, recognition, vector/entry and timing through SLInterruptController without inventing undocumented internal behavior.

## 9. MMU/TLB

{mmu_text}

## 10. Evidence labels

- documented: directly stated by a hardware/reference source.
- derived: mechanically calculated from documented properties.
- estimated: engineering approximation, never presented as silicon fact.
- unspecified: deliberately left unknown.
{sources}""".format(
        cache_lines=cache_lines,
        coherence=("modeled where multi-core" if a["cache"] == "l1l2l3" else "none"),
        dma_text=dma_text, mmu_text=mmu_text,
        sources=sources_block(a["src"]),
        **a)


def doc_architecture(a):
    return """# {identity} Architecture

## Topology

SL{tag}CPU

-> SLClock
-> SLRegisterFile
-> SLALU
-> SLInstructionDecoder
-> SLIOBus
-> SLIOTiming
-> SLInterruptController

### Register section

The architectural register section is a {style}: {regs}.

Internal latches and datapath elements are modeled only where their observable timing or behavior is useful.

### Control section

The control side is represented as:

- instruction register;
- instruction decode;
- control/timing sequencer;
- interrupt logic;
- clock coordination.

### Datapath

registers -> internal bus/latches -> ALU -> register/memory destination

Address formation uses the architectural {addr}-bit address path.

### Bus

The external interface exposes {addr} address lines, a {data}-bit data path, read/write control, clock/phase signals, and reset/interrupt controls. Each observable memory transaction is modeled as a bus event (see BUS.md).

## Design principle

The SLeeLa representation is compositional: a {identity} variant replaces only the couplers, I/O, timing, cache or opcode components that differ, rather than duplicating the whole CPU hierarchy.
{sources}""".format(
        tag=a["_tag"], sources=sources_block(a["src"]), **a)


def doc_registers(a):
    return """# {identity} Registers

The programmer-visible register model is a {style}.

**Registers:** {regs}.

The model exposes {regc} addressable register slots backed by SLRegisterFile. {zero_note}

## Status / condition state

Condition/zero/negative state is tracked by the model's flag fields (flagZ/flagN) and any architecture status register listed above. Flags are updated by ALU and compare operations.

## Internal state

The physical device contains internal latches and buses used during address and data movement. SLeeLa promotes an internal element to a modeled register only when it affects bus visibility, cycle timing, interrupt restart, address formation, or instruction sequencing.
{sources}""".format(
        zero_note=("Register slot %d is modeled as a hardwired zero (reads 0, writes discarded), per the architecture." % a["zero"]
                   if a["zero"] >= 0 else
                   "No register slot is hardwired to zero in this architecture."),
        sources=sources_block(a["src"]), **a)


def doc_bus(a):
    return """# {identity} Bus

## External buses

| Bus | Width | Direction |
|---|---:|---|
| Address | {addr} | CPU output |
| Data | {data} | bidirectional |
| R/W control | 1 | CPU output/control |
| Clock | model clock | timing/control |

## Transaction model

A SLeeLa bus event records cycle number, phase, address, read/write, data, device/coupler, and completion state. Wait/ready behavior is permitted at the system boundary without asserting a specific hardware protocol beyond what is documented.

## Address formation

Addressing modes may require multiple transfers; indexed and indirect operations can produce additional observable bus cycles.

## DMA coupling

{dma_text}
{sources}""".format(
        dma_text=("This part integrates a DMA engine; DMA transfers are modeled as bus-master cycles arbitrated against the CPU."
                  if a["dma"] else
                  "External DMA is represented as a system-level bus master; the base CPU does not instantiate a DMA engine."),
        sources=sources_block(a["src"]), **a)


def doc_cache(a):
    c = cache_rows(a["cache"])
    rows = "\n".join([
        "| L1 instruction | %s | %s |" % (yn(c["l1i"]), "present" if c["l1i"] else "absent"),
        "| L1 data | %s | %s |" % (yn(c["l1d"]), "present" if c["l1d"] else "absent"),
        "| L2 | %s | %s |" % (yn(c["l2"]), "present" if c["l2"] else "absent"),
        "| L3 | %s | %s |" % (yn(c["l3"]), "present" if c["l3"] else "absent"),
    ])
    if a["cache"] == "none":
        consequence = ("A memory access is modeled directly through the processor's bus interface; "
                       "there is no cache-hit path that bypasses the external memory cycle. CPU and "
                       "bus cycles remain observable rather than hidden behind a cache abstraction.")
    else:
        consequence = ("The model records the presence of the cache levels above. The functional model "
                       "accesses memory through the bus interface; cache levels are represented as "
                       "architectural facts and timing hints rather than a cycle-accurate replacement policy.")
    return """# {identity} Cache

## Cache hierarchy

| Level | Present | SLeeLa model |
|---|---|---|
{rows}

## Timing consequence

{consequence}

## Variant rule

Do not attach cache objects that this part does not actually contain, and do not remove levels it does. A different SKU or derivative with a different cache configuration is a separate profile.
{sources}""".format(rows=rows, consequence=consequence,
                     sources=sources_block(a["src"]), **a)


def doc_dma(a):
    if a["dma"]:
        body = """## Base CPU / SoC

This part integrates DMA capability.

SLeeLa may instantiate SLDMAController and arbitrate its transfers against CPU bus access:

SLDMAController -> SLBusArbiter -> SLIOBus

## Modeling rule

Model only the DMA facts that are documented for this part (presence, and where documented, channels and arbitration). Do not invent channel counts, descriptor registers, or burst sizes that are not documented."""
    else:
        body = """## Base CPU

The base {identity} does not contain an integrated DMA controller.

SLDMAController is therefore absent from the base CPU object graph.

## System DMA

A complete system may still contain DMA-capable peripherals or external logic, represented separately:

SLDMAController -> SLBusArbiter -> SLIOBus

The CPU is a bus participant; DMA is a system-level bus-master concern.

## Modeling rule

Do not invent DMA channel counts, descriptor registers, burst sizes, or arbitration policies for the CPU itself. Those belong to a specific machine, chipset or derivative.""".format(**a)
    return "# %s DMA\n\n%s\n%s" % (a["identity"], body, sources_block(a["src"]))


def doc_timing(a):
    return """# {identity} Timing

## Clock model

The model treats the clock as an event source rated at about {clock} MHz, not as "one instruction per clock."

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
{sources}""".format(sources=sources_block(a["src"]), **a)


def doc_circuit(a):
    return """# {identity} Circuit Model

## Scope

This document describes the architectural circuit blocks SLeeLa models reliably for the {identity}. It is not a transistor-for-transistor recreation.

## Major blocks

1. **Register section**
   - {regs}

2. **ALU**
   - arithmetic;
   - logic;
   - shifts/rotates;
   - flag generation.

3. **Address path**
   - {addr}-bit address formation;
   - address latches;
   - effective-address computation.

4. **Data path**
   - {data}-bit data latch;
   - internal data movement;
   - external data bus buffer.

5. **Control**
   - instruction register;
   - instruction decode;
   - timing control;
   - interrupt logic.

6. **Clock**
   - clock input;
   - phase sequencing.

## Accuracy boundary

Gate-level or transistor-level detail should be added only from sufficiently reliable die analysis or a validated hardware recreation. This profile stays at the architectural/behavioral/timing level.
{sources}""".format(sources=sources_block(a["src"]), **a)


BUILDERS = {
    "CPU.md": doc_cpu,
    "SPECIFICATION.md": doc_specification,
    "ARCHITECTURE.md": doc_architecture,
    "REGISTERS.md": doc_registers,
    "BUS.md": doc_bus,
    "CACHE.md": doc_cache,
    "DMA.md": doc_dma,
    "TIMING.md": doc_timing,
    "CIRCUIT.md": doc_circuit,
}


def has_docs(folder_path):
    try:
        return any(n.endswith(".md") for n in os.listdir(folder_path))
    except FileNotFoundError:
        return False


def main():
    check = "--check" in sys.argv
    force = "--force" in sys.argv

    written, skipped, missing = [], [], []
    for spec in SPECS:
        folder, filename, cls, identity, data, addr, regc, style, zero = spec
        folder_path = os.path.join(CPU, folder)
        if not os.path.isdir(folder_path):
            missing.append(folder)
            continue
        if has_docs(folder_path) and not force:
            skipped.append(folder)
            continue
        a = attrs(folder, identity, data, addr, regc, style, zero)
        # class tag, e.g. SL6502CPU -> "6502" portion used in topology lines
        a["_tag"] = cls[2:-3] if cls.startswith("SL") and cls.endswith("CPU") else cls
        for name in DOC_NAMES:
            text = BUILDERS[name](a)
            if not text.endswith("\n"):
                text += "\n"
            if not check:
                with open(os.path.join(folder_path, name), "w") as f:
                    f.write(text)
        written.append(folder)

    print("doc generation: %d folder(s) %s, %d already had docs (skipped), %d spec folders missing on disk"
          % (len(written), "to write" if check else "written", len(skipped), len(missing)))
    if written:
        print("  %s:" % ("would write" if check else "wrote"))
        for w in written:
            print("    lib/cpu/%s/  (+9 .md)" % w)
    if missing:
        print("  missing folders: %s" % ", ".join(missing))


if __name__ == "__main__":
    main()
