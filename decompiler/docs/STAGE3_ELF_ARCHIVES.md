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


## Stage 4 started

Stage 4 begins native recovery with architecture-aware x86/x86-64 control-transfer decoding and branch-aware CFG construction. The decoder currently recognizes common relative calls, unconditional jumps, conditional jumps, and returns. Unknown instructions remain represented conservatively rather than guessed.

The analyzer now forms basic blocks at branch targets and control-transfer fallthrough points, then emits CFG edges for conditional and unconditional branches. This is the beginning of function-boundary recovery; it is not yet a complete x86 decoder or production-grade decompiler.


## Stage 4 implementation note

The Stage 4 native-recovery layer has been expanded with:

- A contained x86/x86-64 decoder path for common register pushes/pops, returns, direct relative calls/jumps, conditional branches, immediate-register moves, and indirect FF /2 calls and FF /4 jumps.
- Basic ModRM/SIB-aware operand descriptions for register and common memory forms.
- Relocation evidence attached to decoded instructions when relocation addresses match the decoder address space.
- CFG construction that keeps branch targets as edges and preserves fallthrough after calls and conditional branches.
- Function-candidate recovery that incorporates non-external symbol and export evidence in addition to generic block starts.
- A minimal x86-64 ELF fixture exercising direct call, conditional branch, indirect call and return decoding.

The implementation intentionally remains conservative. Unsupported x86 instructions are emitted as db records rather than guessed, and ARM/ARM64 continue to use the non-decoding fallback until dedicated architecture decoders are added.
