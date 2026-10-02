# Tutorial 01 — SLeeLa Source to VM

## Goal

Trace a source program through compilation without skipping semantic and VM checks.

1. Load a SLeeLa source unit.
2. Parse classes, functions, fields, and declarations.
3. Resolve names and symbols; unresolved references become diagnostics.
4. Perform type, semantic, capability, and dependency analysis.
5. Build SLeeLa IR.
6. Lower the IR to an SLVM/SLJVM target artifact plan.
7. Produce diagnostics distinguishing source, semantic, capability, dependency, VM-fit, and artifact results.

## Key source files

- LanguageReference.sleela
- ProgramMapping.sleela
- BinaryFormatReference.sleela
- SafetyReference.sleela
- SleelaVMCompilerManager.sleela

A compiler target is an artifact plan, not a second source language.
