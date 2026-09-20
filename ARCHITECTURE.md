# ARCHITECTURE.md — the two source trees, delineated

This repository contains **two distinct implementations** that share the
"Sleela / Nordshrift" naming but are separate bodies of code with no shared
compilation units. This document draws the boundary explicitly so contributors
know which tree they are working in.

| | [`impl/`](impl/) — the working system | [`src/`](src/) — the Java prototype |
|---|---|---|
| **Language** | C (execution core) + C++17 (front end, Nordshrift, subjects) | Java |
| **Status** | **Authoritative, buildable, tested.** This is "the working implementation." | Earlier / parallel prototype (`implementations/_001_`). Not built or exercised by the C/C++ build or CI. |
| **Build** | [`impl/Makefile`](impl/Makefile); [`build-linux.sh`](build-linux.sh) (Linux/macOS) and [`build-windows.ps1`](build-windows.ps1) (Windows 10+, MinGW) → `sleela`, `nordshrift` binaries | IntelliJ module ([`SLeeLa.iml`](SLeeLa.iml)); no Maven/Gradle/Ant build is committed |
| **Platforms** | **Linux, macOS (Darwin/clang), and Windows 10+ (MinGW).** OS facilities go through the `impl/core` abstraction layer (threads, sockets, files, paths, terminal, dynamic libs, time); each reports its native backend (`linux` / `macos` / `windows`) and has POSIX and Win32 implementations. Each platform is built in CI. | JVM |
| **Entry points** | `sleela` and `nordshrift` CLIs ([`impl/frontend/driver.cpp`](impl/frontend/driver.cpp), [`impl/nordshrift/nordshrift.cpp`](impl/nordshrift/nordshrift.cpp)) | `main()` classes such as `drivers/Startup.java`, `nordshrift/NordshriftStartup.java`, `bodi/BodiNetworkBootstrap.java` |
| **Tests / CI** | `make test` (examples, version awareness, subject numeric assertions, platform smokes); the SHA-256 verify gate | none in CI |

## Which tree should I use?

- **Building or running Sleela, changing the language, the VM, Nordshrift, or a
  subject library → use [`impl/`](impl/).** It is the only tree the toolchain
  compiles and the CI/build gate covers. Start at
  [`impl/README.md`](impl/README.md) and [`impl/DESIGN.md`](impl/DESIGN.md).
- **[`src/`](src/) is a historical/parallel Java prototype** (`_001_`) of the
  same ideas — an RMI/Bodi-style Nordshrift experiment. Treat it as reference
  material, not the shipping implementation. It is not required to build or run
  Sleela.

## What lives where

### `impl/` — the C/C++ implementation (authoritative)

```
impl/core/        C stack VM + stable C ABI (slcore_exchange)
impl/frontend/    C++ lexer/parser/AST/compiler and the `sleela` CLI
impl/nordshrift/  C++ `.sst` transpiler driver and the `nordshrift` CLI
impl/subjects/    math, physics, economics, inference, chemistry, finance
impl/catalog/     SHEET.sheet catalog parser
impl/xclass/      `.xclass` (SecureJDK 28) ingest
impl/tests/       version-awareness and subject numeric-assertion harnesses
```

### `src/` — the Java prototype (`implementations/_001_`)

```
src/implementations/_001_/nordshrift/  Java Nordshrift experiment (largest part)
src/implementations/_001_/bodi/        Bodi network bootstrap / smoke Java code
src/implementations/_001_/{drivers,models,modules,parsers,...}
src/Sleela.manifest                    SL-META-0001 metadocument (shared spec)
src/Sleela.formal-core                 formal-core companion to the metadocument
```

Note that a few **specification** files under `src/` (notably
[`src/Sleela.manifest`](src/Sleela.manifest), the SL-META-0001 metadocument, and
`src/Sleela.formal-core`) are language-level documents referenced by *both*
trees. They are specifications, not part of the Java program, and remain
authoritative for the language definition regardless of implementation.

## Naming caveat

Both trees use the words **Sleela** and **Nordshrift**. When a document, issue,
or commit says "Nordshrift," disambiguate by path: `impl/nordshrift/…` is the
C++ transpiler driver that ships; `src/implementations/_001_/nordshrift/…` is
the Java prototype. There is no code sharing between them.
