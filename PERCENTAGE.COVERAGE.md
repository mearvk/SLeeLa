# PERCENTAGE.COVERAGE.md

**Slecompiler™ — Input Coverage, Evidence Coverage, Definitions, and Compilation/Decompilation Reference**

**Max Rupplin - MEARVK LLC - 2026**

## Purpose

Slecompiler coverage is reported in separate dimensions so that **recognizing an input**, **recovering evidence from it**, and **reconstructing program meaning** are never treated as the same accomplishment.

The principal dimensions are:

1. **Format coverage** — recognition and structural parsing.
2. **Structural coverage** — successful identification of meaningful binary structures.
3. **Instruction coverage** — decoding of eligible machine-code regions.
4. **Evidence coverage** — useful information recoverable from the artifact.
5. **Control-flow coverage** — recovery of basic blocks, edges, and control-flow regions.
6. **Function coverage** — identification and recovery of executable functions or function-like regions.
7. **Semantic coverage** — recovery of higher-level behavior and relationships.
8. **Reconstruction coverage** — recovery of source-like program structure.
9. **Runtime coverage** — evidence that depends on execution, loading, environment, JIT activity, external services, or other state not necessarily present in the artifact.

A percentage is not a claim about all binaries in existence. Numbers marked **target** are engineering targets and should eventually be replaced by measured benchmark results.

---

## Core Definitions

### Artifact

An **artifact** is a file or collection of files presented to Slecompiler for analysis.

Examples include executables, object files, static archives, shared libraries, dynamic libraries, kernel modules, firmware images, memory images, and recognized binary containers.

An artifact is the **thing being observed**, not necessarily the original source from which it was produced.

### Input

An **input** is an artifact or artifact set supplied to a Slecompiler analysis operation.

Inputs may be:

- recognized;
- partially recognized;
- malformed;
- truncated;
- ambiguous;
- unsupported;
- or valid but lacking sufficient evidence for a requested analysis.

### Format Recognition

**Format recognition** determines whether Slecompiler can identify the container, executable format, object format, architecture, machine type, byte order, and other top-level properties.

Recognition does not imply complete analysis.

### Structural Parsing

**Structural parsing** means successfully reading known structures within a recognized artifact.

Examples include:

- headers;
- sections;
- segments;
- program headers;
- symbol tables;
- relocation tables;
- import/export records;
- dynamic-link information;
- exception/unwind records;
- archive members;
- code/data boundaries where evidence permits.

### Structural Coverage

**Structural coverage** measures how much of the expected, eligible structure Slecompiler successfully identifies and parses.

A file opening successfully is not equivalent to 100% structural coverage.

### Instruction

An **instruction** is a machine-code operation represented by bytes and decoded according to a particular instruction-set architecture and decoding mode.

An instruction is not automatically equivalent to a source statement, expression, function, or original compiler operation.

### Instruction Decoding

**Instruction decoding** maps eligible machine-code bytes to instruction representations.

Correct decoding can be achieved even when the original source structure cannot be reconstructed.

### Instruction Coverage

A useful definition is:

```
instruction coverage =
decoded eligible instructions / eligible instructions
```

The denominator must be defined by the benchmark or analysis policy. Unknown bytes, embedded data, deliberately excluded regions, and ambiguous code/data boundaries must not silently inflate the percentage.

### Symbol

A **symbol** is a named or otherwise represented reference to an entity in an object or executable artifact.

Symbols may represent functions, objects, sections, TLS entities, imports, exports, or other linker/toolchain concepts.

A symbol may be:

- local;
- global;
- weak;
- dynamic;
- imported;
- exported;
- stripped;
- synthesized;
- or otherwise toolchain-specific.

### Symbol Evidence

**Symbol evidence** is information about symbols that survives into the artifact and can be recovered by Slecompiler.

A stripped artifact may contain substantially less symbol evidence than an unstripped artifact.

### Relocation

A **relocation** is metadata describing an address/reference adjustment that must be resolved or applied by a linker, loader, or related processing stage.

