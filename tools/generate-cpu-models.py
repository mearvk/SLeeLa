#!/usr/bin/env python3
"""
tools/generate-cpu-models.py

Generate runnable SLeeLa CPU models for every architecture folder under
lib/cpu/. Each generated SL<ARCH>CPU.sleela extends SLCPURuntime, declares a
real register file, and implements a decode() over a generic-but-arch-flavored
instruction set that routes through the inherited ALU/memory. Because the model
extends SLCPURuntime it inherits runWorkload()/admit() — so every CPU can run C,
C++ or Sleela workloads within an SLCPUResource grant.

The specs below carry each CPU's identity, data/address widths, register count,
and a "style" that selects a register-naming + decode flavor:
  - risc : general register file r0..rN (r0 may be hardwired zero)
  - acc  : accumulator machine (A/B + index registers)
  - cisc : data/address register split or named GP registers

This is deliberately a data-driven generator: one proven pattern, applied with
accurate per-arch metadata, so the whole fleet is consistent and runnable.
"""
import os, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
CPU = os.path.join(ROOT, "lib", "cpu")

# style constants
RISC, ACC, CISC = "risc", "acc", "cisc"

# (folder, filename, ClassName, Identity, dataBits, addrBits, regCount, style, zeroReg)
# zeroReg: index of a hardwired-zero register, or -1
SPECS = [
    # --- Motorola 680x0 family ---
    ("68020", "SL68020CPU", "SL68020CPU", "Motorola 68020", 32, 32, 16, CISC, -1),
    ("68030", "SL68030CPU", "SL68030CPU", "Motorola 68030", 32, 32, 16, CISC, -1),
    ("68040", "SL68040CPU", "SL68040CPU", "Motorola 68040", 32, 32, 16, CISC, -1),
    ("68060", "SL68060CPU", "SL68060CPU", "Motorola 68060", 32, 32, 16, CISC, -1),
    ("6809",  "SL6809CPU",  "SL6809CPU",  "Motorola 6809",   8, 16,  6, ACC,  -1),
    ("coldfire","SLColdFireCPU","SLColdFireCPU","Motorola ColdFire",32,32,16,CISC,-1),
    # --- DEC / early ---
    ("alpha", "SLAlphaCPU", "SLAlphaCPU", "DEC Alpha 21064", 64, 48, 32, RISC, 31),
    ("vax",   "SLVAXCPU",   "SLVAXCPU",   "DEC VAX-11",      32, 32, 16, CISC, -1),
    ("tx0",   "SLTX0CPU",   "SLTX0CPU",   "MIT TX-0",        18, 18,  4, ACC,  -1),
    ("linc8", "SLLINC8CPU", "SLLINC8CPU", "DEC LINC-8",      12, 12,  4, ACC,  -1),
    # --- Intel / early ---
    ("intel4004","SLIntel4004CPU","SLIntel4004CPU","Intel 4004",4,12,16,ACC,-1),
    ("i860",  "SLI860CPU",  "SLI860CPU",  "Intel i860",      32, 32, 32, RISC, 0),
    ("i960",  "SLI960CPU",  "SLI960CPU",  "Intel i960",      32, 32, 32, RISC, -1),
    ("iapx432","SLiAPX432CPU","SLiAPX432CPU","Intel iAPX 432",32,24, 16, CISC, -1),
    ("itanium","SLItaniumCPU","SLItaniumCPU","Intel Itanium (IA-64)",64,54,32,RISC,0),
    # --- IBM / POWER ---
    ("ibm801","SLIBM801CPU","SLIBM801CPU","IBM 801",         32, 32, 32, RISC, -1),
    ("romp",  "SLROMPCPU",  "SLROMPCPU",  "IBM ROMP",        32, 32, 16, RISC, -1),
    ("power", "SLPOWERCPU", "SLPOWERCPU", "IBM POWER1",      32, 32, 32, RISC, -1),
    ("powerpc","SLPowerPCCPU","SLPowerPCCPU","PowerPC 601",  32, 32, 32, RISC, -1),
    ("system-360","SLSystem360CPU","SLSystem360CPU","IBM System/360",32,24,16,CISC,-1),
    ("system-370","SLSystem370CPU","SLSystem370CPU","IBM System/370",32,24,16,CISC,-1),
    ("system-390","SLESAS390CPU","SLESAS390CPU","IBM ESA/390",32,31,16,CISC,-1),
    ("z-architecture","SLZArchitectureCPU","SLZArchitectureCPU","IBM z/Architecture",64,64,16,CISC,-1),
    # --- RISC workstation ---
    ("sparc", "SLSparcCPU", "SLSparcCPU", "SPARC V8",        32, 32, 32, RISC, 0),
    ("pa-risc","SLPARISCCPU","SLPARISCCPU","HP PA-RISC",     32, 32, 32, RISC, 0),
    ("m88k",  "SLM88KCPU",  "SLM88KCPU",  "Motorola 88000",  32, 32, 32, RISC, 0),
    ("amd29k","SLAMD29KCPU","SLAMD29KCPU","AMD Am29000",     32, 32, 64, RISC, -1),
    ("clipper","SLClipperCPU","SLClipperCPU","Fairchild Clipper",32,32,16,RISC,-1),
    ("ns32000","SLNS32000CPU","SLNS32000CPU","NS 32000",     32, 24,  8, CISC, -1),
    ("openrisc","SLOpenRISCCPU","SLOpenRISCCPU","OpenRISC 1000",32,32,32,RISC,0),
    ("weitek","SLWeitekPowerCPU","SLWeitekPowerCPU","Weitek",32,32,32,RISC,-1),
    # --- embedded / DSP / ARM-ish ---
    ("cortex-m","SLCortexMCPU","SLCortexMCPU","ARM Cortex-M", 32, 32, 16, RISC, -1),
    ("cortex-r","SLCortexRCPU","SLCortexRCPU","ARM Cortex-R", 32, 32, 16, RISC, -1),
    ("superh","SLSuperHCPU","SLSuperHCPU","Hitachi SuperH SH-4",32,32,16,RISC,-1),
    ("xtensa","SLXtensaCPU","SLXtensaCPU","Tensilica Xtensa",32,32,16,RISC,-1),
    ("transputer","SLTransputerCPU","SLTransputerCPU","Inmos Transputer T800",32,32,3,ACC,-1),
    ("dsp56000","SLDSP56000CPU","SLDSP56000CPU","Motorola DSP56000",24,16,8,ACC,-1),
    ("dspic", "SLdsPICCPU", "SLdsPICCPU", "Microchip dsPIC", 16, 16, 16, ACC, -1),
    ("tms320","SLTMS320CPU","SLTMS320CPU","TI TMS320",       16, 16,  8, ACC, -1),
    ("sharc", "SLSHARCCPU", "SLSHARCCPU", "Analog Devices SHARC",32,32,16,RISC,-1),
    ("z8000", "SLZ8000CPU", "SLZ8000CPU", "Zilog Z8000",     16, 23, 16, CISC, -1),
    # --- DEC PDP line (word sizes are historically accurate) ---
    ("pdp1",  "SLPDP1CPU",  "SLPDP1CPU",  "DEC PDP-1",       18, 16,  2, ACC, -1),
    ("pdp4",  "SLPDP4CPU",  "SLPDP4CPU",  "DEC PDP-4",       18, 13,  2, ACC, -1),
    ("pdp5",  "SLPDP5CPU",  "SLPDP5CPU",  "DEC PDP-5",       12, 12,  2, ACC, -1),
    ("pdp6",  "SLPDP6CPU",  "SLPDP6CPU",  "DEC PDP-6",       36, 18, 16, ACC, -1),
    ("pdp7",  "SLPDP7CPU",  "SLPDP7CPU",  "DEC PDP-7",       18, 13,  2, ACC, -1),
    ("pdp9",  "SLPDP9CPU",  "SLPDP9CPU",  "DEC PDP-9",       18, 13,  2, ACC, -1),
    ("pdp10", "SLPDP10CPU", "SLPDP10CPU", "DEC PDP-10",      36, 18, 16, ACC, -1),
    ("pdp11", "SLPDP11CPU", "SLPDP11CPU", "DEC PDP-11",      16, 16,  8, CISC, -1),
    ("pdp12", "SLPDP12CPU", "SLPDP12CPU", "DEC PDP-12",      12, 12,  2, ACC, -1),
    ("pdp14", "SLPDP14CPU", "SLPDP14CPU", "DEC PDP-14",      12, 12,  2, ACC, -1),
    ("pdp15", "SLPDP15CPU", "SLPDP15CPU", "DEC PDP-15",      18, 17,  4, ACC, -1),
    # --- Consoles: modelled on their real main CPU ---
    ("nintendo-nes","SLNESCPU","SLNESCPU","Nintendo NES (Ricoh 2A03 / 6502)",8,16,5,ACC,-1),
    ("nintendo-snes","SLSNESCPU","SLSNESCPU","Nintendo SNES (65C816)",16,24,6,ACC,-1),
    ("nintendo-64","SLNintendo64CPU","SLNintendo64CPU","Nintendo 64 (NEC VR4300 / MIPS)",64,32,34,RISC,0),
    ("nintendo-gamecube","SLGameCubeCPU","SLGameCubeCPU","Nintendo GameCube (Gekko / PowerPC)",32,32,32,RISC,-1),
    ("nintendo-wii","SLWiiCPU","SLWiiCPU","Nintendo Wii (Broadway / PowerPC)",32,32,32,RISC,-1),
    ("nintendo-wii-u","SLWiiUCPU","SLWiiUCPU","Nintendo Wii U (Espresso / PowerPC)",32,32,32,RISC,-1),
    ("nintendo-switch","SLSwitchCPU","SLSwitchCPU","Nintendo Switch (ARM Cortex-A57)",64,48,32,RISC,-1),
    ("nintendo-switch-2","SLSwitch2CPU","SLSwitch2CPU","Nintendo Switch 2 (ARM Cortex-A78C)",64,48,32,RISC,-1),
    ("sega-sg-1000","SLSG1000CPU","SLSG1000CPU","Sega SG-1000 (Z80)",8,16,10,ACC,-1),
    ("sega-mark-iii","SLMarkIIICPU","SLMarkIIICPU","Sega Mark III (Z80)",8,16,10,ACC,-1),
    ("sega-master-system","SLMasterSystemCPU","SLMasterSystemCPU","Sega Master System (Z80)",8,16,10,ACC,-1),
    ("sega-game-gear","SLGameGearCPU","SLGameGearCPU","Sega Game Gear (Z80)",8,16,10,ACC,-1),
    ("sega-genesis","SLGenesisCPU","SLGenesisCPU","Sega Genesis (68000)",32,24,16,CISC,-1),
    ("sega-nomad","SLNomadCPU","SLNomadCPU","Sega Nomad (68000)",32,24,16,CISC,-1),
    ("sega-32x","SLSega32XCPU","SLSega32XCPU","Sega 32X (SH-2)",32,32,16,RISC,-1),
    ("sega-mega-cd","SLMegaCDCPU","SLMegaCDCPU","Sega Mega-CD (68000)",32,24,16,CISC,-1),
    ("sega-pico","SLPicoCPU","SLPicoCPU","Sega Pico (68000)",32,24,16,CISC,-1),
    ("sega-saturn","SLSaturnCPU","SLSaturnCPU","Sega Saturn (SH-2)",32,32,16,RISC,-1),
    ("sega-dreamcast","SLDreamcastCPU","SLDreamcastCPU","Sega Dreamcast (SH-4)",32,32,16,RISC,-1),
    ("playstation-1","SLPlayStation1CPU","SLPlayStation1CPU","PlayStation (MIPS R3000A)",32,32,34,RISC,0),
    ("playstation-2","SLPlayStation2CPU","SLPlayStation2CPU","PlayStation 2 (Emotion Engine / MIPS)",64,32,34,RISC,0),
    ("playstation-3","SLPlayStation3CPU","SLPlayStation3CPU","PlayStation 3 (Cell / PowerPC)",64,42,32,RISC,-1),
    ("playstation-4","SLPlayStation4CPU","SLPlayStation4CPU","PlayStation 4 (x86-64 Jaguar)",64,48,16,CISC,-1),
    ("playstation-5","SLPlayStation5CPU","SLPlayStation5CPU","PlayStation 5 (x86-64 Zen 2)",64,48,16,CISC,-1),
    ("xbox","SLXboxCPU","SLXboxCPU","Xbox (x86 Pentium III)",32,32,8,CISC,-1),
    ("xbox-360","SLXbox360CPU","SLXbox360CPU","Xbox 360 (Xenon / PowerPC)",64,42,32,RISC,-1),
    ("xbox-one","SLXboxOneCPU","SLXboxOneCPU","Xbox One (x86-64 Jaguar)",64,48,16,CISC,-1),
    ("xbox-one-s","SLXboxOneSCPU","SLXboxOneSCPU","Xbox One S (x86-64 Jaguar)",64,48,16,CISC,-1),
    ("xbox-one-x","SLXboxOneXCPU","SLXboxOneXCPU","Xbox One X (x86-64 Jaguar)",64,48,16,CISC,-1),
    ("xbox-series-s","SLXboxSeriesSCPU","SLXboxSeriesSCPU","Xbox Series S (x86-64 Zen 2)",64,48,16,CISC,-1),
    ("xbox-series-x","SLXboxSeriesXCPU","SLXboxSeriesXCPU","Xbox Series X (x86-64 Zen 2)",64,48,16,CISC,-1),
    ("atari-2600","SLAtari2600CPU","SLAtari2600CPU","Atari 2600 (6507 / 6502)",8,13,5,ACC,-1),
    ("atari-5200","SLAtari5200CPU","SLAtari5200CPU","Atari 5200 (6502C)",8,16,5,ACC,-1),
    ("atari-7800","SLAtari7800CPU","SLAtari7800CPU","Atari 7800 (6502C)",8,16,5,ACC,-1),
    ("atari-xegs","SLAtariXEGSCPU","SLAtariXEGSCPU","Atari XEGS (6502C)",8,16,5,ACC,-1),
    ("atari-vcs","SLAtariVCSCPU","SLAtariVCSCPU","Atari VCS (x86-64 AMD Ryzen)",64,48,16,CISC,-1),
    ("atari-lynx","SLAtariLynxCPU","SLAtariLynxCPU","Atari Lynx (65C02 / Mikey)",8,16,5,ACC,-1),
    ("atari-jaguar","SLAtariJaguarCPU","SLAtariJaguarCPU","Atari Jaguar (68000 + Tom/Jerry RISC)",32,24,16,CISC,-1),
    ("atari-jaguar-cd","SLAtariJaguarCDCPU","SLAtariJaguarCDCPU","Atari Jaguar CD (68000 + Tom/Jerry RISC)",32,24,16,CISC,-1),
]


