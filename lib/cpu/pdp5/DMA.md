# PDP-5 DMA and Transfers

Baseline PDP-5 profile: no generic DMA controller is presumed.

- Ordinary memory and I/O-transfer operations use the CPU/device interface.
- Device-specific block transfers may be added as explicit peripherals when supported by the selected hardware configuration.
- A transfer extension must define ownership, address range, completion, errors, and CPU-visible ordering.
- If no extension is installed, a DMA request is reported as unsupported; it must not silently modify memory.

This is a conservative emulator profile, not a claim that no PDP-5 installation could use specialized transfer hardware.