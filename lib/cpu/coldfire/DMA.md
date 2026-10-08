# Motorola ColdFire DMA

DMA is modeled as a system/SoC resource.

SLDMAController
-> SLBusArbiter
-> ColdFire interconnect
-> memory/peripherals

A particular ColdFire SoC may integrate DMA controllers, but SLeeLa does not claim that DMA is part of the CPU execution core.
