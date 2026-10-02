# SLeeLa Decompiler Tutorials

These tutorials teach the evidence-preserving path from executable/library/object input toward SLeeLa-oriented intermediate representations and source reconstruction.

## Learning path

1. 01-artifact-to-slir.md — artifact acquisition through SLIR.
2. 02-os-abi-and-format-discernment.md — combine format, architecture, OS, and ABI evidence.
3. 03-fractional-input-and-source-reconstruction.md — preserve incomplete information.
4. examples/artifact-analysis.sleela and examples/fractional-reconstruction.sleela — analysis examples.

## Pipeline

artifact -> acquisition -> format/OS evidence -> decoding -> CFG -> SLIR -> semantic analysis -> reconstruction -> SLeeLa/target output -> VM validation.

## Safety

Input artifacts are untrusted. Analysis must not execute constructors, entry points, loaders, embedded scripts, or other executable content merely to identify or reconstruct it.

## Build

make decompiler

or make -C lib/decompiler

Native C/C++ provides implementation services; .sleela remains the source-level contract.
