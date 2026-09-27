# HTTP Build Layout

SLeeLa HTTP sources have accumulated under more than one historical directory
name. This document defines the build rule without moving or deleting source.

## Rule

Each HTTP grade should expose:
- C reference source where the grade has a native C implementation.
- C++ reference source where the grade has a native C++ implementation.
- A `build/` directory containing build/check instructions.
- Configuration and specification files beside the implementation.
- No generated object files or binaries committed to the source directory.

Existing `http-2.0/` and `http-3.0/` Makefiles remain authoritative for their
mature protocol trees. The newer `http/7.0/`, `http/8.0/`, and `http/9.0/`
trees use the uniform syntax-check Makefiles supplied in their `build/`
directories.

The central `http/build/` tools locate both naming conventions
(`http/X.Y` and `http-X.Y`) so future grades do not require a directory
rename.

## Verification

Run `http/build/audit.sh` from the repository root. It checks:
- C/C++ source presence by grade when applicable.
- matching build directories for the standardized newer trees.
- absence of generated artifacts under source directories.
- the HTTP/8 crypto policy files.
- the HTTP/9 configuration/specification files.

The audit is structural; it does not claim that a protocol is production-ready
merely because its sources compile.
