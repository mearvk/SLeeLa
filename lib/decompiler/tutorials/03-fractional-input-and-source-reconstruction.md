# Tutorial 03 — Fractional Input and Source Reconstruction

Four explicit policies are supported: PRESERVE, PARTIAL, REPORT_ONLY, and STRICT.

When symbols or debug information are absent, do not invent original names or source-level types. Preserve known instructions and control flow, mark inferred semantics, and retain unresolved metadata.

The executable/library/source reference maps artifact -> format -> architecture/OS/ABI -> producer/toolchain -> language -> source forms -> reconstruction target.

Decompiler confidence is not VM authorization; reconstructed VM output must still pass compiler and VM completeness checks.
