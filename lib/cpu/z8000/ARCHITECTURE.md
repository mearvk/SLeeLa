# Zilog Z8000 Architecture

## Execution path

SLZ8000CPU -> fetch -> decode -> operand/address generation -> integer execution -> memory/I/O -> condition-state update -> completion.

## Profiles

- Z8001: segmented addressing through its segment/address mechanism.
- Z8002: non-segmented 16-bit address-space profile.

The family uses a 16-bit architecture with variable-length instructions and rich operand addressing. Implementation-specific timing and peripheral integration remain profile-dependent.
