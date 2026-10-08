# Motorola 68000 DMA

## Base CPU

The original MC68000 is modeled without an integrated DMA controller.

## System DMA

An external DMA controller can become a bus master:

SLDMAController -> SLBusArbiter -> SLIOBus

The arbiter tracks CPU ownership versus external master ownership.

## Variant rule

Later integrated 68K processors can contain DMA. For example, NXP documents the MC68340 as an integrated processor with DMA. Such features belong in a dedicated processor profile rather than being retroactively assigned to the original 68000.
