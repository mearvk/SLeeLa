# SLeeLa — EDITION

**SLeeLa Language, Runtime, Compiler, APIs, Methods, Protocols, and Application Edition**  
**MEARVK LLC — Max Rupplin — 2026**

## 1. Purpose

This is the consolidated feature map for SLeeLa. It records the language,
runtime, compiler, semantic model, system APIs, subject libraries, networking,
servers, protocols, telephony/driver architecture, security, tooling, and the
principal branches of computation and methods represented by the repository.

This document is a capability map. It does not imply that every listed
facility has identical production maturity. Implemented, documented,
integrated, experimental, architectural, and historical work remain distinct.

## 2. SLeeLa System

SLeeLa is a Java-like programming language running on a Turing-complete C/C++
execution core, with Nordshrift providing a structured semantic/transpilation
layer and the SHEET catalog providing shared object definitions.

The authoritative current implementation is under impl/. The older Java
implementation under src/implementations/_001_/ is retained as project
history/parallel work.

Core lifecycle:

SOURCE → VALIDATE → COMPILE → EXECUTE → INTEGRATE → PRODUCE → VERIFY

Design axes:

START → MIDDLE → END  
NOVICE → INTERMEDIATE → ADVANCE

## 3. Language Features

The documented language model includes:

- classes, fields, methods, values, expressions, variables, bindings;
- control flow and function/method calls;
- access modifiers and static/protected semantics;
- structured data and C/C++-style struct support;
- new instantiation and member access;
- reference semantics;
- struct packing/unpacking;
- syntax-version declarations;
- managed memory behavior;
- concurrency constructs;
- contracts, flows, effects, contexts, lenses, rules, and projections;
- explicit Maybe values rather than implicit null;
- denotational semantics;
- Hindley-Milner-style type inference;
- row polymorphism;
- structural contract subtyping;
- effect typing and effect subsumption;
- declaration-site variance;
- sealed hierarchies;
- compile-time rules and diagnostics.

The Wrapper™ is the repository name for a .sleela source file governed by the
Sleela Language Metadocument.

## 4. Compiler and Front End

The C++ front end provides:

- syntax-version resolution;
- Java-like lexing;
- recursive-descent parsing;
- AST construction;
- semantic/compiler processing;
- code generation;
- compile-time conducted methods;
- diagnostics and source mapping.

The compiler CLI includes run, check, version, and XCLASS-related operations.

The current repository records language syntax 1.0 and toolchain version 0.1.2
in VERSION.md.

## 5. Execution Core

The C execution core is a stack virtual machine with:

- operand stack;
- constant pool;
- globals;
- local variables;
- call frames;
- opcode dispatch;
- arithmetic;
- comparisons;
- logical operations;
- jumps;
- calls and returns;
- printing;
- threading;
- locks;
- message passing;
- halt and result retrieval.

The central C ABI entry point is slcore_exchange(), with operations for reset,
constants, globals, functions, emission, patching, entry selection, execution,
and result retrieval.

The documented bounded concurrency model includes up to 128 threads, 32
lock-table slots, and a 32-slot two-tuple mailbox.

## 6. Operating-System Facilities

The OS abstraction layer covers:

- threads;
- TCP sockets;
- files;
- pipes/named pipes;
- paths;
- terminal/PTY;
- dynamic libraries;
- time;
- processes.

Target platforms documented by the current implementation are Linux, macOS,
and Windows 10+, with POSIX and Win32 backends.

## 7. General APIs

The SLeeLa API direction covers:

### I/O
- standard input/output;
- streams;
- buffers;
- terminal interaction;
- structured records.

### File I/O
- files;
- directories;
- paths;
- metadata;
- reading/writing;
- traversal;
- configuration.

### Process/System
- process launch;
- process control;
- environment;
- native executable execution;
- dynamic libraries;
- resource limits.

