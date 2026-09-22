# PURPOSE.md — SLeeLa Start, Middle, and End Design Goals

## 1. Purpose

SLeeLa is intended to be a complete, inspectable software system built around a
Java-like programming language, a native C/C++ execution core, and a set of
well-defined system APIs.

The purpose of SLeeLa is not merely to provide a compiler. It is to provide a
coherent path from source idea to validated program to execution to useful
software result while keeping the language, runtime, operating-system
interfaces, networking, data, security, and documentation connected by explicit
contracts.

The governing design dimensions are:

**START → MIDDLE → END**

and:

**NOVICE → INTERMEDIATE → ADVANCE**

The first tuple describes the lifecycle of software. The second describes the
progressive depth at which a person or subsystem can use and understand that
lifecycle.

---

## 2. START — Define and Accept the Program

The Start is everything required to take an idea or program description and
turn it into a well-defined SLeeLa input.

### 2.1 Language

The language should provide:

- clear syntax and keywords;
- explicit syntax-version declarations;
- classes, fields, methods, and structured data;
- controlled types and expressions;
- control flow;
- functions and method calls;
- bounded concurrency;
- explicit access modifiers;
- static and protected semantics;
- managed memory behavior;
- predictable error reporting;
- a stable source-file contract.

A SLeeLa source file must have a defined meaning before execution begins.

### 2.2 Input and API Boundaries

The Start includes the interfaces by which software enters SLeeLa.

These include:

- command-line input;
- standard input/output;
- file input/output;
- configuration;
- XML and BODI input;
- networking declarations;
- posts, listeners, and routers;
- email declarations;
- database configuration;
- GUI/application inputs;
- Nordshrift input;
- supported external-language ingestion where explicitly implemented.

Every input boundary should have:

**interface → validation → implementation → error handling → test → documentation**

### 2.3 Source Validation

Before execution, SLeeLa should establish:

1. What program was supplied.
2. Which language/version it declares.
3. Whether the syntax is valid.
4. Whether referenced names and types are valid.
5. Whether security and resource rules permit the operation.
6. Which runtime capabilities are required.

Invalid input should fail early and descriptively.

### 2.4 Start Design Goal

> **A SLeeLa program enters the system as a defined, versioned, validated
> object rather than as an ambiguous collection of text.**

---

## 3. MIDDLE — Compile, Coordinate, and Execute

The Middle is the operating heart of SLeeLa.

It connects the language front end to the native execution core and then to
the system facilities required by an application.

### 3.1 Compiler

The compiler is responsible for transforming valid SLeeLa source into an
executable representation for the SLeeLa core.

The intended path is:

SLeeLa Source
→ Version Resolution
→ Lexer
→ Parser
→ AST
→ Semantic Validation
→ Compiler / Code Generation
→ Core Program Representation

The compiler must not silently invent semantics that are absent from the
language contract.

### 3.2 Execution Core

The C/C++ execution system is the authoritative native implementation.

The core should provide:

- values;
- operand and call-frame management;
- opcode execution;
- functions;
- managed memory;
- concurrency primitives;
- stable native interfaces;
- resource limits;
- runtime diagnostics;
- platform abstraction.

The front end and execution core should remain separated by explicit APIs.

### 3.3 System Services

The Middle connects the program to operating-system capabilities through
controlled abstractions.

The principal service families are:

- memory;
- files;
- paths;
- processes;
- threads;
- terminal/PTY;
- sockets;
- TCP/UDP;
- time;
- dynamic libraries;
- platform-specific facilities.

Linux, macOS, and Windows implementations should expose the same SLeeLa-level
contract wherever the operating systems provide equivalent capabilities.

Platform differences should be isolated in platform backends rather than
spread throughout application logic.

### 3.4 Application APIs

The Middle is where the general SLeeLa software API becomes real.

The API system should grow toward complete, documented coverage of:

- core utilities;
- I/O;
- file I/O;
- networking;
- HTTP;
- email;
- databases;
- GUI/application facilities;
- serialization;
- XML;
- BODI;
- data analytics;
- security;
- server operation;
- deployment.

The api directory should remain the human-readable catalog of these interfaces,
with each important API connected to its actual implementation.

### 3.5 Nordshrift

Nordshrift is a semantic coordination and transformation layer.

Its role is to provide a structured way to represent:

- subjects;
- quantities;
- units;
- assumptions;
- relations;
- formulas;
- transformations;
- results;
- evidence;
- explanations;
- validation;
- work plans.

The Middle therefore preserves the distinction between a computation and the
meaning or evidence surrounding that computation.

### 3.6 Security and Integrity

The Middle must establish the trust boundary around execution.

Security goals include:

