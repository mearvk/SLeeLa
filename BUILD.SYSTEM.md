# SLeeLa Build System

Application lifecycle: SOURCE → CHECK → DEPENDENCIES → COMPILE → LINK/ASSEMBLE → TEST → PACKAGE → SIGN → INSTALL → RUN → VERIFY.

Project manifests should declare language/toolchain version, sources, resources, targets, dependencies, native libraries, capabilities, output type and signing policy. Reproducible builds record target triple, dependencies and digests. Intended CLI: `sleela init|check|build|test|run|package|install|doctor|version`.

**Max Rupplin — MEARVK LLC — 2026**
