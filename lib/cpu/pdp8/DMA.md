# PDP-8 I/O Data Transfer

## Scope

Data movement depends on the configured PDP-8 system and peripheral devices. Do not assume a universal modern DMA controller.

## Device model

Each IOT-capable device should declare its supported instruction codes, status and data behavior, interrupt behavior, and any documented transfer mechanism.

## Transfer safety

Validate device commands and memory addresses through the machine memory interface. Device handlers must not access arbitrary host memory.

## Unknowns

Do not invent peripheral inventories, transfer rates, or model-specific controller behavior. Keep unsupported device operations explicit.
