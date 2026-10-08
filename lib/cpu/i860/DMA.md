# Intel i860 DMA

DMA is modeled as an external/system resource.

SLDMAController
-> SLBusArbiter
-> i860 system interconnect
-> memory/peripherals

No undocumented CPU-internal DMA engine is assumed.