- SHA-256 verification where the execution/build contract requires it;
- fail-closed verification failures;
- explicit privilege requirements;
- bounded inputs;
- no implicit privilege escalation;
- controlled native execution;
- controlled network operations;
- protected resource access;
- explicit credential handling;
- auditable configuration.

Integrity manifests establish file integrity. They do not by themselves
establish authorship, legal validity, or cryptographic commit signing.

### 3.7 Testing

The Middle must be continuously testable.

For each important subsystem, the target is:

**implementation + unit test + integration test + platform test + failure test**

The build system should distinguish:

- source compilation;
- unit tests;
- integration tests;
- platform smoke tests;
- security verification;
- packaging/release verification.

### 3.8 Middle Design Goal

> **A validated SLeeLa program is transformed and executed through explicit,
> testable contracts without losing its language, security, platform, or
> provenance boundaries.**

---

## 4. END — Produce a Complete, Verifiable Result

The End is what the user, operating system, service, or downstream program
actually receives.

Execution is not the end by itself. The end is a useful result whose origin
and behavior can be understood.

### 4.1 Program Result

A completed SLeeLa operation should produce one or more defined results:

- terminal output;
- files;
- records;
- network responses;
- GUI behavior;
- email operations;
- database operations;
- generated source;
- compiled artifacts;
- server behavior;
- analytical results;
- structured BODI results.

The result must correspond to the requested operation.

### 4.2 Result Validation

Where validation is meaningful, the End should record:

- what was requested;
- what was executed;
- what inputs were used;
- what transformations occurred;
- what result was produced;
- whether validation passed;
- what errors occurred;
- what evidence supports the result.

For scientific and analytical software, modeled, derived, inferred, observed,
specified, and assumed information should remain distinguishable.

### 4.3 Reproducibility

A mature SLeeLa result should be reproducible when the application permits it.

The system should identify:

- SLeeLa implementation version;
- language syntax version;
- relevant API version;
- platform/backend;
- input/configuration;
- source or artifact identity;
- integrity information;
- applicable dependencies.

### 4.4 Server and Long-Running Software

For server applications, the End includes the operational state of the
software.

That includes:

- startup;
- configuration;
- listener state;
- routing;
- health;
- controlled shutdown;
- single-instance behavior where required;
- NAT/network configuration where applicable;
- operational diagnostics.

A server should not claim to provide a network capability that its underlying
implementation does not actually provide.

### 4.5 End Design Goal

> **A SLeeLa operation ends with a defined result that can be inspected,
> validated, explained, and—where applicable—reproduced.**

---

## 5. NOVICE → INTERMEDIATE → ADVANCE

The second three-tuple defines progressive software and user capability.
These levels are not quality rankings. They describe increasing scope,
responsibility, and depth.

### 5.1 Novice

Novice is the entry layer.

A Novice user or component should be able to:

- read basic SLeeLa source;
- understand the Start → Middle → End model;
- create simple source files;
- use basic classes, methods, values, and control flow;
- use documented basic I/O;
- run documented examples;
- understand basic errors;
- use the foundational API catalog;
- identify where source, compiler, runtime, and result fit in the system.

**Novice goal:**

> **Understand the path and successfully complete a basic, documented
> operation.**

### 5.2 Intermediate

Intermediate is the application-building layer.

An Intermediate user or component should be able to:

- build multi-file SLeeLa programs;
- use structured data;
- use file I/O and networking;
- use posts, listeners, and routers;
- use XML/BODI;
- use email and database APIs where implemented;
- use data analytics;
- work with Nordshrift input objects;
- understand compiler/runtime boundaries;
- diagnose common build and runtime failures;
- write tests;
- understand platform differences;
- use security and integrity facilities correctly.

**Intermediate goal:**

> **Build, integrate, test, and diagnose a real application using multiple
> SLeeLa subsystems.**

### 5.3 Advance

Advance is the system-engineering layer.

An Advance user or component should be able to:

- design new SLeeLa APIs;
- extend the compiler and runtime;
- implement native C/C++ bindings;
- define language semantics;
- create platform backends;
- design networking and server components;
- reason about managed memory and security boundaries;
- extend BODI and Nordshrift;
- build cross-platform integrations;
- establish validation and integrity boundaries;
- create unit, integration, platform, and failure tests;
- document interfaces and implementation contracts;
- determine whether a capability is implemented, contractual, experimental,
  or incomplete.

**Advance goal:**

> **Extend SLeeLa itself while preserving its language, runtime, API,
> security, platform, testing, and documentation contracts.**

### 5.4 The Three-Tuple

The progression is therefore:

**NOVICE**
→ understand and operate

**INTERMEDIATE**
→ build and integrate

**ADVANCE**
→ design and extend

The levels are cumulative. Advance work should remain understandable through
the Intermediate application layer and ultimately expose a usable Novice path
where practical.

---

## 6. The Complete SLeeLa Journey