Relocations can provide valuable evidence about references and layout, but their presence and form vary by object format, architecture, linker, and build configuration.

### Section

A **section** is a format-defined region containing code, data, metadata, symbols, relocations, or other information.

Sections are especially important in relocatable objects and toolchain-oriented analysis, although executable formats may also contain segment-oriented loading structures.

### Segment

A **segment** is a load-oriented region described by an executable format. Segments may combine or map multiple sections and describe permissions, addresses, alignment, and other loader-relevant properties.

### Import

An **import** is a reference to a symbol or facility supplied outside the analyzed artifact, commonly by another library or runtime component.

### Export

An **export** is a symbol or entry made available for use by other artifacts or runtime components.

### Static Library

A **static library** is commonly an archive containing object files. GNU GCC documents the conventional Unix-like form as `libNAME.a`.

A static archive can expose individual object members before a final executable is linked. This can make archive membership and object-level evidence particularly useful to static analysis.

### Shared / Dynamic Library

A **shared library** is a library intended to be linked or loaded separately from the final executable. GNU GCC documents `-shared` for producing a shared object and commonly uses `libNAME.so` on Unix-like systems.

The exact naming and loading model is platform-dependent. Windows commonly uses PE/COFF DLLs, while macOS commonly uses Mach-O dynamic libraries.

### Dynamic Linking

**Dynamic linking** is a linking/loading model in which some library relationships remain external to the final executable and are resolved by a linker, loader, or runtime mechanism.

Dynamic linking may therefore introduce evidence that is distributed across multiple artifacts.

### Static Linking

**Static linking** incorporates selected library object code into a linked artifact rather than requiring those objects to remain external shared-library dependencies.

Static linking does not guarantee that every dependency is embedded, nor does it guarantee that original source structure remains recoverable.

### Runtime Loader

The **runtime loader** is the operating-system or runtime component responsible for loading executable components and resolving applicable runtime dependencies, relocations, symbols, or related state.

### Dependency

A **dependency** is an external artifact, library, runtime facility, service, or other required component on which program operation or interpretation depends.

### Runtime Evidence

**Runtime evidence** is information that exists only, or becomes fully observable, when an artifact is loaded or executed in an environment.

Slecompiler's static analysis must distinguish runtime evidence from evidence actually present in the artifact.

---

## Input Coverage Targets

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

---

## Static Versus Dynamic/Shared Libraries

GNU GCC describes static libraries as archives of object files, conventionally named `libNAME.a`, and shared libraries commonly as `libNAME.so`. When both forms are available, the linker generally prefers the shared form unless static linking is requested.

