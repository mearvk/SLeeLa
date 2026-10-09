# PDP-15 Memory and I/O

- Memory words are 18 bits; the selected profile defines address range and memory organization.
- Instruction-driven I/O dispatches through a PDP-15-specific device/function map.
- Peripheral implementations are pluggable and must declare supported operations and side effects.
- Unknown devices, disabled options, and unsupported functions produce structured errors and diagnostics.
- Keep peripheral extensions distinct from native CPU behavior.

Do not represent modern PCI-style buses as native PDP-15 hardware. Preserve ordering between memory operations, interrupt/device state, and I/O side effects.