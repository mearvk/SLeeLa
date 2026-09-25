# Slecompiler™ Developer Tutorial

**Max Rupplin - MEARVK LLC - 2026**

This tutorial takes a developer from a native artifact to structured Slecompiler
analysis without executing the artifact.

## 1. Build

Linux:

    ./build/slecompiler-linux.sh

macOS:

    ./build/slecompiler-macos.sh

Windows 10+:

    powershell -ExecutionPolicy Bypass -File .\\build\\slecompiler-windows.ps1

These entry points build the authoritative `decompiler/CMakeLists.txt`.

## 2. Build the API exemplars

The current CMake configuration enables exemplars by default:

    cmake -S decompiler -B build/slecompiler/dev -DSLEE_LA_BUILD_API_EXAMPLES=ON
    cmake --build build/slecompiler/dev

Targets:

    slecompiler-api-inspect
    slecompiler-api-cfg
    slecompiler-api-library
    slecompiler-api-graph
    slecompiler-api-slir-vm

## 3. Inspect first

The inspection layer establishes artifact identity and available metadata before
deeper analysis:

    input path
       |
       v
    Artifact
       |
       +--> format / architecture
       +--> sections
       +--> symbols
       +--> provenance
       |
       v
    inspection report

A missing symbol table means the selected artifact did not provide that evidence; it
does not prove that the original program had no such symbol.

## 4. Decode and recover control flow

The CFG exemplar demonstrates:

    bytes -> decoder -> instructions -> basic blocks -> CFG

CFG recovery can be incomplete for indirect branches, computed calls, obfuscation,
malformed input, or unsupported instructions. Reports should preserve those limits.

## 5. Libraries and archives

The library exemplar treats native software as a family:

    archive / shared library
          |
          +--> members / exports / imports
          +--> ABI and metadata evidence
          +--> dependency relationships
          |
          v
       library model

The graph exemplar provides the higher-level relationship view.

## 6. SLIR

The SLIR/VM exemplar demonstrates:

    native evidence -> SLIR -> analysis-oriented VM

SLIR is an intermediate analysis representation. It is not a guarantee of exact
source-code reconstruction.

## 7. Evidence model

| Category | Meaning |
|---|---|
| Observed | Directly present in the artifact or authoritative metadata |
| Derived | Computed from observed evidence |
| Unknown | Not established by the available evidence or implementation |

This distinction matters for stripped binaries, optimized code, indirect calls,
compiler-generated constructs, and kernel modules.

## 8. Kernel modules and drivers

Kernel modules are treated as untrusted static artifacts. The default path can
inspect their metadata and binary structure but does not load them into the running
kernel.

## 9. Cross-platform integration

The same analysis concepts apply on Linux, macOS, and Windows 10+. Build mechanics
stay in `build/`. Windows uses the PowerShell/CMake entry point; macOS supports host
architecture selection and explicit universal-architecture configuration.

## 10. Documentation growth

As the C++ interfaces stabilize, the API documentation should map each public header
to object ownership, error/result conventions, architecture selection, symbol and
relocation records, CFG/function recovery, SLIR nodes, library graph queries, report
serialization, and compatibility guarantees. Documentation should follow actual
interfaces rather than inventing APIs ahead of implementation.
