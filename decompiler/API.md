# Slecompiler™ API Guide

**Max Rupplin - MEARVK LLC - 2026**

Slecompiler™ exposes a C++ analysis library under `decompiler/`. The API follows a
read-only static-analysis pipeline:

```
artifact
  -> container identification
  -> format/metadata decoding
  -> symbols and sections
  -> instruction decoding
  -> control-flow analysis
  -> SLIR
  -> higher-level analysis
  -> library relationships / report
```

## Product boundary

Slecompiler is an analysis product, not an execution sandbox. Opening or decoding an
artifact does not imply permission to execute it. The default product path does not
load kernel modules, perform device I/O, write target memory, or invoke recovered code.

Supported artifact families include ELF, PE/COFF, Mach-O where implemented, GNU/static
archives, relocatable objects, kernel modules as static evidence, and raw/firmware
artifacts.

## API concepts

| Concept | Responsibility |
|---|---|
| Artifact | Input native artifact and identity/container information |
| Decoder | Native-byte decoding into structured evidence |
| ELF / PE support | Format-specific headers, sections, symbols, and relocation metadata |
| Archive | Static-library container and object members |
| Library | Library/family model for related native artifacts |
| Analyzer | Higher-level static analysis |
| SLIR | Lifted intermediate representation |
| VM | Analysis-oriented representation/execution model; it does not execute the input artifact |

The exact C++ declarations remain authoritative in `decompiler/include/`. This guide
documents the contract without inventing declarations that are not yet stabilized.

## API exemplars

With `SLEE_LA_BUILD_API_EXAMPLES=ON` (the current CMake default), the product builds:

- `slecompiler-api-inspect`
- `slecompiler-api-cfg`
- `slecompiler-api-library`
- `slecompiler-api-graph`
- `slecompiler-api-slir-vm`

Their sources are under `decompiler/examples/`.

They demonstrate:
1. artifact identity and metadata inspection;
2. decoding and control-flow analysis;
3. library/archive metadata;
4. library-family dependency relationships;
5. SLIR and analysis-VM work.

## Recommended application sequence

1. Accept an input path under the caller's authorization policy.
2. Open the artifact without executing it.
3. Identify its container and format.
4. Collect metadata and provenance.
5. Decode the structures needed for the requested analysis.
6. Recover functions/control flow where supported.
7. Lift supported material into SLIR.
8. Run higher-level analysis.
9. Emit an evidence-based report.
10. Keep original input separate from generated analysis output.

Reports should distinguish **observed evidence**, **derived analysis**, and
**unknown/unsupported properties**. Never invent symbols, source lines, calling
conventions, or hardware facts.

## Normal analysis limitations

Unknown formats, truncated files, malformed headers, unsupported architectures,
stripped symbols, unresolved indirect control flow, unsupported instructions, missing
dependencies, and undecodable archive members are normal analysis outcomes. An
unrecovered fact should remain explicitly unknown.

## Platform contract

The product targets Linux, macOS, and Windows 10+. The native library is C++20.
Platform-specific build mechanics belong in `build/`; the analysis contract remains
in `decompiler/`.

See `decompiler/TUTORIAL.md` for the developer walkthrough.