### Time
- precision clocks;
- NTP-related timing;
- HTTP timing records;
- RMI timing records;
- BODI timing records;
- international time-request markers.

### Networking
- TCP;
- UDP;
- sockets;
- clients;
- listeners;
- routers;
- servers;
- network messages;
- NAT-aware operation.

### Email
- SMTP;
- message construction;
- configuration;
- headers;
- transport integration.

### Databases
- connections;
- queries;
- records;
- persistence;
- configuration surfaces.

### GUI/Application
- application windows and surfaces;
- controls;
- events/listeners;
- application state;
- OS integration.

## 8. XML and BODI

SLeeLa provides a declarative XML/BODI direction for:

- email and SMTP;
- subjects;
- posts;
- listeners;
- routers;
- services;
- application components;
- structured configuration;
- timing and other structured records.

## 9. Servers and Services

The server/service architecture covers:

- startup;
- configuration;
- listeners;
- ports;
- routing;
- request/response processing;
- logging;
- health;
- controlled shutdown;
- single-instance behavior where specified;
- firewall/UFW awareness;
- NAT-aware operation.

The project server/service editions include Discord-1™, Discord-2™, and
Discord-3™.

## 10. HTTP Family

The repository contains HTTP 1.x, HTTP 2.0/2.1, HTTP 3.0, and experimental
HTTP 4.0 work.

### HTTP 1.x
Traditional request/response server architecture.

### HTTP 2.0/2.1
Stream/multiplexing-oriented server architecture, framing, concurrent
request processing, configuration, logs, and output handling.

### HTTP 3.0
Application envelopes and processing over an HTTP/3/QUIC-oriented transport
architecture.

### HTTP 4.0
HTTP 4.0 is an experimental SLeeLa protocol generation, not an IETF HTTP/4
standard. It provides:

- explicit session/stream/frame lifecycle;
- OPEN, DATA, END, RESET, WINDOW, PING, PONG, RESUME, CAPSULE;
- stream IDs;
- request IDs;
- sequence numbers;
- capability negotiation;
- incremental delivery;
- resumability;
- flow-control signaling;
- carrier abstraction;
- authenticated-carrier expectations;
- MTU-aware application-layer segmentation;
- bounded reassembly;
- duplicate rejection;
- overlap rejection;
- resource limits.

HTTP 4.0 segmentation is application-layer segmentation, not IP fragmentation.
TLS remains a carrier/transport concern. HTTP/3/QUIC obtains transport security
through its QUIC/TLS architecture; HTTP 4.0 does not duplicate TLS in its frame
encoder.

## 11. Telephony and VoIP

The telephony-skya work provides an architecture for constructing telephony
and VoIP applications from SLeeLa source.

Feature families include:

- telephony APIs;
- device abstractions;
- software drivers;
- hardware-driver boundaries;
- buffering;
- queues;
- message passing;
- lock/synchronization conditions;
- OS adapters;
- cross-platform driver architecture;
- network/media application paths.

Construction model:

SLeeLa Source → Telephony API → Driver Layer → OS/Hardware →
Network/Media → Application

## 12. Drivers and Hardware

The driver architecture separates application interfaces from:

- buffering;
- queues;
- synchronization;
- message passing;
- software drivers;
- hardware drivers;
- OS backends;
- device-specific implementations.

The documented target operating systems are Linux, Windows 10+, and macOS.

## 13. Native Integration

The project includes native artifact/interoperability work for:

- dynamic/shared libraries;
- static archives;
- native executables;
- Linux kernel-module (.ko) artifacts;
- PE/COFF;
- Mach-O;
- raw native artifacts;
- native symbol/format inspection.

## 14. SecureJDK 28 / XCLASS

The XCLASS subsystem ingests SecureJDK 28 XML class descriptions using the
XCI-0001 model.

It covers:

- XML class loading;
- class/field/method structure;
- typed method skeletons;
- default-return reconstruction;
- SLeeLa emission;
- inspection and CLI integration.

