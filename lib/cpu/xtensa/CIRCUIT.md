# Xtensa Functional Circuit Model

Functional block model, not a transistor-level netlist. Profile-selected blocks include fetch/decode, register file and optional window mapping, integer ALU, branch/loop unit, load/store unit, optional floating-point or DSP/SIMD units, memory protection/MMU, caches, debug/exception logic, and SoC bus interfaces. Custom instruction blocks require explicit metadata. SLCoupler models decode-to-execution, operand/writeback paths, memory traffic, exception signaling, and optional extension coupling.

## Executable circuit model

`SLXtensaCircuit.sleela` carries this circuit in source. It states the logic family and
voltage thresholds, the supply rail, the clock and its phases, the functional
blocks with width, depth, gate and transistor counts and derived area, two
timing paths with stage delays, four monitored nets with signal-quality
checks, and three semaphores for the shared latch and buses. `SLXtensaCPU.circuitModel()`
returns it built and clocked.

The class header marks every figure as recalled or derived. Recalled figures
come from general knowledge and still need checking against a primary source.
Derived figures are planning numbers, not measurements. The model is at
functional-block level, not a transistor netlist.

Circuit grade: **C** (6/10), with 0 verification violations, from
`circuitScore()` in `SLCircuitModel`. `lib/cpu/examples/circuit-grades.sleela`
grades every CPU.