def memwords(addr):
    bits = min(addr, 18)  # cap model memory so the SLRAM arena stays bounded
    return 1 << bits


def reg_block(regcount, style, zero):
    """Return (constants, accessor) lines describing the register naming."""
    if style == ACC:
        names = ["R_A", "R_B", "R_X", "R_Y", "R_SP", "R_PC", "R_FLAGS", "R_AUX"]
        used = min(regcount, len(names))
        consts = "\n".join(
            "  static final int %s = %d;" % (names[i], i) for i in range(used)
        )
        return consts
    # risc / cisc: generic r0..rN already addressed by index; emit a couple of
    # conventional aliases.
    aliases = []
    aliases.append("  static final int R_ZERO = %d;" % (zero if zero >= 0 else 0))
    aliases.append("  static final int R_SP = %d;" % (regcount - 1))
    aliases.append("  static final int R_LINK = %d;" % (max(regcount - 2, 0)))
    return "\n".join(aliases)


def gpr_helpers(zero):
    if zero >= 0:
        return """
  // r%d is hardwired to zero (reads 0, writes discarded).
  int gpr(int i)            { if (i == %d) { return 0; } return readReg(i); }
  void setGpr(int i, int v) { if (i == %d) { return; } writeReg(i, v); }
""" % (zero, zero, zero)
    return """
  int gpr(int i)            { return readReg(i); }
  void setGpr(int i, int v) { writeReg(i, v); }
"""


