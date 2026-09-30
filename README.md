<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa

## Editor's Note

**Source is now carefully Open.**

SLeeLa is developed as an inspectable software project. Source, interfaces, implementation boundaries, build methods, tests, and engineering documentation are maintained openly so that the work can be examined, built, tested, and improved with care.



## Java Parallel Execution and Native Procedural Interoperation

SLeeLa can now operate **in parallel with Java** as a language/runtime companion rather than requiring Java to replace or absorb the SLeeLa execution model.

A SLeeLa program can participate alongside Java execution while retaining its own native source representation. Java procedural logic can also be represented and executed from the **SLeeLa Family** through its corresponding **pre-compiled SLeeLa source**. This establishes a direct path between Java procedural behavior and the SLeeLa source/runtime layer:

`Java procedural → SLeeLa Family representation → pre-compiled SLeeLa source → SLeeLa execution`

This capability preserves the procedural role of the Java-side operation while giving the SLeeLa environment a native source-level form that its compiler, loader, SST, and Nordshrift tooling can understand.

### What This Means

- **Parallel with Java:** SLeeLa can run as a parallel language/runtime alongside Java.
- **Native SLeeLa source:** Java procedural operations can have a corresponding pre-compiled SLeeLa source representation.
- **Family interoperability:** Java procedural work can be brought into the SLeeLa Family without making the SLeeLa source layer merely a Java wrapper.
- **Compiler and Loader visibility:** The resulting SLeeLa source participates in the same source, package, symbol, compilation, and loader model described above.
- **Preserved execution boundaries:** Java remains available for Java-native operations, while SLeeLa provides its own native procedural execution path.

The architectural goal is a genuine **SLeeLa ↔ Java parallel relationship**: Java and SLeeLa can cooperate while SLeeLa source remains a first-class native representation and execution surface.

## Native Library File Count and Language / VM Architecture

The current native Java-facing library inventory contains **8,988 files under `/lib/java`**. This is the repository's substantial native source representation of SLeeLa's relationship to Java: it gives SLeeLa a source-level vocabulary for Java packages, classes, procedures, interfaces, runtime concepts, and interoperability boundaries while preserving SLeeLa as its own language rather than reducing it to a Java wrapper.

SLeeLa's language surface is designed around **procedural execution, structured packages, classes and symbols, library resolution, reflection, regular expressions, media and audio systems, networking, synchronization, virtual-machine facilities, and explicit runtime boundaries**. Its source model makes language constructs discoverable to the compiler, loader, SST, and Nordshrift, so a source definition participates in an executable and inspectable language system rather than serving only as documentation.

The Java relationship extends this model in both directions. SLeeLa can run **alongside Java**, Java procedural behavior can be represented through the **SLeeLa Family** as pre-compiled SLeeLa source, and that source representation can participate in normal SLeeLa package, symbol, compilation, and loading operations. The goal is interoperability at the language and procedural level while keeping each runtime's native responsibilities explicit.

### SLeeLa as a C/C++ Virtual Machine

SLeeLa also runs on a **C/C++ native virtual-machine foundation**. C and C++ provide the low-level execution substrate for the SLeeLa VM, including native runtime facilities, memory and process boundaries, platform integration, and the mechanisms required to load and execute SLeeLa procedural representations.

The architecture can therefore be understood as:

`SLeeLa source → compiler/loader → SLeeLa VM → C/C++ native runtime`

with Java interoperability operating in parallel:

`Java source/procedure ↔ SLeeLa Family representation ↔ pre-compiled SLeeLa source ↔ SLeeLa VM`

This does not make C, C++, Java, and SLeeLa interchangeable languages. Each layer has a defined responsibility: **SLeeLa supplies the language and procedural source model; the SLeeLa VM supplies execution; C/C++ supplies the native machine-facing foundation; and Java supplies a cooperating language/runtime surface that can be represented and invoked through the SLeeLa Family.**

### Core Language Characteristics

The current SLeeLa language architecture emphasizes:

- **Native procedural source** — procedures remain first-class SLeeLa source rather than opaque foreign calls.
- **Class and symbol organization** — language objects are organized into packages, classes, symbols, and module-facing definitions.
- **Package/library resolution** — source can be resolved through the standard library and its compiler-visible inventory.
- **Compilation and loading** — the compiler and loader form an explicit path from source to executable runtime artifacts.
- **VM execution** — SLeeLa programs execute through the SLeeLa VM and its C/C++ native foundation.
- **C and C++ integration** — native implementations can provide platform, performance, memory, device, and system facilities beneath the language layer.
- **Java parallel execution** — Java can remain active as a cooperating runtime while SLeeLa retains its independent source and execution model.
- **Java source representation** — Java procedural and language concepts can be represented in the SLeeLa Family through pre-compiled SLeeLa source.
- **Reflection and introspection** — the language/runtime surface includes facilities for examining symbols, classes, packages, and runtime structures.
- **Systems programming boundaries** — networking, synchronization, media, audio, process, and other system-facing packages can cross from SLeeLa into native implementations through defined interfaces.
- **Cross-platform runtime design** — the native foundation is intended to support the repository's C/C++ execution targets while keeping higher-level SLeeLa source portable.
- **Toolchain continuity** — SST and Nordshrift remain connected to the same source and symbol inventory used by compilation and loading.

