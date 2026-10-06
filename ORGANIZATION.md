# SLeeLa — Repository Organization

A one-page map of this repository, intended to orient a new contributor or an
AI assistant (Kiro, ChatGPT, etc.) quickly. For the full project narrative see
[`README.md`](README.md); this file is the short structural index.

## What this project is

**SLeeLa** is a Java-like programming language that runs on a Turing-complete,
thread-friendly **C/C++ execution core**. **Nordshrift** is the `.sst`
transpiler driver layered on top; it drives a triplet of targets (Java, SLeeLa,
C). A SLeeLa source file (`.sleela`) is a **Wrapper™** — the program unit
governed by the Sleela Language Metadocument (SL-META-0001).

## Source of truth — where the canonical definition of each thing lives

| Topic | Canonical source |
|---|---|
| `.sst` sheet format (1.0) | [`SST.model`](SST.model) (NS-SST-0001) |
| `.sst` semantic layer (2.0) | [`SST-2.0.model`](SST-2.0.model) |
| Standard library inventory (`/lib`) | [`lib/LIBRARY.INDEX.md`](lib/LIBRARY.INDEX.md), [`lib/LIBRARY.SYMBOLS.md`](lib/LIBRARY.SYMBOLS.md) |
| Architecture overview | [`markdown/ARCHITECTURE.md`](markdown/ARCHITECTURE.md) |
| Compiler + library resolution | [`markdown/COMPILER.md`](markdown/COMPILER.md), [`markdown/COMPILER.RESOLUTION.md`](markdown/COMPILER.RESOLUTION.md) |
| Build system | [`markdown/BUILD.md`](markdown/BUILD.md), [`impl/BUILD-LAYOUT.md`](impl/BUILD-LAYOUT.md) |
| Native C/C++ design | [`impl/DESIGN.md`](impl/DESIGN.md) |
| Nordshrift internals | [`impl/nordshrift/NORDSHRIFT-COMPLETE.md`](impl/nordshrift/NORDSHRIFT-COMPLETE.md) |
| XML/DTD interchange form | [`markdown/XML.SLEELA.DEFINITION.md`](markdown/XML.SLEELA.DEFINITION.md), [`xml-moment/`](xml-moment/) |

## Top-level map

| Path | Contents |
|---|---|
| [`impl/`](impl/) | **The C/C++ implementation.** Execution core, compiler, Nordshrift, OS abstraction layer (`impl/core`, with POSIX + Win32 backends), subject libraries, native services. Build entry: `cd impl && make`. |
| [`lib/`](lib/) | **The SLeeLa standard library** — the authoritative language-facing `.sleela` source collection, discovered recursively. Each first-level directory is a package namespace. Inventory in `lib/LIBRARY.INDEX.md`. |
| [`markdown/`](markdown/) | Reference documentation (architecture, build, compiler, class/library inventories, subject domains, and more). |
| [`examples/`](examples/) | Worked examples: XML subject models, Nordshrift `.sst` sheets (`examples/nordshrift/`), symmetry builds. |
| [`xml-moment/`](xml-moment/) | XML + DTD interchange form for SLeeLa classes and SST/Nordshrift sheets (see the XML definition doc). |
| [`tools/`](tools/) | Generator and verification scripts (SHA-256 manifests, data generation, accuracy checks). |
| [`test-suites/`](test-suites/) / [`tests/`](tests/) | Test harnesses (C, C++, negative, coverage) and project tests. |
| [`scripts/`](scripts/) | Per-platform build/install helpers (`build-linux.sh`, `build-macos.sh`, …). |
| [`.github/`](.github/) | CI workflows (per-platform builds on every push/PR). |
| [`.kiro/`](.kiro/) | Kiro steering files (auto-loaded guidance for the AI assistant). |
| `impl/core` | OS-aware abstraction: threads, sockets, files, pipes, paths, terminal, dynamic libs, time. Runtime reports backend as `linux` / `macos` / `windows`. |

> This repository also contains many domain and content directories
> (e.g. subject models, HTTP server grades `http-1.0`…`http-9.0`, telephony,
> and policy/data documents). The table above lists the areas most relevant to
> building and extending the language and tooling; browse the tree or
> `README.md` for the rest.

## Build & test (quick start)

| Platform | Command |
|---|---|
| Linux | [`scripts/build-linux.sh`](scripts/build-linux.sh) or `cd impl && make` |
| macOS | [`scripts/build-macos.sh`](scripts/build-macos.sh) |
| Windows 10+ | `build-windows.ps1` (MinGW-w64) |

Tests: `cd impl && make test`. CI runs per-platform builds on every push and
pull request (see [`.github/workflows`](.github/workflows)).

## Branches

`main` and `master` are both maintained and are kept with equivalent content;
changes are applied to both.

## Conventions for AI assistants

- Treat the **source-of-truth** table above as authoritative; prefer the
  canonical spec over restating numbers or grammar from memory.
- The `/lib` inventory counts are verified against the filesystem by the
  library inventory test — when they change, update `lib/LIBRARY.INDEX.md` /
  `lib/LIBRARY.SYMBOLS.md` first and let dependent docs follow.
- Kiro-specific guidance lives in [`.kiro/steering/`](.kiro/steering/) and is
  loaded automatically; this file is the tool-agnostic companion.
