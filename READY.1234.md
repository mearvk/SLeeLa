# READY.1234 — Creating the SLeeLa Repository From Scratch

*A blueprint for an AI agent (Kiro, Claude, ChatGPT) that must author this
repository from nothing, in the most efficient, correctly-ordered, and careful
way — so every layer exists before the layer that depends on it.*

> This is **not** a build-order (compile) guide. It is the **creation order**:
> the sequence in which to *write the artifacts into existence* so dependencies
> are never dangling and the repo stays coherent at every step. Each stage ends
> in a checkpoint you can verify before moving on.

---

## 0. Read this first — principles

1. **Specification precedes implementation.** Nothing is written until the thing
   it must conform to exists. The grammar and the object catalog come before any
   compiler code; the compiler comes before any `.sleela` program.
2. **One source of truth per concept.** The grammar lives in `SLEELA.syntax`; the
   object catalog in `SHEET.sheet`; the library vocabulary in
   `lib/LIBRARY.SYMBOLS.md`. Never create a second registry for the same thing.
3. **Author in dependency order, verify at each checkpoint.** Do not advance to a
   stage until the previous stage's checkpoint passes.
4. **Append, never renumber.** Opcodes, manifest entries, and catalog IDs are
   referenced by position/value elsewhere — add at the end; do not reorder.
5. **The security gate is part of the build, not an afterthought.** Any file the
   verified build consumes must be recorded in the SHA-256 manifest the moment it
   is created or changed.
6. **State honesty about what is verified.** Mark anything that cannot be built or
   run in the current environment (e.g. GPU/OS-specific code) as written-but-
   unverified; never imply otherwise.

Legend for each stage: **Create → Why now → Depends on → Checkpoint.**

---

## 1. Normative specification (the contracts)

**Create**
- `SLEELA.syntax` — the language grammar (lexical rules, types, statements,
  expressions, conducted methods, threading, the Constitution invariants).
- `SHEET.sheet` — the system-object catalog the conducted methods address.
- `SST.model` — the subject/structure model companion.
- `ORGANIZATION.md`, `README.md` (skeleton), top-level intent docs.

**Why now** Every later layer (frontend, core, library, programs) is defined
*against* these. Writing code first would mean coding to an unwritten contract.

**Depends on** Nothing. This is the root.

**Checkpoint** The syntax spec names its companions and version range; the
catalog and model exist and are internally consistent. No code yet.

---

## 2. The execution core (`impl/core/`)

**Create** the C execution engine: value model, bytecode opcodes
(`sleela_core.h/.c`), the VM loop, and the native service bridges it owns
(threads, io, net, time, memory manager, synchro, best-of, os). Keep the opcode
enum **append-only**.

**Why now** The core defines the opcodes and runtime the compiler will target.
The frontend emits for this engine, so the engine's instruction set must exist
first.

**Depends on** §1 (the grammar's semantics and the Constitution values the core
enforces, e.g. system depth 3024, degree ≤ 4).

**Checkpoint** The core compiles standalone; its opcode set and handle-table
discipline (bounded VM-local handles for every resource) are defined.

---

## 3. The front end (`impl/frontend/`)

**Create**, in this internal order:
1. `lexer.*` — tokens, including the contextual built-in registry.
2. `parser.*` — the grammar's productions → AST (`ast.h`).
3. `semantic.*` — type/known-name checks, the `builtin()` set, version gate.
4. `compiler.*` — codegen: AST → the core's opcodes (arity-checked).
5. `version.*`, `artifact.*`, `library_index.*`, driver glue.

**Why now** The front end turns source conforming to §1 into opcodes defined in
§2. It cannot exist before either.

**Depends on** §1 (grammar) and §2 (opcodes to emit).

**Checkpoint** `impl/` links `sleela` and `nordshrift`; `sleela version` runs.
A trivial `class X { void main(){ print("ok"); } }` compiles and runs.

> **Rule for adding a native built-in family later** (the careful 5-touch
> sequence, learned from `bestOf*`/`ideaCanvas*`): (a) add names to
> `lexer.cpp`; (b) add to the `builtin()` set in `semantic.cpp`; (c) codegen with
> arity checks → new **appended** opcodes in `compiler.cpp`; (d) define the
> opcodes (append) + any VM handle table in `sleela_core.h`; (e) execute them in
> `sleela_core.c`, bottoming out in a native bridge. Then regenerate the manifest
> (§5). Never renumber existing opcodes.

---

## 4. The standard library (`lib/`)

**Create** `lib/LIBRARY.SYMBOLS.md` (the authoritative vocabulary) **before** the
classes, then the package families (`core`, `collections`, `io`, `crypto`,
`http`, `user-interface`, `cpu`, …) as `.sleela` class-per-file units, each
resolvable as `<package>.<ClassName>` through the shared `library::Index`.

**Why now** Library classes are *written in Sleela* and resolved by the index the
front end (§3) builds. The compiler must exist to validate them; the symbol
manifest must exist so the index has a source of truth.

