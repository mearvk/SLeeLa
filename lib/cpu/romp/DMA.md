# IBM ROMP DMA

DMA is modeled at the system level.

SLDMAController
-> SLBusArbiter
-> ROMP system interconnect
-> memory/peripherals

This avoids assigning undocumented CPU-internal DMA behavior to ROMP.
