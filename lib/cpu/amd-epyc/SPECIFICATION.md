# AMD EPYC (Zen x86-64) Specification

## 1. Identity

**Model:** AMD EPYC (Zen x86-64)  
**Architecture class:** 64-bit server multi-core microprocessor  
**Data path:** 64-bit  
**Addressing:** 48-bit external address space (256 TiB)  
**Register model:** CISC named/split register set

The profile exposes a programmer model while using the shared SLeeLa CPU objects (clock, register file, ALU, decoder, bus) to sequence each instruction.

## 2. SKU roster — EPYC 9005 "Turin" (Zen 5 / Zen 5c, launched Oct 2024)

Socket SP5, 12-channel DDR5, PCIe 5.0, up to 500 W. Each SKU below is its own runnable class (SLAMDEPYC&lt;model&gt;) extending SLAMDEPYCCPU; they share the Zen x86-64 programmer model and differ in rated clock and identity.

| Model | Cores / Threads | Base | Max boost | L3 cache | TDP | Core type | Evidence |
|---|---:|---:|---:|---:|---:|---|---|
| EPYC 9965  | 192 / 384 | 2.25 GHz | 3.7 GHz | 384 MB | 500 W | Zen 5c | documented |
| EPYC 9755  | 128 / 256 | 2.7 GHz  | 4.1 GHz | 512 MB | 500 W | Zen 5  | documented |
| EPYC 9645  | 96 / 192  | 2.3 GHz  | 3.7 GHz | 256 MB | 400 W | Zen 5c | documented |
| EPYC 9575F | 64 / 128  | 3.3 GHz  | 5.0 GHz | 256 MB | 400 W | Zen 5 (F) | documented |
| EPYC 9475F | 48 / 96   | 3.65 GHz | 4.8 GHz | 256 MB | 400 W | Zen 5 (F) | documented |
| EPYC 9355  | 32 / 64   | 3.55 GHz | 4.4 GHz | 256 MB | 280 W | Zen 5  | documented |
| EPYC 9175F | 16 / 32   | 4.2 GHz  | 5.0 GHz | 512 MB | 320 W | Zen 5 (F) | documented |

The family tops out at 192 cores / 384 threads (9965) and 5.0 GHz max boost (F-series). The runnable model carries each SKU's rated (max boost) clock; a specific base/all-core clock is a configuration of the same model via setSpecClock.

## 3. Programmer-visible state

x86-64 register model (Zen) with chiplet CCDs, large shared L3 and ECC/RAS.

The SLeeLa model stores these in SLRegisterFile and exposes 16 addressable register slots to the runnable model.

## 3. Address and data path

- Data path: 64 bits.
- Address path: 48 bits.
- Directly addressable space: 256 TiB.
- The model's SLRAM arena is bounded for simulation; the architectural address width is recorded above.

## 4. Clock

Rated model clock: about 3700 MHz (overclockable in the model via setSpecClock).

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

- L1 instruction cache: present
- L1 data cache: present
- L2: present
- L3: present

cache coherence protocol: modeled where multi-core

## 7. DMA

The base CPU has no integrated DMA controller; DMA is a system-level bus-master concern and SLDMAController is left uninstantiated for the base profile.

## 8. Interrupts

The model records interrupt request, recognition, vector/entry and timing through SLInterruptController without inventing undocumented internal behavior.

## 9. MMU/TLB

The processor integrates an MMU/TLB; address translation is modeled as virtual -> physical through that unit.

## 10. Evidence labels

- documented: directly stated by a hardware/reference source.
- derived: mechanically calculated from documented properties.
- estimated: engineering approximation, never presented as silicon fact.
- unspecified: deliberately left unknown.

## Sources

- https://en.wikipedia.org/wiki/Epyc
- https://en.wikipedia.org/wiki/Zen_(microarchitecture)
- https://en.wikipedia.org/wiki/Zen_5
- https://en.wikipedia.org/wiki/Template:AMD_Epyc_9005_series
- https://www.amd.com/en/products/processors/server/epyc/9005-series.html
