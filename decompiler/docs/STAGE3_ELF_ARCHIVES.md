# Slecompiler™ Stage 3 — Native ELF and Archive Intelligence

Stage 3 extends the static-analysis foundation without executing native artifacts.

## Implemented

- ELF program-header and load-segment records.
- GNU build-ID extraction from PT_NOTE.
- GNU RELRO, NOW/BIND_NOW, and non-executable-stack evidence.
- Dynamic metadata for SONAME, DT_NEEDED, PLT/GOT, init/fini arrays and functions.
- TLS section detection.
- GNU symbol-version definition and requirement extraction.
- ELF REL and RELA relocation records with architecture-aware names for common x86, x86-64, ARM and AArch64 relocations.
- Relocations retain their section and symbol association.
- GNU ar archive member discovery with member offsets, sizes and basic ELF/PE classification.
- Archive members are recorded as container metadata; member code is not executed.

## Analysis model

Slecompiler treats ELF program headers as the loader-facing view and sections as the linker/debugging view. Relocations and dynamic metadata are retained as evidence rather than applied to process memory.

For kernel modules, the parser continues to read .modinfo and recognizes ksymtab-related sections without inserting or loading a .ko.

## Remaining work

- Complete version-definition/requirement graph semantics and attach versions directly to symbols/imports.
- Parse GNU/SysV hash tables and use them for provider resolution.
- Recover PLT/GOT entries precisely from section and relocation relationships.
- Parse __ksymtab records into named exported/imported kernel interfaces.
- Recursively analyze ELF/COFF archive members as child artifacts.
- Add compressed kernel-module adapters for supported compression formats.
- Replace the current non-cryptographic identity digest with real SHA-256.
- Add fixture-based tests for real ELF32/ELF64 shared objects, relocatable objects, archives and .ko files.

## Safety boundary

The Stage 3 parser is bounded and read-only. It does not load ELF objects, execute instructions, insert kernel modules, open devices, or modify target memory.
