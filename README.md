<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-logo-004.jpg" alt="SLeeLa">




# SLeeLa

## <img src="https://github.com/mearvk/SLeeLa/raw/master/images/debian-logo.png" width="25" height="25" alt="Debian"> I. Audio and Codec Architecture

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

## <img src="https://github.com/mearvk/SLeeLa/raw/master/images/debian-logo.png" width="25" height="25" alt="Debian"> II. Editor's Note

**Source is now carefully Open.**

SLeeLa is developed as an inspectable software project. Source, interfaces, implementation boundaries, build methods, tests, and engineering documentation are maintained openly so that the work can be examined, built, tested, and improved with care.



## <img src="https://github.com/mearvk/SLeeLa/raw/master/images/debian-logo.png" width="25" height="25" alt="Debian"> III. Java Parallel Execution and Native Procedural Interoperation

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

## <img src="https://github.com/mearvk/SLeeLa/raw/master/images/debian-logo.png" width="25" height="25" alt="Debian"> IV. Native Library File Count and Language / VM Architecture

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

## <img src="https://github.com/mearvk/SLeeLa/raw/master/images/debian-logo.png" width="25" height="25" alt="Debian"> V. SLeeLa Decompiler and Native Analysis — `/decompiler`

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


## <img src="https://github.com/mearvk/SLeeLa/raw/master/images/debian-logo.png" width="25" height="25" alt="Debian"> VI. SLeeLa HTTP 1.0–9.0 — HTTP and Protocol Details

The SLeeLa repository maintains nine experimental HTTP-generation directories, from `/http-1.0` through `/http-9.0`. These are **SLeeLa application-protocol generations**, not claims that HTTP/4 through HTTP/9 are published IETF HTTP standards. The generation directories define the SLeeLa application envelope, routing identifiers, protocol state, metadata, negotiation, integrity, and application behavior carried by an appropriate transport.

### Common SLeeLa HTTP Architecture

Across the generations, SLeeLa keeps **native transport addressing** separate from **SLeeLa application addressing**. A logical SLeeLa PORT is an application identifier and does not replace or necessarily map one-to-one with a native TCP/UDP socket.

The general relationship is:

```text
Native transport
      |
HTTP carrier / connection / stream
      |
SLeeLa generation
      |
Logical PORT
      |
SERVICE-ID / OP-ID
      |
REQUEST-ID / protocol state
      |
Application payload
```

The shared large-file transfer contract applies to files larger than 50 MB:

```text
SESSION-ID | DATETIME | FILE-ID | FILE-NAME | INDEX | OFFSET | TOTAL-SIZE
```

The 50 MB threshold selects the resume-oriented mode; it is not a maximum file size.

### HTTP 1.0 — Baseline Application Routing

**Directory:** `/http-1.0`

HTTP 1.0 establishes the baseline SLeeLa application model. It separates transport addressing from SLeeLa logical routing and places `SERVICE-ID` and `OP-ID` above the logical PORT.

**Protocol details:**

- HTTP/1.0 request and connection boundaries carry SLeeLa application exchanges.
- Logical PORT identifies an application service boundary.
- `SERVICE-ID` identifies the service.
- `OP-ID` identifies the requested application operation.
- SLeeLa does not treat HTTP/1.0 as providing HTTP/2-style stream multiplexing.
- Concurrent application work is represented as logical operations above the HTTP/1.0 connection/request boundary.
- TCP sockets, TLS termination, proxies, routers, and native port bindings remain transport concerns.
- The shared DOWNLOAD/resume metadata supports interrupted transfers.

```text
Native transport endpoint
        |
HTTP/1.0 request / connection
        |
SLeeLa logical PORT
        |
SERVICE-ID / OP-ID
        |
Application operation
```

### HTTP 2.0 / 2.1 — Correlated Concurrent Exchanges

**Directory:** `/http-2.0`

HTTP 2.0 / 2.1 extends the baseline with compact application envelopes, request correlation, retry classes, and logical-port routing over concurrent HTTP/2 exchanges.

