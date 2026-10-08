# SLeeLa CPU Architecture Reference

This document defines the common architectural decomposition used when modeling real processors in `/lib/cpu`.

A processor profile is treated as a composition of modest, testable parts rather than one oversized CPU object:

`Clock → Fetch → Decode → Register File → Execution Units → Memory/Cache → Interconnect → I/O/DMA`.

The model distinguishes documented hardware facts from implementation abstractions. Where a vendor does not publish a physical detail, the SLeeLa model records it as unspecified rather than inventing transistor-level behavior.

## Core areas

- instruction set and execution model
- clock and timing domains
- registers and register files
- pipeline/front end
- ALU/FPU/vector execution
- L1/L2/L3 cache hierarchy
- MMU/TLB
- memory controller
- system and I/O buses
- I/O timing and arbitration
- DMA
- interrupts
- cache coherence
- power/performance states
- package/SoC interconnect

Modern processors commonly separate core execution from memory and I/O interconnects. ARM's AMBA documentation explicitly models masters, slaves, caches, MMUs and DMA-capable components, while AMD documents Zen as a scalable core architecture with a cache hierarchy and separate I/O development paths. citeturn0search36turn0search0