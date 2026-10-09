# DSP56000 Architecture

Functional path: program fetch/decode -> X/Y operand and address generation -> data ALU and multiplier-accumulator -> accumulator/register writeback.

The model captures DSP-oriented parallel data movement, fixed-point arithmetic, address modifiers, hardware looping, and interrupt behavior where supported by the chosen profile. Program and data memory organization is profile-defined. Do not project features from later DSP563xx or other DSP56k descendants onto the original DSP56000 without evidence.
