# SLeeLa Build System

## Goal

A SLeeLa project should move from source to a reproducible application artifact without requiring the user to manually assemble compiler, runtime, native libraries and resources.

## Build lifecycle

SOURCE → CHECK → DEPENDENCIES → COMPILE → LINK/ASSEMBLE → PACKAGE → SIGN → TEST → INSTALL/RUN

The repository now provides a thin lifecycle driver at `tools/sleela-build.py`. It drives the existing native `impl/Makefile`; it does not replace the native compiler/linker implementation.

## Lifecycle commands

From the repository root:

```text
python3 tools/sleela-build.py init
python3 tools/sleela-build.py check
python3 tools/sleela-build.py build
python3 tools/sleela-build.py test
python3 tools/sleela-build.py run -- --help
python3 tools/sleela-build.py package
python3 tools/sleela-build.py install /path/to/bin
python3 tools/sleela-build.py clean
python3 tools/sleela-build.py doctor
python3 tools/sleela-build.py version
```

Windows may use the installed Python 3 launcher instead of `python3`.

## Project inputs

A project manifest should declare:

- project name and version;
- language version;
- target platforms/architectures;
- source roots;
- resource roots;
- dependencies;
- native libraries;
- permissions/capabilities;
- output type;
- signing policy;
- test suites.

`init` creates `sleela.project.json` when it does not exist.

## Dependency lock

`init` also creates `sleela.lock.json`. The first lock format records build tools and their observed versions. It is deliberately a provenance/locking foundation, not a claim that all third-party dependency resolution is complete.

Future dependency work must extend this file with resolved dependency identities, versions and integrity digests.

## Outputs

Supported artifact classes are intended to include:

- executable;
- library;
- service;
- plugin;
- package;
- source bundle;
- debug bundle;
- signed release bundle.

The current `package` command produces a provenance bundle under `impl/build/package-provenance/`.

## Provenance

Successful builds write `.sleela/build-manifest.json`.

The manifest records:

- host operating system;
- host release;
- machine;
- Python version;
- C compiler and version;
- C++ compiler and version;
- Make version;
- artifact sizes;
- artifact SHA-256 digests.

## Reproducibility

The current implementation records host/toolchain/artifact provenance. It does **not** yet prove byte-for-byte reproducibility.

Remaining reproducibility work includes deterministic build inputs, target triples, source revision, environment capture, clean-build comparison and documented reproducibility evidence.

## Cross compilation

The build system must distinguish host, target OS and target architecture. A host build must never silently emit an artifact for a different target.

## Security gates

Dependencies and generated artifacts are verified before execution or packaging when integrity manifests are enabled. Privilege escalation is never implicit.

## Current status

The lifecycle driver establishes the first application-level build control surface. Remaining build gates are:

- exact dependency locking;
- incremental builds;
- deterministic/reproducible builds;
- compiler/linker diagnostic capture;
- cross-platform CI integration;
- production package/signing flow.

**Max Rupplin — MEARVK LLC — 2026**


## Decompiler subsystem

The root build dispatcher includes the SLeeLa decompiler:

```text
make decompiler
```

The package Makefile at `lib/decompiler/Makefile` is authoritative for its native C/C++ compilation and sanity checks. The root dispatcher only enters that package build; it does not duplicate its source list.

The decompiler's six source classes are included in the library symbol manifest. The current library inventory is 967 `.sleela` source units, 88 module-facade symbols, and 1,055 total symbol records.


## Compiler and Decompiler Language/Format Reference Catalog

The compiler and decompiler maintain synchronized native reference catalogs for language names, producer programs, versions, source forms, intermediate/object representations, binary/executable/package formats, architecture, OS/ABI evidence, and safety policy. Reference matches are evidence only and never authorize execution.

- lib/compiler/LANGUAGE.FORMAT.REFERENCE.md
- lib/decompiler/LANGUAGE.FORMAT.REFERENCE.md

Current library inventory: 967 .sleela source units, 74 package families, 88 module-facade symbols, 1,055 total symbol records.
