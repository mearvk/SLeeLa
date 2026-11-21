# Intel 4004 Logical Circuit Model

This SLeeLa view is a logical teaching/diagnostic model, not a transistor-level reconstruction.

Conceptual blocks:
- 12-bit program counter and return-stack logic
- instruction-byte fetch and decoder
- 4-bit accumulator and arithmetic/carry path
- sixteen 4-bit registers and pair operations
- program-store interface
- external chipset and I/O control

Shared SLeeLa gates, latches, flip-flops, adders, and bus abstractions may support visualization. Label inferred relationships as derived; do not present them as verified silicon schematics.
## Executable circuit model

`SLIntel4004Circuit.sleela` carries this circuit in source: PMOS silicon gate, 10 um, negative 15 V supply, two-phase 740 kHz. It states the
logic family and voltage thresholds, the supply rail, the functional blocks with
width, depth, gate and transistor counts and derived area, the timing paths with
stage delays, the monitored nets with signal-quality checks, and the semaphores
that arbitrate the shared latch and buses. Every figure is marked published,
cited, recalled or derived in the class header. `circuit-demo.sleela` builds
the model, clocks it, verifies it and injects faults to show each check fails
when it should.

The model stays at functional-block level. It is not a transistor netlist.

Circuit grade: **B** (7/10), from `circuitScore()` in `SLCircuitModel`. `SLIntel4004CPU.circuitModel()` returns this circuit. Checking the recalled figures named in the class header against a primary source raises the grade.