The **8,988-file `/lib/java` collection** therefore represents more than a directory of Java-related files. It is a major part of the SLeeLa language bridge: a carefully organized native source surface through which SLeeLa can understand, represent, and work alongside Java while continuing to execute as its own language on a C/C++ virtual-machine foundation.

## SLeeLa Standard Library

The canonical SLeeLa-facing source collection is maintained under `/lib`. The library is the source-level package surface used by the SLeeLa compiler and loader rather than a documentation-only catalog.

The current development inventory records:

- **74 packages**
- **953 SLeeLa source units**
- **1,041 total symbol records**
- **88 module-facade symbols**

The library includes the current language/runtime foundations together with package families such as regex, video, VM, reflection, audio, networking, synchronization, media, and other repository subsystems. Package-specific C/C++/Java implementations remain native/backend layers where appropriate; the `/lib` sources provide the corresponding SLeeLa language objects and package-facing contracts.

### Compiler and Loader

The compiler and loader treat `/lib` as an explicit library-resolution surface. New SLeeLa library sources are expected to participate in:

`source → package/library resolution → semantic analysis → compilation → artifact/loader resolution`

Library inventory and symbol-resolution information is kept synchronized with the compiler compatibility gate so that newly added SLeeLa classes and package facades are visible to tooling rather than remaining isolated source files.

### SST and Nordshrift

SST and Nordshrift use the repository's library inventory as part of their compiler-facing symbol and package surface. The canonical collection is intended to provide:

- package-to-source resolution;
- symbol-to-source resolution;
- module-facade discovery;
- compiler compatibility checks;
- loader visibility checks; and
- a reproducible inventory of the SLeeLa standard-library surface.

This keeps the SLeeLa source layer, compiler, loader, SST, and Nordshrift representations aligned as the library grows.

## SLeeLa Regular Expression System — `/regex`

The SLeeLa regular-expression subsystem is treated as a **first-class language and library capability**, not as an incidental helper. The `/regex` surface provides the source-level vocabulary for defining, compiling, validating, matching, searching, extracting, and transforming text with regular-expression patterns while keeping those operations visible to the SLeeLa Compiler, Loader, SST, Nordshrift, and native VM architecture.

### Purpose

Regular expressions give SLeeLa a formal pattern language for text processing. The subsystem is intended to support the complete procedural lifecycle:

`pattern source → regex definition → compilation/validation → matching/search → capture/extraction → replacement/transformation`

A regex therefore has two related identities:

- **Pattern identity** — the expression and its syntax.
- **Procedural identity** — the compiled or executable operation that SLeeLa code can invoke.

This distinction allows SLeeLa source to describe a pattern independently from the runtime mechanism that executes it.

### Core Regex Language Features

The SLeeLa regex surface is organized around the conventional building blocks of regular-expression languages:

- **Literal characters** — direct character and text matching.
- **Character classes** — sets and ranges of characters.
- **Character-class negation** — matching characters outside a defined set.
- **Wildcards** — controlled matching of arbitrary characters.
- **Anchors** — beginning/end and other positional assertions.
- **Quantifiers** — repetition of expressions.
- **Grouping** — logical and procedural grouping of pattern expressions.
- **Alternation** — selecting among multiple pattern branches.
- **Escaping** — representing metacharacters and special characters literally.
- **Captures** — retaining matched portions of input for procedural use.
- **Assertions** — expressing conditions about the surrounding input without necessarily consuming it.
- **Flags/modes** — controlling matching behavior where the selected implementation provides those modes.

The exact runtime behavior of a feature is determined by the SLeeLa regex implementation and its documented compatibility surface. A source definition should never imply support for a regex feature that its backend has not implemented and tested.

### Regex as SLeeLa Source

The important architectural point is that regex operations remain **SLeeLa-visible source objects**.

A regex definition can participate in the normal SLeeLa language path:

`SLeeLa regex source → package/library resolution → semantic analysis → regex compilation → SLeeLa VM execution`

This keeps regular expressions connected to:

- SLeeLa classes and symbols;
- package and library resolution;
- compiler and loader discovery;
- procedural operations;
- reflection and introspection;
- native C/C++ runtime facilities where required; and
- the broader SST and Nordshrift symbol inventory.

