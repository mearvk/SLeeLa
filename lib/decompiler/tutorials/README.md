<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa Decompiler Tutorials

These tutorials teach the evidence-preserving path from executable/library/object input toward SLeeLa-oriented intermediate representations and source reconstruction.

## Learning path

1. 01-artifact-to-slir.md — acquire an artifact and build an evidence-preserving analysis path.
2. 02-os-abi-and-format-discernment.md — combine format, architecture, OS, and ABI evidence.
3. 03-fractional-input-and-source-reconstruction.md — preserve partial information instead of inventing source.
4. examples/artifact-analysis.sleela — analysis request example.
5. examples/fractional-reconstruction.sleela — partial reconstruction example.

## Pipeline

artifact -> acquisition -> format/OS evidence -> decoding -> CFG -> SLIR -> semantic analysis -> reconstruction -> SLeeLa/target output -> VM validation.

## Safety

Input artifacts are untrusted. Analysis must not execute constructors, entry points, loaders, embedded scripts, or other executable content merely to identify or reconstruct it.

## Build

make decompiler

or

make -C lib/decompiler

Native C/C++ provides implementation services; .sleela files remain the source-level contract.