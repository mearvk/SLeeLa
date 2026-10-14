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

## Build terms (so Kiro understands how each stage is actually built)

These are the **real commands** found in the repository, mapped onto the stages
above. Run them from the **repository root** unless noted. The SHA-256 gate is
mandatory: set `SLEELA_SHA256_MANIFEST` to the committed manifest first.

```sh
# One-time per shell: point the verified-build gate at the trusted manifest.
export SLEELA_SHA256_MANIFEST="$PWD/security/sha256-manifest.json"
```

### Whole repo (dispatcher) — root `Makefile`
The root Makefile is a dispatcher that delegates to each subdir with `$(MAKE) -C`.
```sh
make all          # core + java28 + regex + compiler + decompiler + vm +
                  # scripting + jetbrains + install + tutorial-check + tests +
                  # config + route + common-rails + codecs + churn + antivirus
make core         # -> $(MAKE) -C impl all      (the native toolchain; stages 2-3)
make compiler     # -> $(MAKE) -C lib/compiler all   (compiler library, stage 4)
make decompiler   # -> $(MAKE) -C lib/decompiler all
make vm           # -> $(MAKE) -C lib/vm all
make tests        # -> $(MAKE) -C tests check
make clean        # clean all subdirs
make help         # list every dispatcher target
```

### Stage 2-3 — the native toolchain (`impl/`) and the SHA-256 gate (§5)
```sh
make -C impl all              # verify-security -> sleela + nordshrift -> stage
# 'all' first runs the gate target:
make -C impl verify-security  # fails closed unless every gated file matches
                              # security/sha256-manifest.json (via
                              # tools/verify-before-execution.py)
# Artifacts: impl/build/sleela, impl/build/nordshrift,
#            staged runtime under impl/build/SLeeLa/ (see impl/BUILD-LAYOUT.md)
make -C impl clean
```
Regenerate the manifest **whenever gated source changes** (new/edited files in
`impl/core`, `impl/frontend`, the compiled `idea/*` sources, etc.):
```sh
python3 tools/generate-sha256-manifest.py        # rewrites security/sha256-manifest.json
```

### Cross-platform convenience scripts (stage 8)
```sh
./build-linux.sh                       # cd impl && make clean && make all; installs to /bin
CC=clang CXX=clang++ ./build-linux.sh  # choose compilers
BIN_DIR="$HOME/bin" ./build-linux.sh   # choose install dir
pwsh ./build-windows.ps1               # Windows 10+: finds mingw32-make/make, runs 'make ... all'
```

### Stage 4 & 6 — compiling / running `.sleela` (the toolchain CLI)
```sh
# Direct CLI (after impl is built):
impl/build/sleela version
impl/build/sleela check   path/to/Program.sleela                 # validate only
impl/build/sleela run     path/to/Program.sleela                 # compile + execute
impl/build/sleela compile path/to/Program.sleela -o build/Program.sleela
impl/build/sleela validate-artifact build/Program.sleela
impl/build/sleela nordshrift [--emit] [--target=sleela|java|c] file.sleela

# Lifecycle driver (thin, auditable wrapper; runs the /lib inventory + ISA gate
# before delegating to the native compiler) -- tools/sleela-build.py:
python3 tools/sleela-build.py build                  # build the native compiler
python3 tools/sleela-build.py inventory-lib          # /lib source + ISA coverage report
python3 tools/sleela-build.py compile <src> <out>    # gated compile of one program
python3 tools/sleela-build.py check | test | package | install <dir> | doctor | version
python3 tools/sleela-build.py run -- <args...>
```

### Stage 7 — optional Idea System(TM) native modules (`idea/`)
Each subtree has its own Makefile; the GPU/GL paths are opt-in and need a
GPU/driver/display (write-but-unverified without one).
```sh
# Idea System core (C ABI + C++ facade/driver) -- pure, no graphics:
make -C idea run        # build + run the reference driver (build/idea-system)
make -C idea lib        # just the static C core (build/libideasystem.a)

# Realtime canvas (native-CPU physics; optional offscreen GPU + GL context):
make -C idea/realtime run                      # CPU physics benchmark (verifiable)
cmake -S idea/realtime -B idea/realtime/build -DIDEA_CANVAS_WITH_GL=ON   # + GPU/GL
cmake --build idea/realtime/build
# The VM links the native canvas by default (-fopenmp); to build GPU-less:
make -C impl IDEA_CANVAS_OMP= all

# Sleela-Native clean-room renderer/model/audio (MIT; own code, no 3rd-party):
make -C idea/sleela-native run     # headless model + audio demo -> a .wav (verifiable)
# The OpenGL renderer needs a GL context (WGL/CGL/EGL) + GPU; see its README.
```

### The canonical from-scratch sequence, as commands
```sh
export SLEELA_SHA256_MANIFEST="$PWD/security/sha256-manifest.json"
python3 tools/generate-sha256-manifest.py     # (only if gated source changed)
make -C impl all                              # gate -> sleela + nordshrift (stages 2-3,5)
make compiler decompiler vm                   # library/tooling (stage 4)
impl/build/sleela run impl/examples/<example>.sleela   # prove it (stage 6)
make -C idea run && make -C idea/sleela-native run     # optional natives (stage 7)
make all                                      # everything the dispatcher knows (stage 8)
```

## Literal prompts to drive an agent (Kiro / Claude / ChatGPT)

Paste these **in order, one at a time**, into Kiro (or Claude/ChatGPT). Each
prompt tells the agent to create one layer; the ordering guarantees every folder
and module exists before whatever depends on it. Do **not** skip ahead — wait for
each step's checkpoint to pass before giving the next prompt. Each step also asks
the agent to *define the module for optimal efficiency* (minimal, cohesive,
dependency-correct) rather than dump a monolith.

