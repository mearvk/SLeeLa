# Z80 DMA

## Base CPU

The original Z80 CPU does not instantiate the Z80 DMA peripheral.

The separate Z80 DMA device has programmable control/status registers and can control bus transfers. Zilog documents configurable starting addresses, transfer length, address-counting rules, port configuration, matching, interrupt conditions, and READY/WAIT behavior.

## SLeeLa topology

SLDMAController -> SLBusArbiter -> SLIOBus

The CPU participates in the bus but the DMA controller remains a separate system component.

## Rule

DMA channel counts, descriptor formats, transfer modes, and arbitration are specified only for a concrete Z80 DMA/device implementation, not invented for the base CPU.