The lifecycle and capability tuples combine as follows:

| Lifecycle | Novice | Intermediate | Advance |
|---|---|---|---|
| **START** | Read and validate basic source | Define multi-component inputs and APIs | Define language/API semantics |
| **MIDDLE** | Compile and run documented programs | Integrate runtime and system services | Extend compiler, runtime, and native core |
| **END** | Inspect a basic result | Validate an application result | Design reproducible and auditable result systems |

The three stages form one continuous design:

START
  |
  v
DEFINE + VALIDATE
  |
  v
MIDDLE
  |
  v
COMPILE + EXECUTE + INTEGRATE + PROTECT
  |
  v
END
  |
  v
PRODUCE + VALIDATE + DOCUMENT
  |
  v
USEFUL SOFTWARE RESULT

The cycle may then begin again:

Result
  ↓
Observation / Feedback
  ↓
New Source or Configuration
  ↓
START

SLeeLa should therefore be designed as a complete software lifecycle, not as
three disconnected subsystems.

---

## 7. Design Goals for the Software Already in the Repository

The current repository contains substantially more than the compiler itself.
That breadth should be organized around the Start/Middle/End model.

### Start responsibilities

These primarily belong to the Start:

- impl/frontend/
- language specifications;
- source-file definitions;
- version handling;
- BODI input definitions;
- API declarations;
- configuration;
- Nordshrift input models.

### Middle responsibilities

These primarily belong to the Middle:

- impl/core/
- impl/frontend/ compiler implementation;
- impl/nordshrift/;
- impl/subjects/;
- runtime services;
- native API implementations;
- networking;
- memory management;
- security verification;
- server implementation;
- platform backends.

### End responsibilities

These primarily belong to the End:

- executable program results;
- sleelas server operation;
- generated artifacts;
- API/server responses;
- analytical output;
- validation records;
- health and operational state;
- release artifacts.

This division does not require moving existing directories. It provides a
design lens for deciding where new software belongs and which responsibility
is missing when a feature is incomplete.

---

## 8. What Must Be Addressed as SLeeLa Grows

The current repository demonstrates substantial implementation breadth. The
next design work should concentrate on consistency rather than simply adding
more independent features.

### 8.1 API-to-Implementation Coverage

Every important API documented under api/ should have a clear answer to:

- Where is the interface?
- Where is the implementation?
- What platforms are supported?
- What is actually implemented today?
- What is only a contract or planned interface?
- How is it tested?
- What is the failure behavior?

### 8.2 Compiler-to-Runtime Coverage

Every language feature should map to:

**syntax → AST → semantic rule → compiler lowering → runtime behavior → test**

A feature documented only at the syntax level is not complete.

### 8.3 Platform Coverage

Linux, macOS, and Windows should share the same public SLeeLa contract where
possible, while platform-specific behavior remains explicit.

### 8.4 Security Coverage

Security-sensitive operations should have:

**authorization → validation → integrity check → execution boundary →
failure behavior → audit/diagnostic path**

### 8.5 Documentation Coverage

Documentation should describe the implementation that actually exists.

Where an API is intentionally a contract, monitor, adapter, or future
integration boundary, the documentation should say so explicitly instead of
presenting it as a completed subsystem.

### 8.6 Test Coverage

The project should progressively close the gap between:

**documented capability → implemented capability → tested capability**

This is especially important as api/, server-edition/, runtime/, and the
native subject libraries continue to expand.

---

## 9. End-State Vision

The long-term SLeeLa design goal is a software system in which a developer can
begin with a defined program, move through a controlled compiler/runtime and
system API stack, and arrive at a useful result without crossing an undefined
boundary.

The desired end state is:

DEFINE
  ↓
VALIDATE
  ↓
COMPILE
  ↓
EXECUTE
  ↓
INTEGRATE
  ↓
PRODUCE
  ↓
VALIDATE RESULT
  ↓
DOCUMENT / REPRODUCE

SLeeLa should ultimately be able to answer, for every significant operation:

1. What entered the system?
2. What rules governed it?
3. What software transformed it?
4. What resources did it use?
5. What security boundaries applied?
6. What result came out?
7. How was the result validated?
8. Can the operation be understood or reproduced?

That is the central purpose of SLeeLa:

> **To provide a coherent software path from defined source to validated
> result, with the language, compiler, runtime, operating system, APIs,
> security model, and documentation remaining connected throughout the entire
> journey.**

---

## 10. Guiding Rule

When deciding whether a new SLeeLa feature belongs in the project, ask:

**START:** Can the system define and validate it?

**MIDDLE:** Can the system implement and safely execute it?

**END:** Can the system produce, validate, and explain the result?

If any answer is no, the feature is not yet complete. The missing stage should
be identified and addressed rather than hidden behind documentation.