**Protocol details:**

- Compact application envelopes.
- `SERVICE-ID` and `OP-ID`.
- `REQUEST-ID` for request correlation.
- Retry classification.
- Logical-port routing independent of native socket numbering.
- Stream-aware concurrent exchanges.
- Shared DOWNLOAD/resume support.
- Explicit generation negotiation.
- Where fallback is permitted, negotiation may return to HTTP/1.1 and then HTTP/1.0.
- A fallback must not silently remove a security property required by deployment policy.

```text
Native transport endpoint
        |
HTTP/2 stream
        |
SLeeLa logical PORT
        |
SERVICE-ID / OP-ID
        |
REQUEST-ID
        |
Application request
```

### HTTP 3.0 — QUIC/HTTP/3 Carrier and Application Integrity

**Directory:** `/http-3.0`

HTTP 3.0 extends the SLeeLa application model with request correlation, service/operation naming, retry classes, processing stages, and an application integrity/security layer. HTTP/3 and QUIC may provide the carrier.

**Protocol details:**

- QUIC connection may provide the authenticated/encrypted transport carrier.
- HTTP/3 streams carry SLeeLa application exchanges.
- Logical PORT remains an application identifier.
- `SERVICE-ID`, `OP-ID`, and `REQUEST-ID` identify application work.
- The SLeeLa application envelope remains distinct from carrier security.
- Application integrity does not replace TLS, QUIC security, or deployment authorization.
- The shared large-file resume contract remains available.
- Implementation and self-test material are maintained with the generation.

```text
QUIC connection
      |
HTTP/3 stream
      |
SLeeLa logical PORT
      |
SERVICE-ID / OP-ID
      |
REQUEST-ID
      |
SLeeLa application envelope
```

### HTTP 4.0 — Message, Frame, Session, and Resume Protocol

**Directory:** `/http-4.0`

HTTP 4.0 is an experimental SLeeLa protocol layer rather than an IETF HTTP/4 standard. It can initially be carried as application data over HTTP/3 or another supported carrier.

**Protocol details:**

- Explicit stream and request identity.
- Authenticated sequence handling.
- Resumable delivery.
- Flow control and backpressure.
- Incremental delivery.
- Capability negotiation.
- Session migration.
- Typed reset and retry behavior.
- Observability.
- Pluggable carrier abstraction.

The initial frame representation is:

```text
VERSION | TYPE | FLAGS | STREAM-ID | REQUEST-ID | SEQUENCE | PAYLOAD-LENGTH | PAYLOAD
```

Defined frame types:

- `OPEN` — establish a logical request or stream.
- `DATA` — carry application payload.
- `END` — complete a request or stream.
- `RESET` — terminate a stream.
- `WINDOW` — advertise receive capacity.
- `PING` / `PONG` — liveness.
- `RESUME` — continue an interrupted transfer.
- `CAPSULE` — carry negotiated or session metadata.

The protocol separates application framing from authenticated transport. HTTP 4.0 does not replace TLS or create a substitute for authenticated transport.

### HTTP 5.0 — Friends' Packs Application Capability

**Directory:** `/http-5.0`

HTTP 5.0 extends the HTTP 4.0 frame/session model with the application-level **Friends' Packs** capability. A Friends' Pack is ordinary user-controlled application data.

**Protocol details:**

- Friends' list entries may contain a friend name, optional point amount, and optional document reference.
- Initial friend payload form:

```text
friend-name|points|document-reference
```

- A pack may contain an opaque identifier, relationship identifier, FP balance, optional bonus-offer references, optional expiration, and negotiated application policy.
- `FRIENDS_PACK` carries pack metadata.
- `BONUS_OFFER` carries an optional offering reference.
- `FP_UPDATE` carries an application-level balance update.
- `AUDIT` carries a defensive conformance/audit event.
- FP is application accounting data, not an authentication credential or authorization to control another system.
- When FP reaches zero, ordinary HTTP 5.0 operation continues; optional pack bonuses are declined without authorizing interference with unrelated traffic.
- Defensive audit kits are limited to authorized conformance and testing.

