# PDP-15 Logical Circuit Model

The SLeeLa circuit representation is a conceptual architecture model, not a verified transistor-level reconstruction.

Conceptual blocks:
- PDP-15 instruction register and decoder
- program counter and address path
- 18-bit accumulator and arithmetic/logical unit
- memory interface
- sequencing and control-flow logic
- I/O dispatch and configured peripheral interfaces
- option-dependent execution units when explicitly enabled

Use shared SLeeLa gates, latches, flip-flops, adders, and bus abstractions where useful. Mark inferred structure as derived; do not present it as a verified board schematic.