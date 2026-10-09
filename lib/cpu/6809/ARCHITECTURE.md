# Motorola 6809 Architecture

SL6809CPU -> instruction fetch -> prefix/opcode decode -> effective-address generation -> ALU -> memory/I/O -> condition-code update.

The architecture supports accumulator, direct, extended, immediate, and indexed addressing. Prefix bytes select additional instruction groups; indexed modes include offsets and auto-increment/decrement forms. SLeeLa models these explicitly rather than treating the 6809 as a 6502 variant.

MC6809E external clock/bus-control signals remain profile-specific.