Regex functionality therefore belongs to the language's executable source model rather than existing only as a foreign-library call.

### Matching Operations

The regex subsystem is designed to distinguish common procedural operations so callers can express intent clearly:

1. **Validate** — determine whether a pattern is syntactically valid before execution.
2. **Compile** — prepare a pattern for repeated execution.
3. **Match** — determine whether input satisfies a pattern.
4. **Search** — locate a matching region within larger input.
5. **Capture** — retrieve portions identified by groups.
6. **Replace** — transform matching regions into replacement text.
7. **Iterate** — process multiple matches where the selected regex implementation supports repeated matching.
8. **Inspect** — expose pattern and match information to SLeeLa runtime/reflection facilities.

The intended separation between validation, compilation, and execution is important for long-running programs: a pattern can be checked once and then reused rather than repeatedly interpreted as untrusted or unvalidated source.

### Text and Symbol Integration

Regex is particularly useful because SLeeLa treats text, symbols, packages, and procedural definitions as inspectable language objects. Regex operations can therefore serve as a bridge between ordinary text and structured SLeeLa processing.

Typical language-level uses include:

- source-text inspection;
- lexical filtering;
- input validation;
- token discovery;
- symbol/name matching;
- configuration parsing;
- structured text extraction;
- search-and-replace operations;
- compiler and loader support;
- documentation and source analysis; and
- protocol and network message processing.

Regex does not replace a parser. Where SLeeLa requires grammatical or structural understanding, the regex subsystem should be used for lexical/pattern-level work and the appropriate parser or semantic subsystem should perform structural analysis.

### Native Runtime Boundary

The regex API remains independent from its low-level execution mechanism. Where native acceleration or operating-system integration is appropriate, C and C++ can provide the implementation beneath the SLeeLa-facing API:

`SLeeLa regex source → Compiler/Loader → Regex runtime interface → C/C++ implementation → SLeeLa VM`

This preserves the same architectural separation used elsewhere in SLeeLa:

- SLeeLa defines the language-facing operation.
- The Compiler and Loader resolve the source and symbols.
- The SLeeLa VM executes the procedural operation.
- C/C++ may provide the native implementation and platform/runtime services.

A Java implementation may also participate through the established Java parallel-runtime model when a Java-side regex facility is intentionally selected and documented.

### Safety and Correctness

Regex processing can become computationally expensive when patterns and inputs interact badly. SLeeLa's regex subsystem should therefore treat pattern validation, execution limits, input boundaries, and backend behavior as engineering concerns rather than assuming that every syntactically valid expression is computationally harmless.

Implementations should document:

- supported syntax;
- unsupported syntax;
- escaping rules;
- character encoding behavior;
- Unicode behavior;
- capture semantics;
- replacement semantics;
- execution/resource limits;
- error reporting; and
- backend-specific compatibility.

This allows the SLeeLa source definition to remain portable while making implementation-specific behavior explicit.

### Compiler, Loader, SST, and Nordshrift

Regex source belongs to the same discoverable library model as the rest of SLeeLa. New regex classes, symbols, or procedural definitions should be added to the canonical library inventory and made visible to the compiler and loader.

The intended chain is:

`/regex source → symbol inventory → SST/Nordshrift → compiler resolution → loader resolution → VM execution`

This is especially important for regex because a pattern may be represented both as source syntax and as a runtime-compiled object. Both identities need stable symbols and predictable loading behavior.

### Relationship to the SLeeLa Language

Regex is one of the language capabilities that demonstrates the intended SLeeLa model: a high-level procedural feature can have a clear source representation, compiler-visible symbols, a VM execution path, and a native C/C++ foundation without losing its identity as a SLeeLa operation.

In that model:

`Regex source`
→ `SLeeLa symbol`
→ `Compiler/Loader`
→ `SLeeLa VM`
→ `native regex/runtime service`

and, where Java interoperability is intentionally used:

`Java regex facility ↔ SLeeLa Family representation ↔ pre-compiled SLeeLa source`

The result is a regex subsystem that belongs to SLeeLa itself while remaining capable of using carefully defined native or Java runtime services underneath the language boundary.


## SLeeLa Decompiler and Native Analysis — `/decompiler`

SLeeLa includes a dedicated **Slecompiler™** subsystem under `/decompiler` for native-binary analysis, decompilation, library inspection, driver investigation, and evidence-based source reconstruction. The subsystem is designed as a read-only static-analysis pipeline: it analyzes native artifacts without treating the analyzed artifact as executable input.

The decompiler covers native artifact families including ELF executables and shared objects, PE/COFF programs and libraries, Mach-O artifacts where implemented, static archives, relocatable objects, Linux kernel modules as static evidence, firmware/raw artifacts, and related library families.

