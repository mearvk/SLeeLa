# PDP-6 Memory and I/O Interface

Separate memory operations from peripheral I/O dispatch.

- Memory data uses the 36-bit word model.
- Address range and memory capacity come from the machine profile.
- I/O instructions dispatch through a PDP-6-specific device/function map.
- Device modules define their own request, completion, and error behavior.
- Unknown devices or unsupported functions emit diagnostics and return a structured unsupported-operation result.

Do not import modern PCI semantics or later PDP-10 peripheral assumptions as native PDP-6 hardware. Preserve deterministic ordering for memory and I/O side effects.