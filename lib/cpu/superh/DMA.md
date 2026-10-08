# SuperH DMA

DMA is modeled as a system/peripheral resource rather than universally as a CPU execution unit.

SLDMAController
-> SLBusArbiter
-> SuperH bus
-> memory/I/O

Renesas documentation shows DMAC support in SuperH system implementations, while availability differs by device/profile. citeturn1search22turn1search0

SLeeLa therefore makes DMA configurable at the system implementation boundary.
