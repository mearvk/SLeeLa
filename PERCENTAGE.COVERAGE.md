# PERCENTAGE.COVERAGE.md

**Slecompiler™ — Input Coverage, Evidence Coverage, and Compilation/Decompilation Reference**

**Max Rupplin - MEARVK LLC - 2026**

## Purpose

Slecompiler coverage is reported in three separate dimensions:

1. **Format coverage** — recognition and structural parsing.
2. **Evidence coverage** — useful information recoverable from the artifact.
3. **Semantic/reconstruction coverage** — recoverable program meaning or source structure.

A percentage is not a claim about the world's binaries. Numbers below marked **target** are engineering targets and should eventually be replaced by measured benchmark results.

## Input coverage targets

| Input family | Structural target | Evidence target | Principal limitation |
|---|---:|---:|---|
| ELF executables | 95% | 85% | Stripping, optimization, indirect control flow |
| ELF shared objects | 95% | 85% | Runtime linking/environment |
| ELF relocatable objects | 95% | 90% | Toolchain metadata variation |
| GNU static archives | 98% | 92% | Compilation transformations |
| PE/COFF executables | 90% | 80% | Windows/compiler variation |
| PE/COFF libraries | 90% | 80% | Imports, exports, runtime loading |
| Mach-O executables | 90% | 80% | Slice/features/toolchain variation |
| Mach-O dynamic libraries | 90% | 80% | Runtime loading/binding |
| Kernel modules | 90% | 75% | Static evidence by default |
| Firmware/raw binary | 70% | 50% | Unknown format/architecture |
| Unknown/custom containers | 50% | 25% | Requires recognition/specification |

These are **engineering targets, not measured claims**.

## Static versus dynamic/shared libraries

GNU GCC describes static libraries as archives of object files, conventionally named `libNAME.a`, and shared libraries commonly as `libNAME.so`. When both are available, the linker generally prefers the shared form unless static linking is requested. citeturn0search0

### Static library

```
source -> compiler -> object files -> static archive -> linker -> executable
```

Static archives expose individual object members to analysis before final linking.

**Structural target: 98%. Evidence target: 92%.**

This does **not** mean 92% source reconstruction. Optimization, inlining, LTO, templates, generated code, stripping, and compiler choices can remove or transform source-level information.

### Dynamic/shared library

```
source -> compiler -> object files -> shared library -> runtime loader
```

GCC documents `-shared` for producing a shared object and `-static` for preventing shared-library linkage where supported. citeturn0search0turn0search12

**Structural target: 90%. Evidence target: 80%.**

Runtime symbol binding, load order, plugins, interposition, environment, and dynamically loaded modules can introduce evidence not contained in one shared-library file.

| Property | Static library | Dynamic/shared library |
|---|---:|---:|
| Container/member inspection | 98% target | 90% target |
| Static evidence | 92% target | 80% target |
| Runtime behavior from one file | Incomplete | Incomplete |
| Runtime dependency state | Lower relevance | Important |
| Source reconstruction | Incomplete | Incomplete |

## Compilation versus decompilation

Compilation is information-reducing:

```
source -> parsing -> optimization -> code generation -> object -> linking -> binary
```

Decompilation is an analytical reverse path:

```
binary -> format analysis -> decoding -> CFG/data flow -> IR/SLIR -> reconstruction
```

They are **not exact inverses**.

Compilers may remove, merge, reorder, inline, specialize, fold, or transform source constructs. Debug information and symbols may also be removed. GCC documents `-s` as removing symbol-table and relocation information. citeturn0search0

Therefore:

> **Instruction coverage is not source-code coverage.**

A binary can have 100% valid-instruction decoding while permitting substantially less than 100% reconstruction of the original source.

## Compilation evidence versus decompilation evidence

### Compilation-side evidence

Source, compiler/version/flags, preprocessing output, compiler IR, object files, debug information, symbols, relocations, link maps, LTO records, reproducible-build metadata, and build-system metadata can explain how an artifact was produced.

### Decompilation-side evidence

Slecompiler can work from headers, sections/segments, surviving symbols, relocations, imports/exports, instruction bytes, basic blocks, recovered functions, CFGs, data-flow relationships, strings/constants, exception/unwind metadata, compiler/runtime signatures, SLIR, and library/dependency relationships.

### The remainder

The **remainder of evidence** is information lost, transformed, externalized, or made runtime-dependent between compilation and the artifact available to Slecompiler.

Examples include comments, formatting, macro structure, original variable names, optimized-away variables, exact source-level type distinctions, original function boundaries after inlining, build configuration with no binary trace, runtime configuration, dynamically loaded code, JIT-generated code, external services/data, and environment-dependent behavior.

These should be reported as **unknown, transformed, runtime-dependent, or unsupported**, not silently reconstructed as facts.

## Per-artifact coverage

A future report can expose independent measurements:

```
Format Recognition       100%
Structural Parsing        98%
Instruction Decoding      96%
Symbol Evidence            72%
CFG Recovery               84%
Function Recovery          79%
SLIR Lifting               81%
Semantic Reconstruction    61%
Source Reconstruction      35%
Runtime Behavior            N/A
```

Those values are **illustrative only**.

Every percentage requires a denominator. For example:

```
instruction coverage = decoded eligible instructions / eligible instructions
CFG coverage          = recovered regions / identified regions
symbol coverage       = resolved symbols / available symbol records
```

## Measurement vocabulary

- **Measured** — calculated from the current artifact and analyzer results.
- **Target** — engineering objective.
- **Estimated** — model-based approximation, not a benchmark.
- **Unknown** — insufficient evidence for a meaningful percentage.

A parser accepting a file must not be counted as equivalent to complete analysis.

## Benchmark corpus

Measured coverage should eventually be derived from a corpus containing stripped/unstripped ELF; static/dynamic libraries; PE executables/DLLs; Mach-O executables/dynamic libraries; relocatable objects; multi-member archives; debug/non-debug builds; multiple optimization levels; LTO/non-LTO builds; C/C++ and other supported native-language outputs; PIE/PIC; static/dynamic executables; kernel modules; generated and hand-written assembly; malformed/truncated artifacts; and unsupported/ambiguous inputs.

## References

**Compiler/linker reference:** GNU GCC *Link Options*, including static/shared libraries, `-static`, `-shared`, PIE, symbols, and relocations. citeturn0search0

**Whole-program/LTO reference:** GNU GCC Internals *WHOPR*, documenting whole-program assumptions and limitations around externally visible functions and variables. citeturn0search6

**Slecompiler references:** `decompiler/API.md`, `decompiler/TUTORIAL.md`, `TERMINOLOGY.md`, `build/PRODUCTS.md`, and this document.

## Governing principle

**Slecompiler coverage means coverage of evidence, not a promise to recreate the original source exactly.**

```
preserved evidence + structural recovery + semantic analysis
    = greater decompilation coverage

greater decompilation coverage
    != original source equivalence
```
