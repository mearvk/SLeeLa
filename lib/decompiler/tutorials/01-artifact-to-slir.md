# Tutorial 01 — Artifact to SLeeLa IR

## Goal

Understand the normal evidence-preserving decompiler path.

1. Acquire the artifact without executing it.
2. Record identity and acquisition metadata.
3. Identify format evidence from headers, sections, architecture markers, and object metadata.
4. Decode conservatively according to validated evidence.
5. Build control-flow relationships where supported.
6. Lift operations into SLIR while preserving uncertainty.
7. Use symbols, debug data, calling conventions, imports/exports, and runtime metadata when available.
8. Reconstruct SLeeLa-oriented source or another requested target.
9. Validate VM-target output through compiler and VM completeness boundaries.

A decompiler should not claim exact original source when the evidence does not support that conclusion.
