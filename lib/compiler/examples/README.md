# Building a SLeeLa Compiler from SLeeLa Source

Max Rupplin - MEARVK LLC - 2026

This directory teaches compiler construction at four levels. Follow the levels in order; each introduces additional compiler responsibilities.

| Level | Example | Objective |
|---|---|---|
| [Novice](Novice/README.md) | HelloCompiler | Recognize a tiny grammar and produce a normalized record |
| [Mid](Mid/README.md) | ExpressionCompiler | Separate lexing, parsing, semantic checks, and IR |
| [Senior](Senior/README.md) | CompilerPipeline | Orchestrate validated phases and fail-closed artifact emission |
| [Very Senior](Very-Senior/README.md) | BootstrapCompiler | Plan reproducible bootstrap stages and compare outputs |

## Architecture

The intended authority path is: SLeeLa source → frontend → semantic analysis → SLeeLa IR → VM lowering → SLVM/SLJVM artifact. The SLeeLa-facing workbench and developer CLI are interfaces; native filesystem and process operations belong behind the explicitly enabled bridge. The authoritative native frontend remains under `/impl/frontend`. See the [compiler package](../README.md) and [multi-language framework](../MULTI-LANGUAGE.FRAMEWORK.md).

## Workflow

1. Read each level's README and inspect its `.sleela` scaffold.
2. Confirm the source syntax and language version against the current checkout; examples are educational scaffolds, not a claim that every method is already accepted by the compiler.
3. Test each stage independently and assert that errors block later stages.
4. Run the validation and compiler tests documented by the repository's current build files. This guide intentionally does not invent a command-line invocation.
5. Do not label the compiler executable or self-hosting until the real toolchain has compiled and tested it end-to-end.

## Safety

Treat these as educational examples, not production compilers. Restrict file access to an explicitly selected workspace, validate output paths, keep native compilation opt-in, validate IR and artifacts before VM admission, and never execute an input program as part of compilation.
