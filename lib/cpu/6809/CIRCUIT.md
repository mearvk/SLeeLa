# Motorola 6809 Circuit Model

Functional architectural model, not transistor-level reconstruction. Blocks: instruction fetch/prefix handling, opcode decoder, A/B accumulators and D view, X/Y index registers, S/U stack pointers and PC, direct-page/effective-address logic, integer/logic ALU, condition-code/interrupt control, memory/bus interface, clock/sequencing. SLCoupler models fetch-to-decode, decode-to-registers, index/DP-to-address generation, ALU-to-condition codes, stack-to-interrupt entry, and bus-to-memory paths.