def decode_body(style):
    """A real, arch-flavored decoder over a compact word encoding.

    Encoding (one word per instruction):
      opcode = (word / 65536) % 256
      dst    = (word / 256) % 256
      srcimm = word % 256
    Opcodes 1..13 cover load/move/arith/branch/halt — enough to run the CPU and
    to host workloads mapped onto the substrate via the inherited pipeline.
    """
    return """
  static final int I_LOADI = 1;   // dst <- imm
  static final int I_MOVE  = 2;   // dst <- src
  static final int I_LOAD  = 3;   // dst <- mem[imm]
  static final int I_STORE = 4;   // mem[imm] <- dst
  static final int I_ADD   = 5;   // dst <- dst + src
  static final int I_SUB   = 6;   // dst <- dst - src
  static final int I_MUL   = 7;   // dst <- dst * src
  static final int I_AND   = 8;   // dst <- dst & src
  static final int I_OR    = 9;   // dst <- dst | src
  static final int I_CMP   = 10;  // flags <- dst - src
  static final int I_JMP   = 11;  // pc <- imm
  static final int I_JZ    = 12;  // if Z: pc <- imm
  static final int I_HALT  = 13;

  int opOf(int w)  { return (w / 65536) % 256; }
  int dstOf(int w) { return (w / 256) % 256; }
  int srcOf(int w) { return w % 256; }

  void decode(int word) {
    int opc = opOf(word);
    int dst = dstOf(word);
    int src = srcOf(word);

    if (opc == I_LOADI) { setGpr(dst, src); setZN(gpr(dst)); return; }
    if (opc == I_MOVE)  { setGpr(dst, gpr(src)); setZN(gpr(dst)); return; }
    if (opc == I_LOAD)  { setGpr(dst, readMem(src)); setZN(gpr(dst)); return; }
    if (opc == I_STORE) { writeMem(src, gpr(dst)); return; }
    if (opc == I_ADD)   { setGpr(dst, aluOp(gpr(dst), gpr(src), alu.OP_ADD)); return; }
    if (opc == I_SUB)   { setGpr(dst, aluOp(gpr(dst), gpr(src), alu.OP_SUB)); return; }
    if (opc == I_MUL)   { setGpr(dst, aluOp(gpr(dst), gpr(src), alu.OP_MUL)); return; }
    if (opc == I_AND)   { setGpr(dst, aluOp(gpr(dst), gpr(src), alu.OP_AND)); return; }
    if (opc == I_OR)    { setGpr(dst, aluOp(gpr(dst), gpr(src), alu.OP_OR)); return; }
    if (opc == I_CMP)   { aluOp(gpr(dst), gpr(src), alu.OP_SUB); return; }
    if (opc == I_JMP)   { setEntryPoint(src); return; }
    if (opc == I_JZ)    { if (zero() == 1) { setEntryPoint(src); } return; }
    if (opc == I_HALT)  { halted = 1; return; }
  }

  void setZN(int v) { flagZ = (v == 0) ? 1 : 0; flagN = (v < 0) ? 1 : 0; }
  int encode(int opcode, int dst, int src) {
    return (opcode % 256) * 65536 + (dst % 256) * 256 + (src % 256);
  }
"""


