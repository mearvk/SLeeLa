# PDP-6 Logical Circuit Model

This is a logical architecture view for SLeeLa diagnostics and education, not a verified transistor-level reconstruction.

Conceptual blocks:
- instruction register and PDP-6 decoder
- 36-bit register and arithmetic paths
- program control and effective-address logic
- memory interface
- I/O instruction and peripheral dispatch
- sequencing and exception/diagnostic path

Use shared SLeeLa gate, latch, flip-flop, adder, and bus abstractions where useful. Mark inferred relationships as derived; do not present them as verified board schematics.