# DEC PDP-10 Architecture

SLPDP10CPU -> fetch -> decode -> accumulator/operand access -> effective-address and byte-pointer processing -> execution -> memory/I/O -> completion.

Preserve 36-bit words, classic instruction fields, accumulator semantics, and byte pointers; do not reinterpret the ISA as a modern byte-addressed 64-bit architecture. PDP-6 lineage and KA/KI/KL/KL10/KS10 system profiles remain distinct.
