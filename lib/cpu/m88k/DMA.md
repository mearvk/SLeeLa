# Motorola 88000 DMA

DMA is modeled as a system-level bus master.

SLDMAController
-> SLBusArbiter
-> SLIOBus / memory system

The generic 88000 CPU does not claim an integrated DMA controller. A workstation/server platform supplies its DMA engine, I/O bridge, descriptors, interrupts, and coherency behavior.
