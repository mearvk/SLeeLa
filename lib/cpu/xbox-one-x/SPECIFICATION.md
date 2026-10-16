# Xbox One X (x86-64 Jaguar) Specification

## 1. Identity

**Model:** Xbox One X (x86-64 Jaguar)  
**Architecture class:** 64-bit console APU (x86-64 Jaguar)  
**Data path:** 64-bit  
**Addressing:** 48-bit external address space (256 TiB)  
**Register model:** CISC named/split register set

The profile exposes a programmer model while using the shared SLeeLa CPU objects (clock, register file, ALU, decoder, bus) to sequence each instruction.

## 2. Programmer-visible state

x86-64 register model; 8-core AMD Jaguar-evolved cluster at a higher clock.

The SLeeLa model stores these in SLRegisterFile and exposes 16 addressable register slots to the runnable model.

## 3. Address and data path

- Data path: 64 bits.
- Address path: 48 bits.
- Directly addressable space: 256 TiB.
- The model's SLRAM arena is bounded for simulation; the architectural address width is recorded above.

## 4. Clock

Rated model clock: about 2300 MHz (overclockable in the model via setSpecClock).

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

The processor/SoC integrates a DMA engine; SLeeLa may instantiate SLDMAController for it.

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

- https://en.wikipedia.org/wiki/Xbox_One_X
- https://en.wikipedia.org/wiki/Jaguar_(microarchitecture)
