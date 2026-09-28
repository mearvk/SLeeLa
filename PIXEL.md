# PIXEL.md

## Project-Level PIXEL View

This document provides the project-level **PIXEL** view of **SLeeLa** and its relationship to **Ubuntu.Determinant.Beta.Restricted**.

PIXEL answers two questions:

1. **What's Made** — the major systems, models, tools, and architectural capabilities represented by the repository.
2. **What's Included** — the source, scripts, data, specifications, experiments, workflows, and documentation that make those capabilities inspectable.

The repository is intentionally experimental and broad. PIXEL describes architectural scope without turning every prototype, model, experiment, workflow, or specification into a claim of production certification.

## What's Made

SLeeLa represents a broad programming-language and systems-development environment centered on a Java-like language and a C/C++ execution core.

Major capabilities represented in the repository include:

- **SLeeLa language and runtime**
  - A Java-like programming language.
  - A Turing-complete, thread-friendly C/C++ execution core.
  - The `.sleela` source format, documented as a **Wrapper™**.
  - Runtime execution, checking, compilation, version handling, and native execution interfaces.

- **Nordshrift**
  - The `.sst` transpiler/semantic layer.
  - NS-SST-0001 compatibility material.
  - Nordshrift 2.0 semantic and control-sheet models.
  - Cross-target work involving Java, SLeeLa, and C.
  - Subject, quantity, unit, assumption, relation, formula, transformation, evidence, explanation, and work-plan models.

- **Cross-platform systems architecture**
  - C/C++ implementations for Linux, macOS, and Windows 10+.
  - OS-aware abstractions for threads, TCP sockets, files, pipes/named pipes, paths, terminals, dynamic libraries, and time.
  - POSIX and Win32 backend organization.
  - Platform-specific build scripts and CI workflows.

- **Compiler and executable tooling**
  - SLeeLa compiler and frontend infrastructure.
  - Native executable launching.
  - JVM-family source ingestion.
  - The `cmd` Java command executable format and associated native linker/inspector work.
  - SHA-256 integrity verification and execution gates.

- **Memory and runtime controls**
  - The Memory Manager.
  - Optional fail-closed memory limits.
  - Runtime resource and execution controls.
  - System Health and system IQ/insight-quality models.

- **Subject libraries**
  - Math.
  - Physics.
  - Astrophysics.
  - Sociology.
  - Economics.
  - Inference/statistics.
  - Chemistry.
  - Financial mathematics.
  - Native implementations, APIs, XML models, and validation-oriented numerical tests where represented.

- **Declarative object and catalog systems**
  - `SHEET.sheet` as a catalog of common system objects and roles.
  - Conducted methods, congruence, routing, contracts, constraints, dependencies, and limits.
  - Object compatibility material used by the SLeeLa/Nordshrift architecture.

- **Terminal and graphical-style models**
  - Phraign™ pixel/frame terminal concepts.
  - Coordinate-based terminal geometry.
  - Resize-aware terminal handling.
  - Pixel-terminal C/C++ layers.
  - The Phraign City 3D model and related configurable viewpoints.

- **Data, evidence, and modeling workflows**
  - Banks and generated data collections.
  - Economic and infrastructure models.
  - Inference models.
  - Social-model experiments.
  - Source and evidence documentation intended to keep observations, specifications, derivations, models, inferences, and assumptions distinguishable.

- **Security and integrity workflows**
  - SHA-256 manifests.
  - Verification workflows.
  - Explicit privilege opt-in in documented security-sensitive paths.
  - CI jobs that regenerate or verify integrity artifacts.
  - Defensive and validation-oriented tooling.

## What's Included

The repository makes the above architectural scope inspectable through multiple forms of project material.

### Source

- `impl/` — the primary C/C++ implementation and buildable execution system.
- `src/` — an earlier/parallel Java prototype retained for reference and comparison.
- Subject-library source trees.
- OS abstraction layers.
- Compiler, runtime, Nordshrift, terminal, launcher, linker, and memory-management source.
- Supporting tools and utilities.

The repository distinguishes the authoritative buildable implementation from earlier or parallel prototype material rather than treating every source tree as equivalent.

### Scripts and Workflows

- Linux, macOS, and Windows build scripts.
- Make-based build and test workflows.
- GitHub Actions CI workflows.
- Data-bank generation and normalization workflows.
- SHA-256 manifest generation and verification workflows.
- Platform-specific launcher workflows.
- Supporting shell, Python, PowerShell, and native tooling where represented.

### Specifications and Models

- `SST.model`.
- `SST-2.0.model`.
- Language and source specifications.
- ABI/API references.
- Object catalogs.
- Structural and semantic models.
- Platform API documentation.
- CMD executable-format specifications.
- Terminal and protocol specifications.

These documents describe intended contracts, compatibility models, implementation details, or experiments according to their individual status; their existence does not by itself establish external certification.

### Data and Experimental Material

- `BANKS.md`, `BANKS2.md`, `BANKS4.md`, and related generated data.
- XML subject models and observation stores.
- Economic and infrastructure examples.
- Inference-model inputs and outputs.
- Social-model experiments.
- Example SLeeLa programs.
- Manifesto and other runnable/documentary Wrapper™ material.
- Generated integrity records and supporting artifacts.

### Documentation

The repository includes architectural and operational documentation covering:

- architecture;
- applications;
- build systems;
- compiler behavior;
- source formats;
- Nordshrift;
- APIs and ABIs;
- platform support;
- drivers;
- file I/O;
- memory management;
- subject libraries;
- definitions and glossary material;
- versioning;
- security and integrity;
- terminal/pixel interfaces;
- workflows and completion status.

## Relationship to Ubuntu.Determinant.Beta.Restricted

Within this PIXEL view, **Ubuntu.Determinant.Beta.Restricted** is treated as a related reference/project context rather than as an authority that automatically certifies SLeeLa.

The relationship is architectural and documentary: the projects may share source concepts, build-system ideas, operating-system concerns, tooling patterns, documentation practices, or experimental infrastructure. Specific files and implementations remain attributable to their actual repository and revision.

SLeeLa therefore should be inspected through its own source, specifications, tests, workflows, and documentation. A concept being present in a related repository does not, by itself, establish that the concept is implemented, complete, tested, certified, or production-ready in SLeeLa.

## PIXEL Reading Rule

A useful project-level reading order is:

**Source → Build → Runtime → Models → Data → Tests → Workflows → Documentation → Status**

This keeps architectural claims connected to inspectable repository material.

Where the repository contains a prototype, experimental implementation, specification, generated artifact, or planned workflow, PIXEL records its presence and architectural role without silently upgrading its status.

## Scope and Status

PIXEL is a repository-navigation and architectural-scope document.

It is **not**:

- a production certification;
- a security certification;
- a standards-body approval;
- a legal determination;
- a government or institutional endorsement;
- a guarantee that every documented feature is production-complete;
- a claim that every experiment is scientifically validated;
- a claim that every specification has an independently verified implementation.

The appropriate status of an individual capability should be determined from its source, tests, CI results, implementation notes, specification, and other directly relevant project evidence.

---

**Max Rupplin - MEARVK LLC - 2026**
