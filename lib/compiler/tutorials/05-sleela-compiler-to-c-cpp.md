# Lesson 05 — Use SLeeLa to build a compiler, then emit C/C++

Max Rupplin - MEARVK LLC - 2026

## Two developer surfaces

- **SLeeLa interface:** create a `SLCompilerWorkbench`, describe source/output/target, validate a request, and retrieve a structured build result. This is the natural interface for the SLeeLa terminal, IDE, or build scripts.
- **Native backend:** verified SLeeLa compiler IR may be lowered to generated C or C++ source. The native toolchain compiles that generated source into a host executable or library.
- **C ABI / C++ orchestration:** the native bridge implements filesystem and toolchain operations. SLeeLa source remains the contract and user-facing API.

## Intended pipeline

`SLeeLa compiler source -> source loading -> lexer/parser -> symbol and semantic checks -> SLeeLa IR -> verified C or C++ emitter -> compiler toolchain -> native artifact`

The alternative primary route remains:

`SLeeLa source -> compiler -> SLVM/SLJVM artifact -> explicit VM/OS security boundary`

## Example interface

```sleela
SLCompilerWorkbench workbench = new SLCompilerWorkbench();
workbench.configure();
if (workbench.configureBuild("src/MyCompiler.sleela", "build/MyCompiler.cpp", "CPP", "/usr/bin/c++")) {
    SLCompilerBuildResult result = workbench.build();
    if (!result.ok()) {
        // Report result.diagnostics() to the developer.
    }
}
```

This example documents the intended API shape. Native emission succeeds only when the host has implemented and enabled the corresponding bridge operations. A source-level contract alone is not proof that a backend exists on a particular build.

## Safe and reproducible native generation

1. Parse and validate against the active `SLEELA.syntax` version.
2. Resolve imports and symbols before lowering.
3. Reject unknown operations, unsupported language features, and unresolved dependencies.
4. Emit only from verified IR; never splice unchecked source text into generated code.
5. Escape all string and identifier data for the target language.
6. Record source identity, compiler version, target triple, and options in a build manifest.
7. Compile in a controlled build directory; do not execute generated output as part of compilation.
8. Run tests and security/capability checks before installation.

## Current implementation boundary

The workbench, request/result objects, target selector, and emitter contract are SLeeLa-side interfaces. The native `sleela_compiler_emit_verified_ir` bridge is an integration point and must be implemented and tested before describing C/C++ emission as production-ready. The experimental top-level `/compiler` frontend is not a complete implementation of the full language.