Reference: GNU GCC, [Link Options](https://gcc.gnu.org/onlinedocs/gcc/Link-Options.html).

### Static library

Typical path:

```
source -> compiler -> object files -> static archive -> linker -> executable
```

**Structural target: 98%. Evidence target: 92%.**

Static archives can preserve object-member boundaries and relocation/symbol information that may be useful to Slecompiler.

However, static-library analysis is not source reconstruction. Optimization, inlining, LTO, templates, generated code, stripping, and compiler choices can remove or transform source-level information.

### Dynamic/shared library

Typical path:

```
source -> compiler -> object files -> shared library -> runtime loader
```

GCC documents `-shared` for producing a shared object and `-static` for preventing shared-library linkage where supported.

**Structural target: 90%. Evidence target: 80%.**

Dynamic/shared-library analysis must account for:

- imports and exports;
- runtime symbol binding;
- relocation/binding state;
- load order;
- library search paths;
- interposition;
- plugins;
- dynamically loaded modules;
- runtime configuration;
- operating-system loader behavior.

### Comparison

| Property | Static library | Dynamic/shared library |
|---|---|---|
| Container/member inspection | 98% target | 90% target |
| Static evidence | 92% target | 80% target |
| Runtime behavior from one file | Incomplete | Incomplete |
| Runtime dependency state | Lower relevance | Important |
| Source reconstruction | Incomplete | Incomplete |
| Evidence distributed across other files | Sometimes | Frequently |
| Loader-dependent behavior | Lower | Higher |

These percentages are targets, not empirical claims.

---

## Compilation Versus Decompilation

Compilation is generally information-reducing:

```
source
  -> preprocessing/parsing
  -> intermediate representation
  -> optimization
  -> code generation
  -> object
  -> linking
  -> executable/library
```

Decompilation is an analytical reverse path:

```
binary
  -> format analysis
  -> decoding
  -> control-flow recovery
  -> data-flow analysis
  -> IR/SLIR
  -> semantic analysis
  -> source-like reconstruction
```

They are **not exact inverses**.

Compilers may:

- remove unused code;
- merge operations;
- reorder instructions;
- inline functions;
- specialize templates;
- fold constants;
- eliminate variables;
- transform control flow;
- lower abstractions;
- generate helper functions;
- alter calling conventions;
- perform whole-program optimization.

Debug information and symbols may also be removed. GCC documents `-s` as removing symbol-table and relocation information.

Reference: GNU GCC, [Link Options](https://gcc.gnu.org/onlinedocs/gcc/Link-Options.html).

Therefore:

> **Instruction coverage is not source-code coverage.**

A binary can have 100% valid-instruction decoding while permitting substantially less than 100% reconstruction of the original source.

---

## Additional Compilation Definitions

### Source Code

The human-authored or generated textual program representation supplied to a compiler or related translation system.

### Preprocessing

The transformation stage that expands or interprets preprocessing constructs such as macros and conditional compilation before or as part of language compilation.

### Intermediate Representation (IR)

A compiler-oriented representation between source-level constructs and final machine code.

IR is generally richer than final machine code for compiler analysis, but it is not necessarily preserved in a released binary.

### Optimization

A transformation intended to improve properties such as performance, size, power consumption, or other objectives while preserving required program behavior.

Optimization can reduce the correspondence between source constructs and final machine instructions.

### Code Generation

The compiler stage that converts an intermediate representation into target-specific machine code or another lower-level representation.

### Object File

A compiled binary unit containing machine code and/or data plus metadata used for relocation, linking, debugging, or other toolchain operations.

### Relocatable Object

An object file intended for further linking. It commonly retains relocation records and symbol information needed by later stages.

### Linker

A tool that combines object files and libraries, resolves applicable symbols, performs relocations, and produces a linked artifact.

### Executable

A linked artifact intended to be loaded and executed by an operating system, runtime, firmware environment, or other execution environment.

### Position-Independent Code (PIC)

Machine code designed so that it can execute correctly regardless of the absolute address at which it is loaded, subject to platform and ABI rules.

### Position-Independent Executable (PIE)

An executable produced so that it can be loaded at varying addresses under a platform's supported dynamic-loading model.

### Link-Time Optimization (LTO)

Optimization performed using compiler information retained for later processing at link time or an equivalent whole-program stage.

LTO can improve optimization across translation-unit boundaries while also changing the relationship between original source functions and final machine-code regions.

### Stripped Artifact

An artifact from which some symbols, debug information, relocation information, or other metadata have been removed.

Stripping can reduce the evidence available to static analysis.

### Debug Information

Metadata intended to associate machine-code or object-level entities with source-level information such as files, lines, types, variables, and scopes.

Debug information can substantially improve reconstruction when it survives into the analyzed artifact.

### ABI

An **Application Binary Interface** defines binary-level conventions such as calling conventions, data layout, symbol conventions, object formats, and interoperability rules.

ABI knowledge is important to function recovery and semantic analysis.

### Calling Convention

A platform/compiler convention describing how arguments, return values, registers, stack frames, preserved registers, and related state are used across calls.

### Control-Flow Graph (CFG)

A graph representing possible control-flow relationships between basic blocks or other executable regions.

### Basic Block

A sequence of instructions with a single entry point and a control-flow structure that permits analysis as one basic unit.

### Data Flow

The movement and transformation of values through registers, memory, parameters, return values, and instructions.

### Function Recovery

The process of identifying and reconstructing function-like executable regions from binary evidence.

Function boundaries are not guaranteed to correspond exactly to original source-level functions.

### Semantic Reconstruction

The process of inferring higher-level behavior from recovered instructions, control flow, data flow, types, symbols, constants, and other evidence.

Semantic reconstruction is an inference process and must distinguish observed facts from derived conclusions.

### Source Reconstruction

The production of source-like output from binary evidence.

Source reconstruction is not guaranteed to reproduce the original:

- comments;
- formatting;
- names;
- macros;
- templates;
- types;
- source organization;
- control structures;
- or build configuration.

### Equivalence

Two representations are **equivalent** only under a defined equivalence criterion.

Textual similarity is not proof of semantic equivalence, and semantic similarity is not proof that the reconstructed text was the original source.

---

## Compilation Evidence Versus Decompilation Evidence

### Compilation-side evidence

Compilation-side evidence can include:

- source;
- compiler and compiler version;
- compiler flags;
- linker flags;
- preprocessing output;
- compiler IR;
- object files;
- debug information;
- symbols;
- relocations;
- link maps;
- LTO records;
- reproducible-build metadata;
- build-system metadata;
- dependency manifests.

This evidence can explain how an artifact was produced, but it may not accompany the released binary.

### Decompilation-side evidence

Slecompiler can work from:

- file headers;
- sections;
- segments;
- surviving symbols;
- relocations;
- imports;
- exports;
- instruction bytes;
- basic blocks;
- recovered functions;
- CFGs;
- data-flow relationships;
- strings;
- constants;
- exception/unwind metadata;
- compiler/runtime signatures;
- SLIR;
- library relationships;
- dependency relationships.

### Observed Evidence

**Observed evidence** is information directly supported by the analyzed artifact or explicitly supplied analysis input.

Examples:

- a byte sequence;
- a section name;
- an exported symbol;
- an instruction;
- a relocation record.

### Derived Evidence

**Derived evidence** is an analytical result calculated from observed evidence.

Examples:

- a recovered CFG edge;
- a probable function boundary;
- an inferred calling convention;
- a likely library signature.

Derived evidence should be labeled as derived rather than presented as though it were directly stored in the binary.

### Inference

An **inference** is a conclusion produced from available evidence according to an analysis method.

An inference may be strong, weak, ambiguous, or unsupported depending on the evidence.

### Confidence

**Confidence** describes the strength or reliability assigned to an analytical result.

Confidence is not the same thing as percentage coverage.

A function recovery result can have high confidence while the overall artifact has low function coverage.

---

## The Remainder of Evidence

The **remainder of evidence** is information that is:

- lost during compilation;
- transformed by optimization;
- removed by stripping;
- externalized into dependencies;
- stored in separate debug/build artifacts;
- generated at runtime;
- dependent on environment;
- dynamically loaded;
- generated by a JIT;
- supplied by external services;
- or otherwise unavailable to the static artifact analysis.

Examples include:

- comments;
- formatting;
- macro structure;
- original variable names;
- optimized-away variables;
- exact source-level type distinctions;
- original function boundaries after inlining;
- build configuration with no binary trace;
- runtime configuration;
- dynamically loaded code;
- JIT-generated code;
- external services/data;
- environment-dependent behavior.

These should be reported as **unknown, transformed, runtime-dependent, unavailable, or unsupported**, rather than silently reconstructed as facts.

---

## Coverage Is Not Completeness

A coverage percentage answers a defined question about a defined denominator.

It does not mean:

- the whole program has been understood;
- the original source has been recovered;
- runtime behavior has been observed;
- every dependency has been analyzed;
- every function has been recovered;
- or the original source text has been reproduced.

### Example

If an artifact contains 10,000 eligible instructions and Slecompiler correctly decodes 9,500:

```
instruction coverage = 9,500 / 10,000 = 95%
```

That does **not** mean:

```
95% of the original source was recovered
```

The two measurements have different denominators and different meanings.

---

## Coverage Categories

### Format Coverage

Can Slecompiler recognize the input and identify its basic format?

### Structure Coverage

Can Slecompiler parse the meaningful structures expected for that format?

### Code Coverage

How much eligible machine code can be identified and decoded?

### Metadata Coverage

How much available symbol, relocation, debug, import/export, unwind, and related metadata can be recovered?

### Control-Flow Coverage

How much eligible control flow can be represented as recovered basic blocks and edges?

### Function Coverage

How many eligible function-like regions can be identified and analyzed?

### Semantic Coverage

How much higher-level behavior can be inferred from the available evidence?

### Source Coverage

How much source-like structure can be reconstructed?

### Runtime Coverage

How much behavior is known from runtime evidence rather than static artifact evidence?

Runtime coverage is normally **N/A** for a purely static analysis unless runtime evidence is explicitly supplied.

---

## Coverage Status Vocabulary

- **Measured** — calculated from an actual artifact or benchmark corpus.
- **Target** — an engineering objective.
- **Estimated** — a model-based approximation.
- **Observed** — directly supported by available evidence.
- **Derived** — calculated or inferred from observed evidence.
- **Unknown** — insufficient evidence for a meaningful conclusion.
- **Unavailable** — the needed evidence is absent from the supplied inputs.
- **Unsupported** — the analyzer does not currently implement the relevant format, architecture, feature, or operation.
- **Ambiguous** — multiple interpretations remain plausible.
- **Runtime-dependent** — the result depends on execution or environment state.
- **Transformed** — the original information exists only in a materially changed representation.
- **Partial** — some but not all eligible evidence or structures were recovered.

---

## Benchmark Corpus

Measured coverage should eventually be derived from a corpus containing:

- stripped and unstripped ELF;
- static and dynamic libraries;
- PE executables and DLLs;
- Mach-O executables and dynamic libraries;
- relocatable objects;
- multi-member archives;
- debug and non-debug builds;
- multiple optimization levels;
- LTO and non-LTO builds;
- C/C++ and other supported native-language outputs;
- PIE/PIC;
- static and dynamic executables;
- kernel modules;
- generated and hand-written assembly;
- multiple compiler families;
- multiple linker families;
- multiple architectures;
- malformed/truncated artifacts;
- unsupported inputs;
- ambiguous code/data boundaries.

The benchmark should record **format success separately from evidence success and reconstruction success**.

---

## Recommended Measurement Record

Each benchmark result should identify:

```
artifact
format
architecture
ABI
compiler
compiler version
compiler flags
linker
linker flags
optimization level
LTO status
debug-information status
stripped/unstripped status
input size
analysis version
analysis configuration
coverage denominator
coverage numerator
coverage percentage
unknown regions
unsupported regions
ambiguous regions
runtime dependencies
notes
```

This makes a percentage reproducible and prevents a number from being interpreted outside its intended scope.

---

## References

**Compiler/linker reference:** GNU GCC, [Link Options](https://gcc.gnu.org/onlinedocs/gcc/Link-Options.html), including static/shared libraries, `-static`, `-shared`, PIE, symbols, and relocations.

**GCC manual:** GNU Compiler Collection documentation for compilation and linking behavior.

**Slecompiler references:**

- `decompiler/API.md`
- `decompiler/TUTORIAL.md`
- `TERMINOLOGY.md`
- `COMPILER.md`
- `ARCHITECTURE.md`
- `build/PRODUCTS.md`
- `PERCENTAGE.COVERAGE.md`

---

## Governing Principles

1. **Evidence before inference.**
2. **Observed facts must be distinguishable from derived conclusions.**
3. **Every percentage must have a defined denominator.**
4. **Format recognition is not complete analysis.**
5. **Instruction decoding is not source reconstruction.**
6. **Static analysis is not runtime observation.**
7. **Static and dynamic/shared libraries expose different evidence models.**
8. **Missing evidence must remain missing rather than being silently invented.**
9. **Engineering targets must be distinguished from measured benchmark results.**
10. **Slecompiler coverage means coverage of evidence, not a promise to recreate the original source exactly.**

```
preserved evidence
+ structural recovery
+ control-flow recovery
+ data-flow analysis
+ semantic analysis
= greater decompilation coverage

greater decompilation coverage
!= original source equivalence
```
