# AMD Ryzen Threadripper 9000 (Zen 5 x86-64) Specification

## 1. Identity

**Model:** AMD Ryzen Threadripper 9000 ("Shimada Peak", Zen 5 x86-64)
**Architecture class:** 64-bit many-core HEDT / workstation microprocessor
**Data path:** 64-bit
**Addressing:** 48-bit external address space (256 TiB)
**Register model:** CISC named/split register set
**Process:** 4 nm "Zen 5"
**Socket:** sTR5 (TRX50 for HEDT, WRX90 for PRO WX)

The profile exposes a programmer model while using the shared SLeeLa CPU objects (clock, register file, ALU, decoder, bus) to sequence each instruction.

## 2. SKU roster (Zen 5 "Shimada Peak", launched July 2025)

### High-End Desktop (HEDT) — 4-channel DDR5-6400, sTR5/TRX50

| Model | Cores / Threads | Base clock | Max boost | L3 cache | TDP | Evidence |
|---|---:|---:|---:|---:|---:|---|
| Threadripper 9980X | 64 / 128 | ~3.2 GHz | 5.4 GHz | 256 MB | 350 W | documented |
| Threadripper 9970X | 32 / 64  | 4.0 GHz  | 5.4 GHz | 128 MB | 350 W | documented |
| Threadripper 9960X | 24 / 48  | 4.2 GHz  | 5.4 GHz | 128 MB | 350 W | documented |

HEDT PCIe: up to 88 PCIe 5.0 lanes (platform-dependent).

### Workstation PRO WX-Series — 8-channel DDR5-6400, sTR5/WRX90, up to 128 PCIe 5.0 lanes

| Model | Cores / Threads | Max boost | L3 cache | TDP | Evidence |
|---|---:|---:|---:|---:|---|
| Threadripper PRO 9995WX | 96 / 192 | 5.4 GHz | 384 MB | 350 W | documented |
| Threadripper PRO 9985WX | 64 / 128 | 5.4 GHz | 256 MB | 350 W | documented |
| Threadripper PRO 9975WX | 32 / 64  | 5.4 GHz | 128 MB | 350 W | documented |
| Threadripper PRO 9965WX | 24 / 48  | 5.4 GHz | 128 MB | 350 W | documented |
| Threadripper PRO 9955WX | 16 / 32  | 5.4 GHz | 64 MB  | 350 W | documented |
| Threadripper PRO 9945WX | 12 / 24  | 5.4 GHz | 64 MB  | 350 W | documented |

Each SKU above is its own runnable class (SLAMDThreadripper&lt;model&gt; / SLAMDThreadripperPro&lt;model&gt;) extending SLAMDThreadripperCPU; they share the Zen 5 x86-64 programmer model and decode() and differ in rated clock and identity label. All rate their spec clock at the 5.4 GHz family max boost; a specific SKU's base clock is a configuration of the same model via setSpecClock.

## 3. Programmer-visible state

x86-64 register model (Zen 5 microarchitecture) with per-core L1/L2 and shared L3 per CCD, aggregated across multiple CCDs on an I/O die.

The SLeeLa model stores these in SLRegisterFile and exposes 16 addressable register slots to the runnable model.

## 4. Address and data path

- Data path: 64 bits.
- Address path: 48 bits.
- Directly addressable space: 256 TiB.
- The model's SLRAM arena is bounded for simulation; the architectural address width is recorded above.

## 5. Clock

Rated model clock: about 5400 MHz max boost (overclockable in the model via setSpecClock).

The model records nominal frequency, cycle number, bus direction, and completion phase. Clock frequency is not instruction latency: one instruction may consume multiple cycles.

## 6. Execution model

The SLeeLa model sequences each instruction as:

1. instruction fetch;
2. opcode decode;
3. operand/address acquisition;
4. ALU/register operation;
5. memory read/write;
6. next-instruction transition.

## 7. Cache

- L1 instruction cache: present
- L1 data cache: present
- L2: present (per-core)
- L3: present (shared per CCD; 64–384 MB across the SKU roster)

cache coherence protocol: modeled where multi-core

## 8. DMA

The base CPU has no integrated DMA controller; DMA is a system-level bus-master concern and SLDMAController is left uninstantiated for the base profile.

## 9. Interrupts

The model records interrupt request, recognition, vector/entry and timing through SLInterruptController without inventing undocumented internal behavior.

## 10. MMU/TLB

The processor integrates an MMU/TLB; address translation is modeled as virtual -> physical through that unit.

## 11. Evidence labels

- documented: directly stated by a hardware/reference source.
- derived: mechanically calculated from documented properties.
- estimated: engineering approximation, never presented as silicon fact.
- unspecified: deliberately left unknown.

## Sources

- https://en.wikipedia.org/wiki/Zen_5
- https://en.wikipedia.org/wiki/Ryzen_Threadripper
- https://en.wikipedia.org/wiki/List_of_AMD_Ryzen_processors
- https://www.amd.com/en/blogs/2025/designed-to-create-built-to-inspire-amd-introduces-new.html
- https://www.amd.com/en/blogs/2025/amd-introduces-new-zen-5-based-ryzen-threadripper-pro.html
