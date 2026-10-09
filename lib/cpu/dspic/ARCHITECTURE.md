# dsPIC Architecture

Functional path: instruction fetch/decode -> register and effective-address generation -> MCU ALU or DSP arithmetic/MAC -> architectural writeback -> memory/peripheral transaction.

The implementation keeps control-oriented MCU operations, DSP operations, interrupt behavior, and peripheral access integrated but distinct. dsPIC30 and dsPIC33 variants differ; instruction encoding, address spaces, data widths, DSP register details, and pipeline behavior are profile-specific. Do not assume PIC16/PIC18 semantics apply unchanged.
