# Slecompiler™ API Guide

**Max Rupplin - MEARVK LLC - 2026**

Slecompiler™ exposes a C++ analysis library under `decompiler/`. The API is organized
around a deliberately read-only analysis pipeline:

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

## 1. Product boundary

Slecompiler is an analysis product, not an execution sandbox. Opening or decoding an
artifact does not imply permission to execute it. The default product path does not
load kernel modules, perform device I/O, write target memory, or invoke recovered code.

The principal artifact families are:

- ELF executables and shared objects;
- PE/COFF executables and libraries;
- Mach-O artifacts where supported by the implementation;
- GNU/static archives;
- relocatable objects;
- kernel modules as static ELF evidence;
- raw or firmware artifacts.

## 2. API concepts

The implementation is divided into cooperating concepts rather than one monolithic
decompiler call:

| Concept | Responsibility |
|---|---|
| Artifact | Represents an input native artifact and its identity/container information |
| Decoder | Converts recognized native bytes into structured instruction/format evidence |
| ELF / PE support | Format-specific headers, sections, symbols, and relocation metadata |
| Archive | Represents static-library containers and their object members |
| Library | Models a library or family of related native artifacts |
| Analyzer | Performs higher-level static analysis over decoded information |
| SLIR | Slecompiler's lifted intermediate representation used between decoding and later analysis |
| VM | Provides the product's analysis-oriented representation/execution model without executing the input artifact itself |

The exact C++ declarations remain authoritative in the headers under
`decompiler/include/`. This document intentionally describes the contract at the
conceptual level so the public guide does not drift from implementation names.

## 3. API exemplars

The CMake project builds these focused programs when
`SLEE_LA_BUILD_API_EXAMPLES=ON` (the current default):

- `slecompiler-api-inspect`
- `slecompiler-api-cfg`
- `slecompiler-api-library`
- `slecompiler-api-graph`
- `slecompiler-api-slir-vm`

Their source files are under `decompiler/examples/`.

The examples are intended to answer five common application questions:

1. **What is this artifact?**
2. **What instructions and control-flow relationships can be recovered?**
3. **What library/archive metadata can be extracted?**
4. **How are native artifacts related as a software family?**
5. **How does decoded material move into SLIR and the analysis VM?**

## 4. Recommended application sequence

An application integrating Slecompiler should keep the stages explicit:

1. Accept an input path under the caller's authorization policy.
2. Open the artifact without executing it.
3. Identify the container/format.
4. Collect format metadata and provenance.
5. Decode only the structures needed for the requested analysis.
6. Build control-flow/function evidence where supported.
7. Lift supported material into SLIR.
8. Run higher-level analysis.
9. Emit a report containing evidence and limitations.
10. Preserve the original artifact separately from generated analysis output.

A report should distinguish **observed evidence**, **derived analysis**, and
**unknown/unsupported properties**. Do not manufacture symbols, source lines,
calling conventions, or hardware facts when the artifact does not establish them.

## 5. Failure handling

Applications should treat these conditions as normal analysis outcomes:

- unknown file format;
- truncated artifact;
- malformed header or section table;
- unsupported machine architecture;
- incomplete symbol information;
- stripped symbols;
- unresolved indirect control flow;
- unsupported instruction;
- missing dependency;
- archive member that cannot be decoded.

An inability to recover a fact is evidence of an analysis limitation, not permission
to substitute a guessed value.

## 6. Platform contract

The Slecompiler product is designed for:

- Linux;
- macOS;
- Windows 10+.

The native library is C++20. Platform-specific build behavior belongs in `build/`;
the analysis API remains a product-level interface.

See `build/PRODUCTS.md` for the build contract and `decompiler/TUTORIAL.md` for
a complete developer walkthrough.