HTTP 5.0 retains the HTTP 4.0 frame/session, capability, flow-control, integrity, and authenticated-carrier architecture.

### HTTP 6.0 — Consolidated Friends' Bet and Teamster Debate

**Directory:** `/http-6.0`

HTTP 6.0 consolidates HTTP 5.0 application capabilities into a portable **Consolidated Friends' Bet** record. It is ordinary application data carried through an authorized HTTP exchange.

**Protocol details:**

The record may contain:

- friends list;
- optional point values;
- assigned document references;
- team/area label;
- user-supplied consolidated IQ value;
- debate topic;
- user-authored position or argument;
- optional recipient label.

The generation adds the application fields:

- `CONSOLIDATED_FRIENDS_BET`
- `TEAMSTER_DEBATE`
- `CONSOLIDATE_IQ`
- `TEAM_AREA`
- `DEBATE_TOPIC`
- `DEBATE_POSITION`
- `RECIPIENT_LABEL`

A Teamster Debate packet can be formed by identifying the team area, supplying the user-provided IQ value as application data, supplying a debate topic and authored position, identifying the intended recipient/audience, and serializing the package into an HTTP 6.0 application packet.

The record is non-binding application data. It does not authorize interference with networks, systems, persons, property, or communications.

### HTTP 7.0 — Reality Assertion and Provenance Layer

**Directory:** `/http-7.0`

HTTP 7.0 introduces a **Reality Assertion** layer for carrying statements as explicitly classified application claims. Transmission of a statement is kept separate from the question of whether evidence establishes that statement.

**Protocol details:**

Each assertion may contain:

- `ASSERTION_TYPE`
- `STATEMENT`
- `STATUS`
- `SOURCE`
- `SOURCE_DATE`
- `AUTHOR`
- optional `DOCUMENT_REFERENCE`

Suggested status values include:

- `FACTUAL_DOCUMENTED`
- `USER_AUTHORED`
- `FICTIONAL`
- `COUNTERFACTUAL`
- `DISPUTED`
- `UNVERIFIED`

The protocol therefore carries **claims plus provenance/classification**, not an automatic declaration that the claim is true. Applications should preserve author, source, source date, and classification information.

Reality Assertion data does not grant authority, establish legal status, change physical reality, or authorize action against people or systems.

### HTTP 8.0 — Cryptographic Session Boundary and Structured Metadata

**Directory:** `/http-8.0`

HTTP 8.0 adds an explicit cryptographic and session-security boundary. Cryptographic configuration is separated from ordinary application payloads, and implementations are expected to fail closed when required security policy cannot be satisfied.

**Protocol details:**

- Explicit generation acceptance before selection.
- Configurable cryptographic support.
- Early security configuration.
- C/C++ cryptographic interface.
- HTTP 8.0 application metadata remains distinct from transport authentication/encryption.
- Fallback is permitted only where policy allows it and must not silently weaken a required security property.
- Logical PORT remains an application identifier.
- Large-file resume uses the common SLeeLa contract.

HTTP 8.0 also defines an application-level **National Emblems, Signals, and Frequency** data area for descriptive, interoperable metadata. Records may include emblem identifiers/provenance, authorized signal identifiers and descriptions, frequency units/bands/ranges/measurement context, jurisdiction or service context when explicitly supplied, timestamps, versioning, and source references.

Frequency and signal information is descriptive and bounded by authorization policy. HTTP 8.0 does not define instructions for unauthorized interception, interference, jamming, evasion, or disruption.

The project-level **DarkPower** C++ model provides a typed session descriptor, including the fixed SCHEDULE TERM field, the documented DARK POWER value, and a contact-request string helper. These are application data-format constructs; they do not authenticate peers, authorize access, bypass controls, or establish a network connection.

### HTTP 9.0 — International Security, Safety, Police, and Dark Band Metadata

