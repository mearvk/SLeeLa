# SiFive U74 (RISC-V RV64GC) Specification

## 1. Identity

**Model:** SiFive U74 (RISC-V RV64GC)  
**Architecture class:** 64-bit application core (RISC-V RV64GC)  
**Data path:** 64-bit  
**Addressing:** 39-bit external address space (512 GiB)  
**Register model:** RISC general-purpose register file

The profile exposes a programmer model while using the shared SLeeLa CPU objects (clock, register file, ALU, decoder, bus) to sequence each instruction.

## 2. Programmer-visible state

RISC-V RV64GC register model (x0 hardwired zero, x1-x31, 32 FP), Sv39 MMU.

The SLeeLa model stores these in SLRegisterFile and exposes 32 addressable register slots to the runnable model.

## 3. Address and data path

- Data path: 64 bits.
- Address path: 39 bits.
- Directly addressable space: 512 GiB.
- The model's SLRAM arena is bounded for simulation; the architectural address width is recorded above.

## 4. Clock

Rated model clock: about 1500 MHz (overclockable in the model via setSpecClock).

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
- L3: none

cache coherence protocol: none

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

- https://www.scs.stanford.edu/~zyedidia/docs/sifive/sifive-u74.pdf
- https://en.wikipedia.org/wiki/RISC-V
