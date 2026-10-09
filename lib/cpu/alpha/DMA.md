# DEC Alpha DMA

DMA is modeled as a platform-level bus master.

SLDMAController
-> SLBusArbiter
-> SLIOBus / memory system

Alpha systems commonly relied on chipset/peripheral DMA rather than an ISA-defined integrated DMA controller.

A concrete platform profile supplies DMA channels, descriptors, interrupts, coherency, and I/O bridge behavior.
