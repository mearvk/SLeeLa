# SLeeLa Decompiler Descriptor

**Max Rupplin - MEARVK LLC - 2026**

This descriptor defines the investigation workflow for PE/DLL/SYS, ELF and raw binary artifacts.

### Evidence
Record source path, acquisition timestamp, SHA-256, file size, format and architecture.

### Parsing
Parse container headers and mappings without executing code.

### Recovery
Recover symbols, imports, exports, relocations, strings, instructions, basic blocks and function candidates. Mark inferred information with confidence and provenance.

### Refactoring
Transform SLIR into normalized C/C++, pseudocode, API documentation, diagrams or test programs. Never silently overwrite the original artifact.

### Driver-specific records
Driver analysis may document service metadata, dispatch evidence, IRP-related symbols when present, imports/exports, section permissions, device-string references and version resources. This is static analysis only.

### Storage
Every analysis bundle should retain the original digest, tool version, architecture, parser version, SLIR version and generated-file hashes.

## API exemplars and tutorial

The implementation is accompanied by runnable API exemplars in `decompiler/examples/` covering artifact inspection, decoding/CFG construction, library metadata, LibraryGraph, and SLIR/VM validation. The complete product walkthrough is maintained in `decompiler/docs/TUTORIAL.md`.

## Analysis-manifest baseline

A reproducible analysis record should retain at least:

- artifact path or acquisition identifier;
- byte size and SHA-256;
- Slecompiler version;
- parser/format version;
- architecture and platform;
- analysis configuration and enabled stages;
- recovered evidence and provenance;
- generated-output hashes.

This separates immutable input identity from later inferred or generated material.
