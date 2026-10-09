# SHARC DSP Architecture

Functional path: instruction fetch/decode -> register and address generation -> ALU / multiplier / MAC / floating-point units -> data-memory and peripheral interfaces -> architectural state update.

The Super Harvard organization is represented through separate program and data access paths where the device supports them. Execution-unit count, SIMD features, pipeline depth, memory organization, and core generation are profile-specific; newer SHARC+ features are not projected onto earlier SHARC devices.
