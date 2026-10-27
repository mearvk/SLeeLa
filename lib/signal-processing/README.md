<!-- Author: Max Rupplin — MEARVK LLC — 2026 -->

# SLeeLa Signal Processing Package

This directory is the SLeeLa-facing package location for signal-processing
services. The portable native C11/C++17 implementation, public headers, build
files, examples, tests, and engineering notes are maintained in the repository's
top-level [signal-processing/](../../signal-processing/README.md) directory.

## Package boundary

- Keep algorithm implementations and native regression tests in the top-level
  library; avoid maintaining a second copy under lib/.
- SLeeLa source-level facades can be added here when the runtime/native bridge
  contract is established and tested.
- A facade must report unavailable native functionality explicitly rather than
  claiming that a calculation has succeeded.
- Preserve the native library's documented buffer, numeric-input, and status
  contracts at any future bridge boundary.

## Current status

This is the package entry point and integration note. It does not yet claim to
provide SLeeLa wrappers or a VM bridge. See the native library's
[README](../../signal-processing/README.md) for implemented functionality,
build instructions, tests, and historical decade notes.

---

Max Rupplin — MEARVK LLC — 2026
