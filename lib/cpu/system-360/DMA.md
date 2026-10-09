# System/360 Channel I/O and Data Movement

## Scope

System/360 data movement is modeled through the configured channel subsystem and devices, rather than assuming a modern generic DMA controller exists in every machine profile.

## Configuration

Describe available channel types, device attachment, command formats, storage access, status reporting, interruption delivery, and concurrency according to the selected machine and system documentation.

## Storage safety

Channel operations must validate command structures and storage references through the machine's access rules. Malformed commands or invalid addresses return modeled errors and must not access arbitrary host memory.

## CPU interaction

CPU I/O instructions initiate or inspect channel operations through explicit system interfaces. Channel scheduling and device transfer latency remain separate from CPU instruction timing.

## Unknown behavior

Do not invent channel count, transfer rates, device capabilities, or model-specific options. Keep unsupported details `unspecified`.
