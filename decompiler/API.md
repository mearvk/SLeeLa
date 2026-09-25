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
  -> source-output projection
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
| SourceEmitter | Evidence-based projection into Java, Sleela, C, or C++ |

The exact C++ declarations remain authoritative in `decompiler/include/`.

## Source output targets

The CLI accepts an explicit output-language argument:

```
sleela-decompiler decompile <file> --output <java|sleela|c|c++>
```

Examples:

```
sleela-decompiler decompile program --output java
sleela-decompiler decompile program --output sleela --file decompiled.sleela
sleela-decompiler decompile program --output c --file decompiled.c
sleela-decompiler decompile program --output c++ --file decompiled.cpp
```

Aliases: `sl` selects Sleela and `cpp` selects C++.

| Target | Extension | Output role |
|---|---|---|
| Java | `.java` | Java-oriented source reconstruction |
| Sleela | `.sleela` | Native SLeeLa/Sleela source representation |
| C | `.c` | C-oriented source reconstruction |
| C++ | `.cpp` | C++20-oriented source reconstruction |

The four targets share the recovered native evidence and analysis. Selecting an output
language does **not** claim that the original program was written in that language.
Unresolved semantics remain marked as evidence/unknowns rather than invented facts.

## Recommended application sequence

1. Accept an input path under the caller's authorization policy.
2. Open the artifact without executing it.
3. Identify its container and format.
4. Collect metadata and provenance.
5. Decode the structures needed for the requested analysis.
6. Recover functions/control flow where supported.
7. Lift supported material into SLIR.
8. Run higher-level analysis.
9. Select the requested source-output language.
10. Emit an evidence-based source artifact or report.

Reports and generated source should distinguish observed evidence, derived analysis, and
unknown/unsupported properties.
