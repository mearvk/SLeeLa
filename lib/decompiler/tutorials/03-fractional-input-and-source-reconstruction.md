# Tutorial 03 — Fractional Input and Source Reconstruction

## Policies

| Policy | Purpose |
|---|---|
| PRESERVE | Keep fractional information in the reconstruction. |
| PARTIAL | Produce a bounded reconstruction while marking incomplete regions. |
| REPORT_ONLY | Report evidence and unresolved regions without reconstruction. |
| STRICT | Reject incomplete input when completeness is required. |

## Example

When a function has known entry points and basic blocks but lacks symbols and debug information, do not invent original names or source-level types.

Instead: known instructions -> known control flow -> inferred semantics -> unresolved metadata -> marked reconstruction.

The executable/library/source reference maps artifact -> format -> architecture/OS/ABI -> producer/toolchain -> language -> source forms -> reconstruction target.

If reconstruction is later used for a VM artifact, it must pass compiler and VM completeness checks. Decompiler confidence is not VM authorization.
