# PDP-12 Logical Circuit Model

This is a logical teaching and diagnostics view, not a transistor-level schematic.

Conceptual blocks:
- shared memory and address interface
- mode selector / instruction-environment control
- PDP-8-family instruction decoder
- LINC-compatible instruction decoder
- environment-specific execution and register state
- I/O dispatch and sequencing logic

Use shared SLeeLa gates, latches, flip-flops, adders, and buses where they help represent behavior. Mark inferred connections as derived; do not imply a verified board-level reconstruction.