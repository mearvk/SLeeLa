# DEC PDP-10 Registers

## Accumulators

The architectural accumulator set is AC0 through AC17, each 36 bits wide.

## Instruction and status state

SLeeLa models:

- program counter and processor flags/state;
- current instruction;
- effective-address state;
- accumulator registers;
- byte-pointer state;
- profile-specific memory-management and interrupt state.

Do not conflate PDP-10 accumulators with PDP-11 R0-R7.
