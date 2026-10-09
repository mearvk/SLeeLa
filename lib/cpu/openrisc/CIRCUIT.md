# OpenRISC 1000 Functional Circuit Model

Functional block model, not a transistor-level netlist. Core blocks include instruction fetch/decode, GPR file, integer ALU, branch/control unit, load/store unit, exception/interrupt control, and SPR access. Optional blocks include FPU, DSP, atomic support, debug, MMU/TLB, and caches. SLCoupler models decode-to-execution, register writeback, memory traffic, exception transitions, and optional extension connections.