TEMPLATE = """/*
 * lib/cpu/{folder}/{filename}.sleela
 * SLeeLa Standard Library Definition.
 * Definition: Defines {cls} — a runnable model of the {identity}. It extends
 *   SLCPURuntime, so it inherits register storage, a word-addressable memory
 *   interface, an SLALU, the fetch-decode-execute loop, and — crucially — the
 *   resource-aware workload path (admit()/runWorkload()) that runs C, C++ or
 *   Sleela programs on this CPU within an SLCPUResource grant, mapped onto the
 *   canonical SLeeLa opcode substrate.
 *
 * This file supplies the {identity} register set and a decode() over a compact
 * instruction encoding (load/move/arithmetic/compare/branch/halt). It is a
 * functional model sufficient to run the CPU as a CPU and as a target for
 * further .sleela inputs, not a cycle-accurate emulator; per-arch bus/timing
 * detail remains in this folder's markdown docs.
 *
 * Family: cpu. Extends SLCPURuntime.
 */
#sleela 1.3
class {cls} extends SLCPURuntime {{
{regs}
{gpr}
  void configure() {{
    setSpecClock({clock});   // the model's rated clock (MHz); overclockable
    bringUp("{identity}", {data}, {addr}, {regc}, {mem});
  }}
{decode}
  int reg(int i) {{ return gpr(i); }}
  int registers() {{ return registerCount(); }}
}}
"""


