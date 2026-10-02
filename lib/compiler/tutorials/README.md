<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa Compiler Tutorials

These tutorials teach /lib/compiler from authoritative .sleela source through semantic analysis, SLeeLa IR, VM lowering, and SLVM/SLJVM artifact planning.

## Learning path

1. 01-source-to-vm.md — follow source through the compiler pipeline.
2. 02-compiler-manager-and-profiles.md — understand Basic/Complete and Advanced/Total checks.
3. 03-language-and-binary-format-reference.md — use language, producer, toolchain, and binary mappings safely.
4. examples/hello-vm.sleela — minimal source example.
5. examples/advanced-vm-target.sleela — advanced target planning example.

## Pipeline

SLeeLa source -> parsing -> declaration discovery -> symbol resolution -> semantic analysis -> SLeeLa IR -> capability/dependency review -> VM lowering -> SLVM/SLJVM emission -> diagnostics.

## Build

make compiler

or

make -C lib/compiler

Native C/C++ implements compiler services; it does not replace the SLeeLa source-level contract.