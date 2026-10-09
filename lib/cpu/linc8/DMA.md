# LINC-8 DMA and Transfer Policy

The baseline profile does not assume a generic DMA controller.

- CPU-mediated transfers use the configured memory/device interfaces.
- A hardware-specific transfer feature must be explicitly enabled by the machine profile.
- Any extension must define address ranges, ownership, completion, error handling, and ordering across mode changes.
- Unsupported DMA requests must return a structured result and must not silently alter memory.

Do not infer DMA support merely from the presence of LINC-specific peripherals.