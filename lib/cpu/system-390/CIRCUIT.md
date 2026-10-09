# ESA/390 Logical Circuit Abstraction

## Scope

A functional logic abstraction for simulation and teaching, not a transistor-level System/390 netlist.

## Blocks

- Instruction fetch and decode.
- General-purpose register file and execution units.
- ESA/390 PSW, condition-code, and interruption control.
- Address generation and optional translation/protection path.
- Profile-enabled floating-point and control-state units.
- Storage interface and channel-I/O request interface.

## Profile control

Instantiate only blocks justified by the selected ESA/390 model and enabled facilities. Do not infer physical gate counts, clock rate, pipeline depth, power, or undocumented cache layout.
