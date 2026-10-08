# RISC-V DMA

DMA is modeled as a platform-level bus master rather than as an inherent RISC-V ISA feature.

SLDMAController
-> SLBusArbiter
-> SLIOBus / memory fabric

A selected SoC profile may add one or more DMA engines with descriptor rings, interrupt generation, cache-coherency requirements, IOMMU support, or device-specific channels.

The generic RISC-V CPU model does not claim those facilities unless selected by the platform implementation.
