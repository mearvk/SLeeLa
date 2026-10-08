# IBM 801 DMA

DMA is modeled at the system level.

SLDMAController
-> SLBusArbiter
-> 801 system interconnect
-> memory/peripherals

No undocumented CPU-internal DMA engine is assumed.