**Directory:** `/http-9.0`

HTTP 9.0 extends the HTTP 8.0 packet model. It retains the prior packet metadata and adds structured **International Data for Security, Safety, and Police** together with the **Dark Band** metadata field.

**Protocol details:**

HTTP 9.0 retains:

- protocol grade;
- prior packet metadata;
- identity and international identifiers;
- monitoring/frequency metadata;
- National Emblems, Signals, and Frequency metadata;
- sequence number.

It additionally carries:

- Dark Band name and original-content hexadecimal value;
- international security, safety, and police metadata.

The default Dark Band original-content representation is:

```text
0x4441524B42414E44
```

The repository exposes this through `HTTP90_DARK_BAND_ORIGINAL_CONTENT_HEX` and the `http90_dark_band` structure. It is application metadata, not a cryptographic key, credential, authorization token, radio-control instruction, or hidden operational channel.

The International Data model is provenance-oriented. Applications should identify the responsible jurisdiction or organization, preserve source and timestamp information, and distinguish descriptive records from claims of authority. The police field is a data category, not a command channel.

### Generation Negotiation

For generations that implement explicit negotiation, a peer must accept the proposed generation before that generation is selected. Where fallback is supported, the documented fallback path may return to HTTP/1.1 and then HTTP/1.0.

The general rule is:

```text
propose generation
       |
peer acceptance
       |
select generation
       |
apply generation protocol
       |
fallback only when permitted by policy
```

A fallback must not silently weaken a security property required by the configured deployment policy.

### Protocol Boundary and Security Model

The SLeeLa HTTP generations distinguish four concerns:

1. **Transport** — TCP, TLS, QUIC, HTTP/2, HTTP/3, native sockets, proxies, and related carrier mechanisms.
2. **SLeeLa protocol** — generation, framing, request/stream identity, sequencing, negotiation, retry, resume, and validation.
3. **Application metadata** — service records, Friends' Packs, assertions, national metadata, security/safety/police records, Dark Band, and related application fields.
4. **Authorization** — actual identity, permissions, legal status, access control, and deployment policy.

A field in a SLeeLa packet does not by itself grant authority, authenticate an actor, establish jurisdiction, provide security clearance, or permit access to a protected system.

### Implementation and Build References

Each generation keeps its own implementation boundary and README:

- `/http-1.0/README.md`
- `/http-2.0/README.md`
- `/http-3.0/README.md`
- `/http-4.0/README.md`
- `/http-5.0/README.md`
- `/http-6.0/README.md`
- `/http-7.0/README.md`
- `/http-8.0/README.md`
- `/http-9.0/README.md`

The root README is the architectural index; the generation READMEs remain the detailed implementation references. Build and conformance work should use the generation's own source tree and documented tests rather than treating the root summary as a substitute for implementation evidence.
## <img src="https://github.com/mearvk/SLeeLa/raw/master/images/debian-logo.png" width="25" height="25" alt="Debian"> VII. SLeeLa Regular Expression System — `/regex`

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


## <img src="https://github.com/mearvk/SLeeLa/raw/master/images/debian-logo.png" width="25" height="25" alt="Debian"> VIII. SLeeLa Server Edition — `/server-edition`

SLeeLa includes a dedicated **Server Edition** under `/server-edition`. It is the server-side network service boundary for the SLeeLa HTTP generation family, with explicit packet admission, routing, filtering, audit, annotation, and application-service boundaries.

### Server Architecture

The Server Edition provides a bounded transport and admission layer:

`accept → fixed envelope → header parser → generation/routing policy → filters and heuristics → admission boundary → application service handler`

The implementation is intentionally separated from application payload execution. Payloads are treated as bytes; the server does not execute programs, shell commands, scripts, XML procedures, or metadata.

The current Server Edition documentation covers:

- HTTP generations 1.0 through 9.0 through explicit generation adapters.
- Fixed wire-envelope and packet-processing rules.
- Annotation-language integration through the SLeeLa front end.
- Holding Document → Forwarding Annotation → Nexter Colony forwarding vocabulary.
- Port-awareness and host-firewall lifecycle boundaries.
- Logging, heuristics, malformed-input rejection, bounded memory/frame sizes, timeouts, and deterministic shutdown.
- Native C++17 server implementation for Linux/macOS, with a documented Winsock2 build path for Windows.

### International Sternary

The Server Edition also contains the **International Sternary** subsystem under `server-edition/international-strernary` (the repository currently uses the `strernary` spelling in the directory name).

International Sternary is the repository's designated international administrative-services area. In the current project terminology, it is associated with the **Processor of Internal Affairs of the State**. This README documents that role as a project-defined subsystem/function, not as a claim about an external governmental institution or real-world authority.

The directory is currently present as an explicit Server Edition boundary and can be expanded with its source, protocol, state, and administrative-service definitions as those components are implemented.

### Relationship to SLeeLa

The Server Edition connects the SLeeLa language/runtime to network service operation without turning network metadata into authority:

`SLeeLa source → compiler/loader → runtime → Server Edition → validated transport/application boundary`

Its documented classification, security, police, safety, frequency, Dark Band, and related fields remain application metadata. They do not by themselves grant identity, clearance, authority, network control, or access.

