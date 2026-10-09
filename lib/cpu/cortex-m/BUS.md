# Cortex-M Bus and Memory Interface

## Core-side model

Represent instruction and data accesses through a configurable memory/bus interface. The number and organization of bus interfaces, access ordering, buffering, and arbitration vary by implementation and SoC.

## Address spaces

The MCU integration supplies the memory map for code flash/ROM, SRAM, peripheral registers, system control space, external memory, and any vendor-specific windows. The CPU model must not hard-code a universal board memory map.

## Access semantics

Honor access size, alignment, privilege, security attribution, execute permissions, and memory type. Device/peripheral accesses may have side effects and must not be optimized as ordinary RAM. Unsupported or unmapped accesses produce the configured bus fault/error behavior.

## Latency

Wait states, flash accelerators, bridges, bus contention, and peripheral response are SoC-specific. Provide configurable wait states and annotate estimates. Do not assert universal cycles per access.

## DMA and peripherals

DMA controllers and peripheral buses are SoC components, not required parts of the Cortex-M CPU core. Attach them through the platform bus and describe cache coherency requirements where relevant.