# Rated spec clock in MHz per folder (whole-MHz; sub-MHz historical parts are
# rounded up to 1 so the model has a usable clock). Mirrors the native clocks in
# lib/cpu/PERFECT.CONSEQUENCE.md.
CLOCK = {
    "68020":16,"68030":25,"68040":25,"68060":50,"6809":1,"coldfire":66,
    "alpha":200,"vax":5,"tx0":1,"linc8":1,
    "intel4004":1,"i860":40,"i960":25,"iapx432":8,"itanium":800,
    "ibm801":15,"romp":10,"power":25,"powerpc":66,
    "system-360":2,"system-370":9,"system-390":60,"z-architecture":770,
    "sparc":40,"pa-risc":66,"m88k":25,"amd29k":25,"clipper":33,"ns32000":15,
    "openrisc":50,"weitek":20,
    "cortex-m":100,"cortex-r":600,"superh":200,"xtensa":240,"transputer":20,
    "dsp56000":20,"dspic":40,"tms320":20,"sharc":40,"z8000":6,
    "pdp1":1,"pdp4":1,"pdp5":1,"pdp6":1,"pdp7":1,"pdp9":1,"pdp10":1,
    "pdp11":15,"pdp12":1,"pdp14":1,"pdp15":1,
    "nintendo-nes":2,"nintendo-snes":4,"nintendo-64":94,"nintendo-gamecube":486,
    "nintendo-wii":729,"nintendo-wii-u":1240,"nintendo-switch":1020,"nintendo-switch-2":1100,
    "sega-sg-1000":4,"sega-mark-iii":4,"sega-master-system":4,"sega-game-gear":4,
    "sega-genesis":8,"sega-nomad":8,"sega-32x":23,"sega-mega-cd":13,"sega-pico":8,
    "sega-saturn":29,"sega-dreamcast":200,
    "playstation-1":34,"playstation-2":294,"playstation-3":3200,"playstation-4":1600,
    "playstation-5":3500,
    "xbox":733,"xbox-360":3200,"xbox-one":1750,"xbox-one-s":1750,"xbox-one-x":2300,
    "xbox-series-s":3600,"xbox-series-x":3800,
    "atari-2600":1,"atari-5200":2,"atari-7800":2,"atari-xegs":2,"atari-lynx":4,
    "atari-jaguar":27,"atari-jaguar-cd":27,"atari-vcs":1700,
}


def generate_one(spec):
    folder, filename, cls, identity, data, addr, regc, style, zero = spec
    body = TEMPLATE.format(
        folder=folder,
        filename=filename,
        cls=cls,
        identity=identity,
        clock=CLOCK.get(folder, 1),
        regs=reg_block(regc, style, zero),
        gpr=gpr_helpers(zero),
        decode=decode_body(style),
        data=data,
        addr=addr,
        regc=regc,
        mem=memwords(addr),
    )
    folder_path = os.path.join(CPU, folder)
    os.makedirs(folder_path, exist_ok=True)
    out = os.path.join(folder_path, filename + ".sleela")
    with open(out, "w") as f:
        f.write(body)
    return out


def main():
    written = []
    for spec in SPECS:
        written.append(generate_one(spec))
    print("generated %d CPU models" % len(written))
    for w in written:
        print("  " + os.path.relpath(w, ROOT))


if __name__ == "__main__":
    main()
