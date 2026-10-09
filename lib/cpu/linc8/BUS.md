# LINC-8 Memory and I/O

Separate the memory interface from the device interface while allowing the machine profile to describe shared hardware paths.

- Transfers use 12-bit words.
- Address mapping and memory capacity are profile-defined.
- LINC and PDP-8 modes may expose different instruction-level I/O behavior.
- LINC-specific peripherals must be explicitly registered with their device handlers.
- Unknown devices or unsupported operations return structured diagnostics.
- Do not silently route an unsupported LINC I/O operation through the PDP-8 decoder, or vice versa.