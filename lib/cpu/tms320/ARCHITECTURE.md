# Texas Instruments TMS320 Architecture

SLTMS320CPU -> program fetch/decode -> operand/address generation -> multiply/accumulate or arithmetic -> data memory -> accumulator/state update.

SLeeLa exposes MAC execution, accumulator handling, circular addressing, repeat/loop controls, and separate program/data memory interfaces when supported by the chosen profile. C1x/C2x fixed-point and C3x/C4x floating-point generations remain distinct; features are not projected across the family without evidence.