### Analysis Architecture

The decompiler follows a defined architecture:

`Artifact → Container Reader → Architecture Decoder → Instruction Stream → Symbols/Imports/Exports/Relocations → Basic Blocks → CFG → Function Candidates → SLIR → Analysis → Reports/API/Refactoring`

The central intermediate representation is **SLIR — SLeeLa Intermediate Representation**. SLIR provides an architecture-neutral layer between native decoding and higher-level analysis/reconstruction.

The decompiler's documented design rules emphasize:

- deterministic, side-effect-free parsing;
- preservation and provenance of original bytes;
- explicit evidence attached to inferred objects;
- separation of architecture-specific decoding from SLIR;
- stable public API boundaries;
- conservative representation of unsupported instructions;
- distinction between observed evidence and inferred hypotheses; and
- archival, reproducible analysis output.

### C/C++ and SLeeLa Reconstruction

The decompiler provides a shared C/C++ semantic class model for native reconstruction, including types, fields, variables, function signatures, calling conventions, methods, C structures/unions/enums, C++ classes/namespaces, artifacts, addresses, instructions, basic blocks, control-flow graphs, reconstruction units, provenance, and evidence classifications.

Its documented source-emission targets include:

- **Java** — JVM-oriented source reconstruction;
- **SLeeLa** — native SLeeLa source representation;
- **C** — procedural C reconstruction;
- **C++** — C++20-oriented reconstruction.

The important relationship to the rest of SLeeLa is:

`Native artifact → analysis → SLIR → reconstruction model → SLeeLa/Java/C/C++ source representation`

Generated source is evidence-derived. The decompiler explicitly preserves uncertainty rather than presenting recovered code as original source when the binary evidence does not establish that equivalence.

### Decompiler VM Boundary

The SLeeLa VM used by the decompiler is an **analysis and validation environment for SLIR**, not an execution path for untrusted native binaries. Its purpose is deterministic testing of lifting and transformation passes using controlled representations.

This maintains a clear boundary:

`Native binary → static analysis → SLIR → controlled VM validation`

rather than:

`Native binary → execution`

Driver and kernel-module analysis is likewise static. The decompiler does not load kernel modules, open devices, write target memory, or invoke recovered native code as part of normal analysis.

### Documentation and Tooling

The `/decompiler` subsystem contains API, architecture, class-model, definitions, terminology, tutorial, CMake, CLI, examples, source, include, documentation, and test areas. The principal documentation includes `decompiler/README.md`, `API.md`, `API_CLASS_MODEL.md`, `ARCHITECTURE.md`, `DEFINITIONS.md`, `TERMINOLOGY.md`, and `TUTORIAL.md`.

The API documentation also describes source-selection for decompilation through Java, SLeeLa, C, and C++ output targets. This makes the decompiler an important bridge between compiled/native artifacts and the SLeeLa source ecosystem.

See the complete [`/decompiler` subsystem](https://github.com/mearvk/SLeeLa/tree/master/decompiler) and its documentation for implementation details, API contracts, analysis limitations, and build guidance.

## Audio and Codec Architecture

The Audio work is organized by implementation language and responsibility:

- `audio/c/` — C11 implementation and library interface.
- `audio/cpp/` — C++17 implementation and typed API.
- `audio/java/` — Java 21 orchestration and native-process boundary.
- `audio/native/` — native media-processing implementation.
- `audio/gui/` — JavaFX presentation and integration.
- `codecs/` — codec standards registry, handler API, capability metadata, and codec conformance work.

### Major Sound Standards

The new `/codecs` registry provides explicit coverage for major audio standards and formats:

- PCM/WAV
- AIFF
- FLAC
- ALAC
- MP3
- AAC
- HE-AAC
- Vorbis
- Opus
- Speex
- WMA
- AC-3
- E-AC-3
- AMR-NB
- AMR-WB
- G.711 μ-law
- G.711 A-law
- MIDI
- Matroska Audio
- WebM Audio

The codec registry distinguishes **Native**, **Backend**, **Recognized**, **Container**, and **Event** capabilities. Listing a codec does not by itself claim that an encoder or decoder is already implemented.

The intended audio path is:

`codec/container → handler → PCM boundary → Audio API`

and, for encoding:

`Audio PCM → handler → codec/container output`

The codec layer remains separate from the Audio mixer so that validation, decoding, encoding, and media processing have clear boundaries.

See `codecs/README.md` and `codecs/CODECS.md` for the detailed registry, capability definitions, implementation order, security requirements, and licensing considerations.

The phrase **carefully Open** is intentional: openness includes clear interfaces, explicit implementation boundaries, reproducible builds, validation, tests, and documentation of unfinished areas. It does not imply that an implementation is complete merely because its source is visible.

— Editor's Note, SLeeLa