An XCLASS description represents structure rather than ordinary executable
bytecode.

## 15. Nordshrift

Nordshrift is the .sst transpiler/control driver.

It provides:

- indentation-significant scripting sheets;
- source resolution;
- target selection;
- pipeline declarations;
- rules;
- effects;
- derivation;
- guards;
- interoperability declarations;
- profiles;
- diagnostics;
- object compatibility;
- semantic coordination.

Its central target triplet is:

JAVA | SLEELA | C

The SLeeLa target can continue into the native SLeeLa execution core.

## 16. SHEET.sheet and Conducted Methods

SHEET.sheet is the common object catalog. Current repository documentation
records 129 objects across 16 role categories, with system depth 3024 and
maximum complexity degree 4.

The documented conducted methods include:

- conduct("Name");
- role("Name");
- insight("Name");
- congruent("A","B");
- route("A","B");
- sysdepth();
- degreemax().

Nordshrift also uses the catalog for per-target relevance:

DIRECT | MODEL | NONE

## 17. Subject Libraries

The repository documents first-class computational subject branches for:

### Mathematics
Arithmetic, numerical functions, trigonometry, modular operations, series,
and numerical assertions.

### Physics
Native physical formula/API and executable model surfaces.

### Astrophysics
Numerical ABI, C++ facade, native frontend vocabulary, and XML observation
storage.

### Sociology
Statistical mathematics, C++ facade, frontend vocabulary, and XML
model/procedure structures.

### Economics
Economic identities and models including Y = C + I + G, present value,
investment, depreciation/renewal, backlog, and marginal NPV.

### Inference / Statistics
Mean, standard deviation, OLS trend, correlation, R², CAGR, z-score,
forecast, classification, and data-series analysis.

### Chemistry
Chemistry library and frontend/API surfaces.

### Finance
Financial library and frontend/API surfaces.

## 18. Social/System Modeling

The social-model branch represents:

DESIGN → METHOD → REMEDY

It includes:

- proposed normals;
- observed states;
- 2D and 3D boundaries;
- radial and box tolerances;
- deviation;
- stale-assumption detection;
- workforce allocation drift;
- clarity synthesis.

The model distinguishes assumptions from facts and computed results from
empirical measurements.

## 19. Evidence and Analytical Method

The repository also contains a separate evidentiary-method body of work,
including:

- simple facts;
- stirred-ratio comparison;
- close-circuit-ratio detection;
- two-event continuity testing;
- corrected values;
- evidentiary size;
- root sources;
- disclosure;
- provenance;
- context;
- binary/all-occurrence testing;
- ordered continuance.

Its governing discipline is to distinguish fact, interpretation, association,
label, and unknown state.

## 20. Lens

The Lens concept is a focused get/set abstraction with compositional behavior.
The repository also applies the Lens idea to bounded historical/data views.

Conceptually:

GET + SET + COMPOSITION

## 21. Phraign™

Phraign™ is the project's frame-based terminal-control model.

It provides:

- planar terminal geometry;
- integer x/y addressing;
- upper-left origin;
- terminal-following geometry;
- SIGWINCH resize awareness;
- capability negotiation;
- Bash text protocol;
- native C++ frame/geometry layer;
- pixel-terminal support;
- 60 Hz pixel-terminal support.

The Phraign City 3D model demonstrates large configurable terminal rendering.

## 22. Sigil™

The dependency-free Sigil tooling provides:

- configurable QR code generation;
- pure-Python QR encoding;
- Reed-Solomon ECC;
- masking/penalty scoring;
- grayscale PNG output;
- deterministic 248 × 48 steganographic frame;
- SHA-256 seeding;
- HMAC-SHA-256 DRBG;
- embedded digest;
- recovery and verification.

The deterministic frame is idempotent: identical input produces identical
output.

## 23. Security and Integrity

Security is treated as a cross-layer property.

Documented mechanisms include:

- SHA-256 verification;
- fail-closed verification;
- explicit privilege opt-in;
- no implicit privilege escalation;
- bounded inputs;
- bounded memory;
- bounded HTTP 4.0 reassembly;
- controlled native execution;
- authenticated transport expectations;
- secure configuration;
- diagnostics and integrity manifests.

Integrity manifests establish file integrity; they are not by themselves a
claim of authorship, legal validity, or cryptographic commit signing.

## 24. Testing

The repository includes:

- execution-core smoke tests;
- threading smoke tests;
- compiler/version tests;
- subject-library numeric assertions;
- HTTP tests;
- HTTP 4.0 frame tests;
- HTTP 4.0 segmentation/reassembly tests;
- Sigil verification tests;
- platform build workflows;
- integration/smoke tests.

The engineering target is:

IMPLEMENTATION + UNIT TEST + INTEGRATION TEST + PLATFORM TEST + FAILURE TEST

## 25. Branches of Computation

SLeeLa covers the following major computational branches:

1. Symbolic/formal computation.
2. Arithmetic computation.
3. Numerical analysis.
4. Statistical computation.
5. Inference.
6. Physical computation.
7. Astrophysical computation.
8. Chemical computation.
9. Economic computation.
10. Financial computation.
11. Social/system modeling.
12. Information processing.
13. State-machine computation.
14. Concurrent computation.
15. Distributed computation.
16. Protocol computation.
17. Compiler computation.
18. Transpilation.
19. Systems computation.
20. Hardware/driver computation.
21. Media/telephony computation.
22. User-interface computation.
23. Server/service computation.
24. Security/integrity computation.
25. Data-provenance/evidence methods.

## 26. Methods of Programming and Computation

SLeeLa is deliberately multi-method.

### Imperative
Ordered operations and explicit state changes.

### Object-Oriented
Classes, fields, methods, contracts, and encapsulated objects.

### Functional / Expression-Oriented
Expressions, pure flows, typed transformations, and composable operations.

### Declarative
XML, BODI, .sst sheets, subject models, procedures, rules, and configuration.

### Concurrent
Threads, locks, joining, message passing, and bounded parallel execution.

### Event-Driven
Listeners, routers, server events, GUI events, terminal events, and network
events.

### Stream / Frame
Network frames, HTTP lifecycle, terminal frames, buffering, segmentation, and
reassembly.

### Systems
Processes, files, memory, sockets, dynamic libraries, OS backends, and
drivers.

### Numerical
Formulas, numerical approximation, statistics, inference, and domain models.

### Model-Based
Explicit assumptions, parameters, observations, boundaries, relationships,
and results.

### Evidence-Bounded
Separation of source, fact, interpretation, association, and unknown state.

### Transformation
Source → AST → core representation, and .sst → Java/SLeeLa/C.

### Hybrid
These methods can coexist within one application.

## 27. Example Cross-Layer Constructions

### General application

IDEA → SLeeLa Source → Compiler → Core → API → OS/Domain → Result

### Scientific application

DATA → SLeeLa → Subject Library → Numerical/Statistical Model → Result

### Server

SLeeLa → Server API → Listener → Router → HTTP → Application → Response

### Telephony

SLeeLa → Telephony API → Driver → Buffer/Queue → Network/Media → Application

### Data service

SLeeLa → File/Database → Model/Inference → HTTP → Server/Client

### Terminal system

SLeeLa → Phraign™ → Frame/Geometry → Terminal

## 28. Application Families

The current feature map supports construction of:

- command-line programs;
- scientific programs;
- numerical tools;
- statistical applications;
- economic models;
- financial models;
- physics and astrophysics applications;
- chemistry applications;
- sociology/modeling applications;
- data-processing tools;
- file utilities;
- network clients;
- network servers;
- HTTP servers;
- email applications;
- database applications;
- GUI applications;
- terminal applications;
- native-process tools;
- system utilities;
- services;
- telephony/VoIP applications;
- driver-integrated applications;
- distributed services;
- protocol implementations;
- compiler/tooling extensions;
- transpilation pipelines.