> Replace nothing but the bracketed hints. Keep the invariants at the bottom of
> this document in force for every step.

**1.** "Create a new repository named `SLeeLa`. First author ONLY the normative
specification, nothing else: `SLEELA.syntax` (the Java-like grammar: lexing,
types, statements, expressions, conducted methods, threading, and the
Constitution invariants system-depth 3024 and complexity-degree ≤ 4),
`SHEET.sheet` (the system-object catalog the conducted methods address), and
`SST.model`. Add a skeleton `README.md` and `ORGANIZATION.md`. Write no code yet.
Keep each file single-purpose. Stop and show me the spec for review."

**2.** "Create `impl/core/` — the C execution engine. Define `sleela_core.h`
(the bytecode opcode enum — APPEND-ONLY — plus the value model and bounded
VM-local handle tables) and `sleela_core.c` (the VM loop), then the native
service bridges it owns, each as its own small file: threads, io, net, time,
memory manager, synchro, best-of, os. Conform to the Constitution values from
`SLEELA.syntax`. Make it compile standalone. Favor small cohesive translation
units over one large file. Checkpoint: the core compiles."

**3.** "Create `impl/frontend/` in this internal order, each a focused unit:
`lexer.*` (tokens + the contextual built-in registry), `parser.*` (grammar →
AST in `ast.h`), `semantic.*` (type/known-name checks, the `builtin()` set, the
syntax-version gate), `compiler.*` (codegen: AST → the opcodes defined in
`impl/core`, with arity checks), then `version.*`, `artifact.*`,
`library_index.*`, and the driver. Add the top-level build so `impl/` links
`sleela` and `nordshrift`. Checkpoint: `impl/build/sleela version` runs and a
trivial `class X { void main(){ print(\"ok\"); } }` compiles and runs."

**4.** "Create the integrity gate now that real source exists: write
`tools/generate-sha256-manifest.py` (records every file the verified build
compiles) and `tools/verify-before-execution.py`, generate
`security/sha256-manifest.json`, and wire a `verify-security` target into
`impl/Makefile` that fails closed unless every gated file matches. Document the
rule: regenerate the manifest whenever gated source changes. Checkpoint:
`SLEELA_SHA256_MANIFEST=$PWD/security/sha256-manifest.json make -C impl verify-security`
passes, and editing a gated file then rebuilding fails until regenerated."

**5.** "Create the standard library under `lib/`. First author
`lib/LIBRARY.SYMBOLS.md` as the single authoritative vocabulary, THEN the
package families as one class per `.sleela` file, each resolvable as
`<package>.<ClassName>` through the shared library index: start with `core`,
`collections`, `io`, then `crypto`, `http`, `user-interface`, `cpu`, and the
rest. Do not create a second registry. Checkpoint: the `/lib` inventory reports
clean counts and representative classes pass `impl/build/sleela check`."

**6.** "Create example programs under `impl/examples/` and `examples/` that
exercise the grammar and each native built-in family, plus the
compiler-construction teaching ladder under `lib/compiler/examples/`. Checkpoint:
`impl/build/sleela run <each example>` executes end to end under the gate."

**7.** "Create the optional Idea System(TM) native modules under `idea/`, in this
internal order so each extends a finished layer: (a) `idea/` core — the C ABI +
C++ facade for the signal-distributed Idea System (System/State model, hex-payload
signal input, forward Series), pure, no graphics; (b) `lib/idea/*.sleela` — the
Sleela runnables that use the `lib/user-interface` classes and the built-ins; (c)
`idea/realtime/` — the native-CPU OpenMP physics core, an optional offscreen GPU
compute backend, and an OS-conditional GL context layer (WGL on Windows 10+, CGL
on macOS, EGL on Linux — open standards only); (d) wire the `ideaCanvas*` native
built-ins into the VM using the 5-touch sequence (lexer → semantic → compiler →
appended opcode → core+bridge), link the module into `sleela`, and regenerate the
manifest; (e) `idea/sleela-native/` — a clean-room ORIGINAL renderer/model/audio
(own math, own GL loader replacing GLEW/GLAD, own GLSL, own PCM mixer) with a
`PROVENANCE.md` (MIT, originality statement, standards-vs-copied-code table)
committed in the same change. Mark any GPU/OS-specific code that cannot be run
here as written-but-unverified. Checkpoint: the CPU-verifiable parts build and
run; GPU/OS code compiles."

**8.** "Create the packaging, platform scripts, and steering: `build-linux.sh`
and `build-windows.ps1` (both delegating to `impl/Makefile`), the staged runtime
layout (`impl/BUILD-LAYOUT.md`), the `tools/sleela-build.py` lifecycle driver
(subcommands init/check/inventory-lib/build/test/compile/package/install/clean/
doctor/version/run), the root `Makefile` dispatcher that builds every subdir with
`$(MAKE) -C`, and `.kiro/steering/` notes (toolchain + build status). Checkpoint:
`make all` builds the whole repo on a clean checkout and the gate passes."

**9.** "Finalize: run `make all`, confirm `verify-security` passes, run every
example once, and write/refresh `READY.1234.md` so it reflects the repository as
built. Commit."

> **Efficiency note the agent must honor:** at each step, define the module as the
> *smallest cohesive set of files that satisfies that layer's checkpoint* — no
> speculative files, no forward dependencies, no duplicated registries. If a later
> step reveals a cleaner factoring, refactor that module in place rather than
> bolting on. This keeps the dependency graph a clean DAG from stage 1 to 9.

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
