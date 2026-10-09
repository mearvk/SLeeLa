# PDP-4 Memory and I/O Interface

Represent memory access and peripheral I/O as separate interfaces.

- Memory transfers use 18-bit data words and a configured address range.
- I/O instructions dispatch using a PDP-4-specific device/function decoder.
- Device implementations are pluggable and configured per machine.
- Unknown device codes and unsupported functions produce diagnostics and structured errors.
- Preserve ordering between CPU-visible memory operations and I/O side effects.

Do not model modern PCI/PCIe semantics as native PDP-4 hardware. Any nonstandard peripheral behavior must be declared as a machine-specific extension.