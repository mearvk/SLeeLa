# Xtensa Functional Circuit Model

Functional block model, not a transistor-level netlist. Profile-selected blocks include fetch/decode, register file and optional window mapping, integer ALU, branch/loop unit, load/store unit, optional floating-point or DSP/SIMD units, memory protection/MMU, caches, debug/exception logic, and SoC bus interfaces. Custom instruction blocks require explicit metadata. SLCoupler models decode-to-execution, operand/writeback paths, memory traffic, exception signaling, and optional extension coupling.
