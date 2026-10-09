# Texas Instruments TMS320 Architecture

## DSP execution model

SLTMS320CPU -> program fetch/decode -> operand/address generation -> multiply/accumulate or arithmetic -> data memory -> accumulator/state update.

## DSP-specific behavior

SLeeLa exposes MAC execution, accumulator handling, circular addressing, repeat/loop controls, and separate program/data memory interfaces when supported by the chosen profile. Harvard-style organization is represented only for profiles that document it.

C1x/C2x fixed-point and C3x/C4x floating-point generations remain distinct. Features are not projected across the family without evidence.
