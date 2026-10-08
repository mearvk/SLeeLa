# Intel Itanium DMA

DMA is a platform-level facility.

SLDMAController
-> SLBusArbiter
-> SLIOBus / memory system

A concrete Itanium platform supplies DMA engines, I/O bridges, descriptor formats, interrupts, and coherency behavior.

The IA-64 instruction set itself does not become a DMA controller.
