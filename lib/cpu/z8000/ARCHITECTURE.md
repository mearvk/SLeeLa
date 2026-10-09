# Zilog Z8000 Architecture

SLZ8000CPU -> fetch -> decode -> operand/address generation -> integer execution -> memory/I/O -> condition-state update -> completion.

Z8001 uses segmented addressing; Z8002 has a non-segmented 16-bit address-space profile. The family has variable-length instructions and rich operand addressing. Implementation-specific timing and peripheral integration remain profile-dependent.