**Depends on** §3 (to compile/validate) and §1 (grammar they're written in).

**Checkpoint** The library inventory tool reports source/package counts with no
duplicate-symbol surprises; representative classes pass `sleela check`.

---

## 5. The security / integrity gate (`security/`, `tools/`)

**Create** `tools/generate-sha256-manifest.py` (records every source the verified
build consumes), `tools/verify-before-execution.py`, and the generated
`security/sha256-manifest.json`; wire `verify-security` into `impl/Makefile`.

**Why now** As soon as there is real source that the build compiles (§2–§4), the
integrity gate must cover it. Create the gate once source exists, and **regenerate
the manifest whenever gated source is added or changed** — including every time a
later stage (§3 built-ins, §7 modules) adds files.

**Depends on** §2–§4 (the files it hashes); the Makefile target in §3.

**Checkpoint** `make -C impl verify-security` passes with
`SLEELA_SHA256_MANIFEST` pointing at the committed manifest; editing any gated
file and rebuilding fails until the manifest is regenerated.

---

## 6. Example programs & self-hosting surface (`impl/examples/`, `examples/`)

**Create** example `.sleela` programs that exercise the language and each
built-in family, plus the compiler-construction teaching ladder
(`lib/compiler/examples/`).

**Why now** Programs are the top of the dependency stack: they need the grammar
(§1), a working compiler (§3), the library (§4), and should run under the gate
(§5).

**Depends on** §1, §3, §4, §5.

**Checkpoint** `sleela run <example>` executes each example end to end.

---

## 7. Optional native modules — the Idea System(TM) subtree (`idea/`)

Author these **last among code**, because they *extend* a working VM; a healthy
`sleela` (§3–§6) must exist first. Create in this internal order:

1. **`idea/` core** — the C ABI + C++ facade for the signal-distributed Idea
   System (System/State model, hex-payload signal input, forward Series). Pure,
   no graphics. *Verifiable anywhere.*
2. **`lib/idea/*.sleela`** — the Sleela runnables (the System, the Canvas), which
   depend on §4 UI classes and §3 built-ins.
3. **`idea/realtime/`** — the native-CPU render/physics canvas: OpenMP physics
   core (verifiable), optional offscreen GPU compute, and the OS-conditional GL
   **context layer** (WGL/CGL/EGL — open standards). GPU/GL paths are
   write-but-unverified without a GPU/display.
4. **VM wiring** — register the `ideaCanvas*` built-ins via the §3 5-touch
   sequence so a runnable can call the module; link the module into `sleela`
   (opt-in or default per policy), then regenerate the manifest (§5).
5. **`idea/sleela-native/`** — the clean-room original renderer/model/audio
   (own math, own GL loader, own GLSL, own PCM mixer). Ships `PROVENANCE.md`
   (MIT, originality statement, standards-vs-copied-code table) **in the same
   commit as the code**.

**Why now** Every item here consumes a finished layer below it. Building them
earlier would couple the core to features it does not need.

**Depends on** §2 (opcodes/handles), §3 (built-in wiring), §4 (`lib/idea` UI),
§5 (manifest), and the open graphics standards (OpenGL/EGL/WGL/CGL) — never a
third-party engine/library.

**Checkpoint** The CPU-verifiable parts build and run (physics ripple; audio
WAV); GPU/OS code compiles and is clearly marked unverified where no device
exists; `PROVENANCE.md` present.

---

## 8. Tooling, packaging, platform builds, steering

**Create** `build-linux.sh`, `build-windows.ps1` (delegating to `impl/Makefile`),
the staged runtime layout (`impl/BUILD-LAYOUT.md`), docs, and `.kiro/steering/`
notes (e.g. the toolchain note, build status).

**Why now** Packaging and platform scripts wrap a product that already builds and
verifies; steering captures the conventions a future agent should follow.

**Depends on** all prior stages.

**Checkpoint** A clean checkout builds via the documented command on each target
OS; steering accurately describes the toolchain and gate.

---

## Creation order, in one line

```
1 spec (SLEELA.syntax, SHEET.sheet, SST.model)
  → 2 core (opcodes, VM, native bridges)
  → 3 frontend (lexer→parser→semantic→compiler)
  → 4 lib (LIBRARY.SYMBOLS.md → class families)
  → 5 security gate (manifest + verify, kept in sync forever after)
  → 6 examples/programs
  → 7 optional native modules (idea/ core → lib/idea → realtime → VM wiring → sleela-native)
  → 8 tooling / platform / steering
```

## Invariants to hold at every stage

- Conform to `SLEELA.syntax`; address only catalogued `SHEET.sheet` objects.
- Honor the Constitution: system depth **3024**, complexity degree **≤ 4**.
- Opcodes, manifest entries, catalog IDs: **append-only**.
- Any new gated source → **regenerate the SHA-256 manifest** before claiming a
  verified build.
- Prefer **open standards** and **own code** over third-party libraries; when a
  module is original, commit its `PROVENANCE.md` with it.
- Mark unverifiable (GPU/OS-specific) code honestly; never imply it was run when
  it was only compiled.

---
*Idea System(TM) / Idea(TM). From Author; distributive. This document describes
how to recreate the repository carefully from scratch; it is guidance for AI
agents and maintainers, not a normative language contract.*
