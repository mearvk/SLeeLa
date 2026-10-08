# ARM DMA

DMA is a system-level or SoC-level bus-master facility.

SLDMAController
-> SLBusArbiter
-> SLIOBus / memory fabric

A concrete ARM SoC can integrate DMA controllers, while a standalone CPU core may rely on an external controller.

DMA coherency, cache maintenance, memory attributes, security state, and ordering are implementation/platform properties and must be supplied by the selected ARM profile.
