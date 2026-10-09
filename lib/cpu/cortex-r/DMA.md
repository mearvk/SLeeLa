# Cortex-R DMA and Platform Integration

## Scope

DMA is a platform integration concern unless a selected implementation explicitly specifies an integrated engine. This document defines the interface between the CPU model and an SoC DMA controller.

## Configuration

Specify controller implementation, channels, request routing, transfer widths, address constraints, descriptor format, arbitration, memory reachability, completion/error signaling, and interrupt integration from target documentation.

## CPU interaction

DMA shares memory and interconnect resources with the CPU and other masters. Model contention when timing analysis requires it. Validate address ranges, alignment, descriptor bounds, and access permissions; invalid transfers must not read or write arbitrary host memory.

## Cache and TCM

Document whether DMA can access each TCM region and whether cache maintenance or coherency support is required. Do not assume all DMA masters can reach all CPU-local memories.

## Reporting

Separate CPU-core capabilities from SoC DMA capabilities. Unknown routing, latency, and coherency behavior remains `unspecified`.
