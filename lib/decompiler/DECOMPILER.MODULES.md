<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-decompiler-logo-001.jpeg" alt="SLeeLa Decompiler" width="100%"></p>

\n\n# SLeeLa Decompiler Modules

Modules extend decompilation beyond C, C++, and SLeeLa without modifying the core pipeline.

Each module declares language family/version, executable/object/container formats, architecture/ABI, OS/runtime associations, decoder services, symbol and metadata readers, type/signature recovery, reconstruction rules, evidence weights, dependencies, and trust/provenance metadata.

Selection is evidence-driven: explicit user selection, expected input, observed artifact evidence, compatibility matching, dependency resolution, then load. The selected module set is recorded in the decompilation report.

A missing or untrusted module must not cause invented source semantics.

Future module families can cover JVM bytecode, .NET assemblies, Rust, Go, Fortran, Swift, Objective-C, and additional VM formats. These are extension points, not claims of complete implementation.
