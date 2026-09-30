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