See the complete [`/server-edition` subsystem](https://github.com/mearvk/SLeeLa/tree/master/server-edition) and its subsystem README for the current implementation and protocol documentation.

## <img src="https://github.com/mearvk/SLeeLa/raw/master/images/debian-logo.png" width="25" height="25" alt="Debian"> IX. SLeeLa Standard Library

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

## <img src="https://github.com/mearvk/SLeeLa/raw/master/images/debian-logo.png" width="25" height="25" alt="Debian"> X. SLeeLa Telephony — `/telephony-skya`

SLeeLa includes a dedicated **Skya™ telephony subsystem** under `/telephony-skya`. Skya defines the SLeeLa application boundary for client, server, and combined telephony operation, with native C/C++ services beneath SLeeLa-level runnable programs and a JavaFX/Guia™ presentation layer.

### Skya Architecture

The subsystem separates responsibilities across its repository layout:

- `native/` — authoritative C/C++ engine boundary and native build files for networking, media, security, NAT, and file-transfer services.
- `sleela/` — runnable `.sleela` wrapper programs for server, client, and combined room operation.
- `javafx/` — JavaFX client presentation and integration.
- `drivers/` — device and telephony-driver integration boundary.
- `docs/` — protocol, runtime, security, media, NAT, GUI, and runnable-program documentation.
- `config/` — deployment and runtime defaults.
- `build/` — build/output boundary.

The native layer remains authoritative for low-level networking, media, security, NAT, and file transfer. The SLeeLa programs provide application-level runnables and orchestration without creating a second native runtime.

### User and Administrative Clients

The default Skya GUI is the non-administrative user client, `SkyaClientApp`. Its primary user-facing capabilities include:

- Chat
- Video
- Audio
- File Transfer
- connection and room selection

Administrative lifecycle and local circuit monitoring are separated into `SkyaApp`, launched through the Client Monitor. This keeps ordinary user operation distinct from administrative controls.

### Network, Security, and Media Boundary

Skya is designed around HTTP/2 and HTTP/3-capable networking, NAT/relay awareness, certificate verification, RSA-2048 compatibility, ephemeral Diffie-Hellman, resumable file-transfer contracts, and codec negotiation. Codec names describe adapter capabilities; deployment must provide the corresponding libraries and comply with applicable licensing.

The intended native/application relationship is:

`Skya client/server → SLeeLa runnable layer → native C/C++ telephony engine`

This keeps the language-level application surface inspectable while preserving native implementations for platform-sensitive services.

### Guia™ and BODI GUI Protocol

The Skya JavaFX client uses **Guia™ 1.0** as its GUI-to-SLeeLa client/listener protocol. **BODI** supplies declarative UI definitions while Guia™ provides runtime lifecycle, events, commands, data, monitoring, and listener transitions.

This places the Skya GUI inside the broader SLeeLa Java parallel-runtime architecture rather than making JavaFX the telephony engine itself.

### Build and Execution

The documented native build boundary is:

`make -C telephony-skya/native`

with the native executable serving as the bridge for combined operation, including HTTP/3 and room selection where those options are enabled. The `.sleela` programs are compiled and run through the normal SLeeLa toolchain and use SLeeLa socket/thread primitives where a pure-SLeeLa runnable is appropriate.

### Telephony Drivers

Skya reserves `/drivers` for device and telephony hardware integration. This provides a defined place for headset, USB, audio, video, and other supported-device drivers while keeping device-specific implementation beneath the higher-level Skya room/client/server model.

### Relationship to SLeeLa

The Skya subsystem demonstrates the intended SLeeLa architecture across several layers:

`SLeeLa source → compiler/loader → SLeeLa VM → Skya runnable → native C/C++ telephony services`

and for the JavaFX presentation path:

`Skya JavaFX → Guia™/BODI → SLeeLa client/runtime → native telephony services`

Skya therefore serves as a concrete cross-platform application boundary where SLeeLa source, the C/C++ VM foundation, native services, Java interoperability, GUI protocols, and device drivers can operate as defined layers rather than as one undifferentiated runtime.

See the complete [`/telephony-skya` subsystem](https://github.com/mearvk/SLeeLa/tree/master/telephony-skya) and its subsystem README for the current implementation layout and protocol documentation.

## <img src="https://github.com/mearvk/SLeeLa/raw/master/images/debian-logo.png" width="25" height="25" alt="Debian"> XI. SLeeLa Terminal — `/terminal`

SLeeLa maintains its own terminal development surface under `/terminal`. This directory is a **regular tracked directory in the SLeeLa repository**, not a Git submodule. It is reserved for SLeeLa-specific terminal features, extensions, integrations, and derived work associated with the repository's terminal environment.

### Terminal Architecture and Source Ownership

The terminal layout deliberately separates SLeeLa development from the pristine vendored GNU Bash source tree:

- `bash/` — the vendored upstream GNU Bash source tree. It should remain close to upstream and should not be edited directly for ordinary SLeeLa development.
- `terminal/` — SLeeLa-specific, new, or derived terminal work. Changes intended to become part of SLeeLa's terminal behavior belong here.

This boundary gives SLeeLa a clean ownership model: upstream Bash remains identifiable as upstream source, while SLeeLa's terminal language/runtime work remains reviewable as native repository code.

### Terminal as a SLeeLa Language and Runtime Surface

The terminal is an important systems-facing part of SLeeLa because command-line interaction connects the language to processes, input/output streams, environment state, command execution, scripting, diagnostics, build tooling, and interactive development. The intended architecture keeps those facilities explicit rather than hiding them inside an opaque external dependency.

The terminal relationship can be represented as:

`SLeeLa terminal source → compiler/loader → SLeeLa runtime → native process/terminal services`

Where Bash compatibility or upstream shell behavior is required, the vendored Bash tree provides the reference implementation boundary while `/terminal` provides the place for SLeeLa-specific extensions and integration.

### Repository History and Development Convention

The `/terminal` directory was previously configured as a Git submodule pointing toward the upstream GNU Bash repository. It has been converted into a normal tracked directory so that SLeeLa terminal development is represented directly in the SLeeLa repository rather than as an upstream submodule reference.

This convention improves source ownership, reviewability, reproducibility, and integration with the SLeeLa compiler, loader, native C/C++ VM foundation, build system, and CI. It also prevents SLeeLa-specific terminal work from being confused with modifications to upstream Bash.

The terminal subsystem currently contains its own `README.md` and tracking placeholder, with the directory intentionally prepared for continued SLeeLa-specific terminal development.

See the complete [`/terminal` subsystem](https://github.com/mearvk/SLeeLa/tree/master/terminal) for its current source-ownership convention and development notes.