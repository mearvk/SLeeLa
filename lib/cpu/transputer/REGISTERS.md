# Inmos Transputer Registers

## Execution state

SLeeLa models the Transputer's processor state around:

- instruction pointer;
- workspace pointer;
- operand evaluation registers/state;
- processor status/control;
- process scheduling state.

The exact internal register organization is profile-specific and is not replaced by a fabricated conventional 32-register file.