Specific production readiness depends on the implementation status of the
selected subsystem.

## 29. Feature Status Vocabulary

| Status | Meaning |
|---|---|
| Implemented | Executable/buildable source exists. |
| Documented | A defined interface/specification exists; implementation depth may vary. |
| Integrated | Connected to another SLeeLa subsystem. |
| Experimental | Implemented/designed but deliberately not presented as a mature external standard. |
| Architectural | Contract/design exists while implementation remains incomplete. |
| Historical | Retained for project history and not authoritative for the current build. |

## 30. Complete Architecture

SLeeLa Wrapper™
→ Version Resolver
→ Lexer
→ Parser
→ AST
→ Semantic/Compiler Layer
→ SLeeLa Core
→ Runtime Services
→ OS Abstraction
→ Files / Processes / Memory / Threads / Time / Terminal / Network
→ Drivers / Libraries / Services
→ Application / Server / Device / Scientific Result
→ Verify

Alongside the execution path:

SST.model → Nordshrift → JAVA | SLEELA | C

SHEET.sheet → Objects | Roles | Methods | Relevance

Subject Libraries → MATH | PHYSICS | ASTROPHYSICS | CHEMISTRY | ECONOMICS |
FINANCE | STATISTICS | INFERENCE | SOCIOLOGY

Security/Integrity → Verification | Resource Limits | Authenticated Transport

## 31. Governing References

Primary technical references include:

- README.md
- SUMMARY.md
- GLOSSARY.md
- SLEELA.md
- SOURCE.md
- COMPILER.md
- VERSION.md
- NORDSHRIFT.md
- src/Sleela.manifest
- SST.model
- impl/xclass/XCLASS.model
- impl/README.md
- impl/DESIGN.md
- SHEET.sheet
- subject-library documentation
- HTTP generation documentation
- server/service documentation
- telephony/driver documentation

Where EDITION.md summarizes another document, that document remains the
authoritative technical reference.


## 32. Platform Completion Layer

The repository now defines the engineering completion layer for turning the feature catalog into a complete application platform. New companion documents are:

- LANGUAGE.md — practical language surface;
- LANGUAGE.SPEC.md — compiler/type/semantic contract;
- RUNTIME.md — VM/runtime contract;
- STANDARD.LIBRARY.md — portable standard-library map;
- BUILD.SYSTEM.md — source-to-application build lifecycle;
- PACKAGE.MANAGER.md — dependency and artifact model;
- ABI.md — language/core/native/platform/protocol ABI boundaries;
- PLATFORM.md — Linux, macOS and Windows 10+ portability contract;
- SECURITY.md — layered security model;
- TESTING.md — positive, negative, fuzz, concurrency, ABI and platform testing;
- IDE.md — editor and language-server direction;
- APPLICATIONS.md — SLeeLa-native application lifecycle;
- VOIP.md — telephony and VoIP architecture;
- HTTP.md — HTTP generation and HTTP 4.0 profile;
- DRIVERS.md — cross-platform driver contract;
- API.INDEX.md — consolidated API navigation.

These documents intentionally distinguish implemented facilities from architectural targets. They do not convert a documented contract into a claim of production maturity.

### Application-completion chain

SOURCE → CHECK → DEPENDENCIES → COMPILE → LINK/ASSEMBLE → TEST → PACKAGE → SIGN → INSTALL → RUN → VERIFY

### Platform layers

LANGUAGE → COMPILER → CORE ABI → RUNTIME → STANDARD LIBRARY → OS ABSTRACTION → NATIVE/DRIVER LAYER → APPLICATION

The resulting architecture supports the explicit goal of building a complete application through SLeeLa tooling while keeping native and platform dependencies visible and controlled.


---

**Max Rupplin — MEARVK LLC — 2026**
