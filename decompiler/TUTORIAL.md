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

    powershell -ExecutionPolicy Bypass -File .\build\slecompiler-windows.ps1

## 2. Build the API exemplars

    cmake -S decompiler -B build/slecompiler/dev -DSLEE_LA_BUILD_API_EXAMPLES=ON
    cmake --build build/slecompiler/dev

## 3. Inspect first

The inspection layer establishes artifact identity and available metadata before deeper
analysis. A missing symbol table means the selected artifact did not provide that
evidence; it does not prove that the original program had no such symbol.

## 4. Decode and recover control flow

The CFG path is:

    bytes -> decoder -> instructions -> basic blocks -> CFG

CFG recovery can be incomplete for indirect branches, computed calls, obfuscation,
malformed input, or unsupported instructions.

## 5. Libraries and archives

The library model captures members, exports, imports, ABI evidence, and dependency
relationships.

## 6. SLIR

    native evidence -> SLIR -> analysis-oriented representation

SLIR is the common intermediate point for later source projection. It is not a
guarantee of exact source-code reconstruction.

## 7. Select source output

Slecompiler supports four source-output targets:

    sleela-decompiler decompile ./program --output java
    sleela-decompiler decompile ./program --output sleela
    sleela-decompiler decompile ./program --output c
    sleela-decompiler decompile ./program --output c++

Save output with:

    sleela-decompiler decompile ./program --output c++ --file program.cpp

The language is selected **after** native decoding, CFG recovery, and function
recovery. It is therefore a source-output target, not a claim about the original
input language.

### Output architecture

    native artifact
         |
         v
    format / instruction analysis
         |
         v
    CFG + recovered functions
         |
         v
        SLIR
         |
         +----> Java
         +----> Sleela
         +----> C
         +----> C++

## 8. Evidence model

| Category | Meaning |
|---|---|
| Observed | Directly present in the artifact or authoritative metadata |
| Derived | Computed from observed evidence |
| Unknown | Not established by available evidence or implementation |

Generated source must preserve this distinction.

## 9. Kernel modules and drivers

Kernel modules are treated as untrusted static artifacts. The default path can inspect
their metadata and binary structure but does not load them into the running kernel.

## 10. Cross-platform integration

The product targets Linux, macOS, and Windows 10+. Build mechanics stay in `build/`.
The output-language interface is platform-neutral.
