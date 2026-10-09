# DEC PDP-10 Circuit Model

Functional architectural simulation only, not transistor-level reconstruction.

Major blocks: instruction fetch/register, decoder, 36-bit accumulator file, effective-address unit, byte-pointer unit, arithmetic/logic execution, memory interface, I/O interface, interrupt/control logic, clock/sequencing.

SLCoupler models fetch-to-decode, decode-to-accumulator, accumulator-to-execution, byte-pointer-to-address, execution-to-memory, I/O-to-control, and interrupt-to-processor paths. Implementation-specific details are not inferred beyond documented profiles.
