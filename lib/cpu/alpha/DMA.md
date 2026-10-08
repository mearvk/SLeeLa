# DEC Alpha DMA

DMA is modeled at the platform level.

SLDMAController -> SLBusArbiter -> SLIOBus / memory system.

A concrete Alpha platform supplies DMA channels, descriptors, interrupts, coherency, and I/O bridge behavior.