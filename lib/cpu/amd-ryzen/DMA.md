# AMD Ryzen (Zen x86-64) DMA

## Base CPU

The base AMD Ryzen (Zen x86-64) does not contain an integrated DMA controller.

SLDMAController is therefore absent from the base CPU object graph.

## System DMA

A complete system may still contain DMA-capable peripherals or external logic, represented separately:

SLDMAController -> SLBusArbiter -> SLIOBus

The CPU is a bus participant; DMA is a system-level bus-master concern.

## Modeling rule

Do not invent DMA channel counts, descriptor registers, burst sizes, or arbitration policies for the CPU itself. Those belong to a specific machine, chipset or derivative.

## Sources

- https://en.wikipedia.org/wiki/Zen_(microarchitecture)
- https://en.wikipedia.org/wiki/List_of_AMD_Ryzen_processors
