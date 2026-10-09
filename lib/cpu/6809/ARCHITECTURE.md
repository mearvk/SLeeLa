# Motorola 6809 Architecture

SL6809CPU -> instruction fetch -> prefix/opcode decode -> effective-address generation -> ALU -> memory/I/O -> condition-code update.

Addressing includes accumulator, direct, extended, immediate, and indexed modes. Prefix bytes select additional instruction groups; indexed modes include offsets and auto-increment/decrement forms. The 6809 is not treated as a 6502 variant. MC6809E external bus timing/control remains profile-specific.
