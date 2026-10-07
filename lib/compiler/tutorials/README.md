<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa Compiler Tutorials

These tutorials teach /lib/compiler from authoritative .sleela source through semantic analysis, SLeeLa IR, VM lowering, and SLVM/SLJVM artifact planning.

## Learning path

1. 01-source-to-vm.md — source through the compiler pipeline.
2. 02-compiler-manager-and-profiles.md — Basic/Complete and Advanced/Total checks.
3. 03-language-and-binary-format-reference.md — language, producer, toolchain, and binary mappings.
4. 04-building-a-language-front-end.md — build a compiler for any public language with the modular framework.
5. examples/hello-vm.sleela, examples/advanced-vm-target.sleela, and examples/multi-language-registry.sleela — source examples.

## Pipeline

SLeeLa source -> parsing -> declarations -> symbol resolution -> semantic analysis -> SLeeLa IR -> capability/dependency review -> VM lowering -> SLVM/SLJVM emission -> diagnostics.

## Build

make compiler

or make -C lib/compiler

Native C/C++ implements compiler services; .sleela remains the source-level contract.