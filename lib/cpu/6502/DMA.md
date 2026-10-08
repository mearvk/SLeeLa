# 6502 DMA

## Base CPU

The original 6502 does not contain an integrated DMA controller.

SLDMAController is therefore absent from the base CPU object graph.

## System DMA

A complete 6502 computer may still contain DMA-capable peripherals or external logic. SLeeLa represents that separately:

SLDMAController -> SLBusArbiter -> SLIOBus

The CPU is a bus participant; DMA is a system-level bus-master concern.

## Modeling rule

Do not invent:

- DMA channel counts;
- descriptor registers;
- DMA burst sizes;
- arbitration policies

for the 6502 itself.

Those belong to a specific machine, chipset or derivative specification.
