# PDP-7 Memory and I/O

Memory and peripheral I/O are modeled through separate interfaces.

- Memory transfers use 18-bit words and the configured address range.
- I/O operations use a PDP-7-specific decoder and the selected device map.
- Peripheral modules define their supported function codes and side effects.
- Unknown device/function requests return a structured unsupported result and are logged.
- Maintain ordering between CPU memory effects and device-visible operations.

Modern PCI-style bus semantics are not represented as native PDP-7 hardware. Any special transfer capability must be an explicit machine-specific extension.