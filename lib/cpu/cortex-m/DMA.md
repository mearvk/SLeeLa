# Cortex-M DMA Integration

## Ownership

DMA is generally a microcontroller/SoC peripheral, not a mandatory CPU-core feature. This module describes how SLeeLa connects an external DMA controller to a Cortex-M platform; it does not imply that every Cortex-M chip contains the same controller.

## Platform configuration

Define controller type, channels/streams, transfer widths, address constraints, request routing, descriptor format, arbitration, interrupt signaling, and memory reachability from the SoC documentation. Leave unsupported fields unspecified.

## CPU interaction

DMA transfers share platform memory/bus resources and may race with CPU accesses. Model completion/error interrupts through the NVIC interface and honor platform-defined ordering and peripheral side effects.

## Cache considerations

On cache-equipped targets, CPU/DMA visibility may require clean/invalidate operations and barriers. The exact policy depends on the cache, interconnect, and memory attributes. Do not assume universal coherency.

## Safety

Validate transfer bounds, alignment, permitted memory regions, and descriptor chains. Invalid transfers should return a modeled bus/transfer error rather than access arbitrary host memory.
