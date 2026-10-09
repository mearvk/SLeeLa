# PDP-5 Circuit Model

The SLeeLa circuit view is a logical teaching/diagnostic model, not a transistor-level reconstruction.

Conceptual blocks:
- instruction register and decoder
- program counter and address path
- accumulator and link arithmetic path
- core-memory interface
- I/O instruction and device dispatch
- sequencing/control logic

Use shared SLeeLa gate, latch, flip-flop, adder, and bus abstractions only when useful to the selected fidelity level. Label inferred wiring as derived; do not imply verified board-level schematics.