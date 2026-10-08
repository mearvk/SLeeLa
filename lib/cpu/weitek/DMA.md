# Weitek POWER DMA

DMA is modeled at the system/interconnect level.

SLDMAController
-> SLBusArbiter
-> Weitek POWER system interconnect
-> memory/peripherals

No undocumented CPU-internal DMA engine is assumed.
