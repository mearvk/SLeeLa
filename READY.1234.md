# READY.1234 — Creating the SLeeLa Repository From Scratch

# SLeeLa
<p align="center"><img src="images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

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
specification, nothing else — no implementation, no `/lib`, no tooling yet. The
spec must be complete and self-consistent enough to compile against
idempotently; produce exactly these artifacts:

  **1a. `SLEELA.syntax`** — the normative language grammar, as a numbered
  Markdown spec with these sections in order (this exact structure):
   - Header block: Language=Sleela; **Syntax version 1.11** (baseline 1.3–1.10 +
     nested classes); Compiler **Sleelvac(TM) 0.3.29-dev** accepting syntax
     **1.3..1.11**; Surface = Java-like; Core = Turing-complete, thread-friendly
     C/C++ execution core; Companions = `SHEET.sheet`, `SST.model` (NS-SST-0001),
     `lib/LIBRARY.SYMBOLS.md`, `markdown/EXTENDS.md`. State the two distinct
     version numbers (per-file `#sleela MAJOR.MINOR` syntax version vs. the tool
     version) and that a file must declare a supported syntax version.
   - §1 Lexical grammar: UTF-8 source; `//` and `/* */` comments; **16 reserved
     keywords** = `class static void struct new int double boolean String if
     else while for return true false null print`; identifier rule
     `(letter|_)(letter|digit|_)*`; literals (64-bit int, double, bool, string
     with escapes `\n \t \r \\ \"`, null); operators `+ - * / %  == != < <= > >=
     && || !  =  ( ) { }  ; ,`.
   - §2 Program structure: `program ::= top-decl+`; top-decl = struct-decl |
     class-decl; execution begins at the single `void main()`.
   - §3 Types & declarations: scalar types `int double boolean String void`;
     unsigned `U<n>` (width 1..1048576) and character-set `C<n>` (capacity
     1..1048576) families; fields (shared, thread-safe globals, init in order at
     top of main); methods (all behave static; locals per-thread); §3.4 structs
     (syntax 1.2+): named aggregates, reference types, `new`, `.` access, limits
     256 types / 64 fields / 4096 live instances.
   - §4 Statements: block, var-decl, assign, field-assign, gc-hint (§13),
     if/else, while, for (with the optional 4th Boolean thread-stability guard
     and 5th Boolean heap-health guard), return, print, expr-stmt.
   - §5 Expressions: full precedence ladder lowest→highest (logic-or, logic-and,
     equality, comparison, additive, multiplicative, unary, primary); `+` is
     arithmetic or String concatenation (Java-style); `new`, member access.
   - §6 Conducted methods (address every `SHEET.sheet` object by string name):
     the **7** built-ins `conduct role insight congruent route sysdepth
     degreemax`, with `sysdepth()`→**3024** and `degreemax()`→**4**, and the
     congruence/route rules.
   - §7 Threading (bounded, ≤128 threads): the **6** built-ins `spawn join lock
     unlock send recv`; §7.1 struct transport `structPack`/`structUnpack`.
   - §8 The Constitution (invariants in force): system depth = **3024**,
     complexity degree max = **4** (1 direct·2 mediated·3 brokered·4 federated),
     the connect rule (n congruent-linear systems iff n ≤ 3024 and degree ≤ 4).
   - §9 A complete worked example that prints depth/degree, a role/insight, a
     route, and runs bounded concurrency with its expected output.
   - §10 Reserved-word & built-in SUMMARY with exact counts: keywords(16),
     conduct(7), threading(6), struct(2, 1.2+), munction(1+10: `Munction.start`
     + `.connect .enable .send .thatch .consume .observe .propagate .latch
     .closeWithReceipt .reception`, 1.3+), synchro(11: `synchroOpen
     synchroDispatch synchroReport synchroClose synchroSent synchroReceived
     synchroMean synchroMin synchroMax synchroP95 synchroLoss`, 1.3+),
     best-of(18: the `bestOf*` family, 1.3+), gc-hint(2: contextual `gc`/`mem`),
     system(1: contextual `ran`). State contextual built-ins are recognized in
     call position only.
   - §11 Library class vocabulary (informative): point to
     `lib/LIBRARY.SYMBOLS.md` as the single authoritative source; do not inline a
     second registry.
   - §12 Syntax 1.8: inferred locals (`let`) and constructor arguments on
     `new Type(args)` (and that overload resolution / `this(...)`/base delegation
     are explicitly out of scope).
   - §13 Syntax 1.9: the GC-hint statement `x = gc N;` / `x = mem N;` (N in
     0..100), its contextual (not reserved) nature, semantics/limits, and §13.4
     the `ran::` system namespace (`ran::mem`, `ran::gc`).
   - §14 Syntax 1.11: nested classes — the "rule of two" (lexical nesting depth
     2), the per-document limit of **42** nested classes, `Outer.Inner` naming
     (mangled `Outer__Inner`), and the version gate.

  **1b. `SHEET.sheet`** — the system-object catalog, machine-readable with its
  OWN parse contract at the top: `#key value` header directives (`#sheet`,
  `#kind system-object-catalog`, `#rev`, `#format`, `#machine yes`); `section
  <name>:` opens a category; an indented bare `Word` names an Object; under an
  Object, indented `children:`/`siblings:`/`note:` give relations (comma lists,
  `-` = none); section-level `key: value` (e.g. depth, complexity-degree-max).
  It must define the **16 conduct roles** exactly: root, start, idiom, highlight,
  call, method, attention, gain, conversation, piping, transitive, completitive,
  inheritance, smart-move, congruence, limit — and populate representative
  objects (System as root, Pipeline as piping, etc.). The same file is parsed by
  both the Sleela conduct registry and the Nordshrift object-compatibility list.

  **1c. `SST.model`** — the Nordshrift `.sst` (Scripting Sheet) format, normative
  edition **NS-SST-0001**: preamble/purpose (Nordshrift is the Sleela→Java
  transpiler driver; `.sleela` files are 'Wrapper™' units), scope, the normative
  keyword rules (must/shall/required/forbidden normative; should/may
  non-normative), lexical conventions, block/section grammar, every directive +
  operands + semantics, import/composition, profile resolution/override, and the
  Nordshrift error codes.

  Also add a skeleton `README.md` and `ORGANIZATION.md`. Write NO code. Keep each
  file single-purpose and internally consistent (every count, built-in name, and
  cross-reference must agree across the three files). Stop and show me the spec
  for review."

> Why #1 is this detailed: the real `SLEELA.syntax` is a ~790-line, 14-section
> normative document with exact keyword/built-in counts and versioned features;
> `SHEET.sheet` is a machine-parsed catalog with a fixed 16-role contract; and
> `SST.model` is the normative NS-SST-0001 format. A vague "write a Java-like
> grammar" prompt would produce a plausible-but-different language, not THIS one.
> The spec is the single source every later stage compiles against, so it must be
> pinned precisely enough to regenerate idempotently.

**2.** "Create `impl/core/` — the C execution engine (C11). It defines the
bytecode ISA and the VM; everything above compiles to it. Produce:
   - `sleela_core.h` — the value model (`SLValue` tagged union: int/double/
     bool/string/null/struct/array handles), the **canonical bytecode ISA**
     (the opcode enum; the design target is the **124 canonical opcodes** the
     lib `SLSleelaVM` references — the enum is strictly **APPEND-ONLY** so
     serialized-artifact opcode numbers never shift), and the bounded VM-local
     handle-table sizes (`SL_SYNCHRO_MAX`, `SL_BESTOF_MAX`, `SL_MUNCTION_MAX`,
     `SL_AI_MAX`, `SL_CAP_MAX`, …) — a program never sees a raw pointer.
   - `sleela_core.c` — the fetch-decode-execute loop, the global/stack machine,
     struct & array stores, and the per-family opcode handlers that call the
     native bridges.
   - the native service bridges, each as its own small `.c/.h` pair:
     `sleela_thread` (≤128 threads, lock table, 2-tuple mailbox),
     `sleela_io`, `sleela_net`, `sleela_time`, `sleela_path`, `sleela_os`,
     `sleela_memory` + `sleela_memmgr` (+ grade2/grade3; 2 GiB default,
     256 MiB–2 GiB range), `sleela_synchro`, `sleela_munction`,
     `sleela_bestof`, `sleela_library`, `sleela_platform` (Win32/POSIX/Apple),
     `sleela_terminal`, plus the `*_smoke.c` self-tests per family.
   Enforce the Constitution from `SLEELA.syntax`: `sysdepth()`=3024,
   `degreemax()`=4. Favor many small cohesive units over one large file.
   Checkpoint: `impl/core` compiles standalone and each `*_smoke` self-test
   links and passes."

**3.** "Create `impl/frontend/` (C++17), each a focused unit, in this internal
order so each depends only on the ones before it:
   - `lexer.{h,cpp}` — tokens for the 16 keywords and all literals/operators,
     plus the **contextual built-in registry** (conduct/threading/struct/
     munction/synchro/best-of/gc-hint/ran names recognized only in call
     position, per §10 of the spec).
   - `parser.{h,cpp}` + `ast.h` — the grammar's productions → AST.
   - `semantic.{h,cpp}` — type/known-name resolution, the `builtin()` set
     (the authoritative list of recognized built-in call names), the
     `#sleela` syntax-version gate (`version.{h,cpp}`, accept 1.3..1.11), and
     the §12/§13/§14 feature checks (`let` inference, gc-hint band 0..100,
     nested-class rule-of-two / 42-per-doc).
   - `compiler.{h,cpp}` — codegen: AST → the `impl/core` opcodes, with per-call
     ARITY CHECKS; emit the persistent `.sleela` Core artifact.
   - `artifact.{h,cpp}` (serialize/validate artifacts), `library_index.{h,cpp}`
     (resolve `<package>.<ClassName>` against `/lib`), `unsigned_integer.h`
     (U<n>/C<n>), the Java-equivalence/flow/exception helpers, and `driver.cpp`
     (the `sleela` CLI: `check`/`run`/`compile -o`/`validate-artifact`/
     `version`/`nordshrift`).
   Add `impl/Makefile` so it links `impl/build/sleela` and `impl/build/nordshrift`.
   Checkpoint: `impl/build/sleela version` prints `Sleelvac(TM) 0.3.29-dev`, and
   `class X { void main(){ print(\"ok\"); } }` compiles and runs via
   `sleela run`.

   RULE for every native built-in family added later (the 5-touch sequence,
   applied verbatim for bestOf*, synchro*, ideaCanvas*, …): (a) names →
   `lexer.cpp`; (b) names → the `builtin()` set in `semantic.cpp`; (c) codegen +
   arity checks → **appended** opcodes in `compiler.cpp`; (d) define the opcodes
   (APPEND) + any handle table in `sleela_core.h`; (e) execute them in
   `sleela_core.c` via a native bridge. Then regenerate the manifest (step 4).
   Never renumber existing opcodes."

**4.** "Create the integrity gate now that real source exists under
`impl/core` and `impl/frontend`. Produce:
   - `tools/generate-sha256-manifest.py` — scans the source dirs the verified
     build compiles (`impl/core`, `impl/frontend`, `impl/nordshrift`,
     `impl/catalog`, `impl/xclass`, `impl/subjects`) plus an explicit
     extra-files list (e.g. `Parameters.c`, the runtime/*, `bash/pixel_terminal.*`,
     and any compiled `idea/*` sources added later), and writes
     `security/sha256-manifest.json`. Support `--root`, `--output`, `--stdout`.
   - `tools/verify-before-execution.py` — fails closed unless every listed file
     matches its recorded SHA-256.
   - wire `verify-security` into `impl/Makefile` as the FIRST prerequisite of
     `all` (so `all: verify-security $(SLEELA_BIN) $(NORDSHRIFT_BIN) stage`),
     reading the manifest from `SLEELA_SHA256_MANIFEST`/`VERIFY_MANIFEST`
     (default `../security/sha256-manifest.json`).
   - `security/BUILD-VERIFICATION.md` + `SHA256-VERIFICATION.md` documenting the
     rule: **regenerate the manifest whenever any gated source is added or
     changed** — including after every step-3 built-in addition and every
     step-7 module.
   Checkpoint:
   `SLEELA_SHA256_MANIFEST=$PWD/security/sha256-manifest.json make -C impl verify-security`
   passes; editing a gated file then rebuilding FAILS until the manifest is
   regenerated with `python3 tools/generate-sha256-manifest.py`."

**5.** "Create the standard library under `lib/`. FIRST author
`lib/LIBRARY.SYMBOLS.md` as the single authoritative vocabulary (the manifest of
every `<package>.<ClassName>` symbol; it is generated by
`tools/generate-library-symbols.py` and is the only registry — the spec's §11
appendix merely mirrors it). THEN create the package families as **one class per
`.sleela` file** (syntax `#sleela 1.x`, `class <Name> { … }`), each resolvable
through the shared `library::Index` the frontend built in step 3. Author in
dependency order — foundational first:
   - foundational: `core`, `collections`, `data-structures`, `io`, `text`,
     `math`, `reflection`, `runtime`, `os`, `process`, `memory`;
   - then: `crypto`, `net`, `http` (+ `http-1.0..9.0`, `http-servers`),
     `filesystem`, `database`, `regex`, `config`, `logger`, `connector`, `rmi`,
     `synchro`, `munction`, `codecs`, `audio`, `video`;
   - then higher-level families: `user-interface`/`ui`/`gui`, `cpu`, `vm`,
     `compiler`, `decompiler`, `debugger`, `languages`, `deliberation`,
     `inference-model`, and the domain packages.
   Target ~**93 package families**. Do NOT create a second registry; every class
   must appear in `lib/LIBRARY.SYMBOLS.md`. Checkpoint:
   `python3 tools/sleela-build.py inventory-lib` reports the source/package
   counts with no duplicate-symbol surprises, and representative classes pass
   `impl/build/sleela check`."

**6.** "Create example programs under `impl/examples/` that exercise the grammar
and every native built-in family, one concern per file — at minimum: `hello`,
`fizzbuzz`, `factorial`, `fibonacci` (control flow); `struct_basic`,
`struct_network` (structs + transport); `threads` (spawn/join/lock/send/recv);
`conduct` (conducted methods against `SHEET.sheet`); `synchro_measure`,
`munction_reach`, `bestof_route` (measurement/reach/route built-ins);
`versioned`, `nested_classes`, `static-protected` (version features); `time`,
`fileio`, `fileio-fifo`, `network_echo` (os/io/net); plus the subject demos
(`chemistry_presumed`, `financial`, `stock_market*`). Add the compiler-
construction teaching ladder under `lib/compiler/examples/` (Novice/Mid/Senior/
Very-Senior, fail-closed gates). Checkpoint: `impl/build/sleela run <file>`
executes each example end to end under the SHA-256 gate; `make -C impl test-sleela`
runs them all."

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

**8.** "Create packaging, platform scripts, and steering:
   - `build-linux.sh` — `cd impl && make clean && make all`, honoring
     `CC`/`CXX`/`BIN_DIR` (default `/bin`), installing `sleela` + `nordshrift`.
   - `build-windows.ps1` — resolve `mingw32-make`/`make`, then
     `make CC=$Cc CXX=$Cxx all` (delegates to the same `impl/Makefile`; Windows
     thus selects the WGL context automatically).
   - `impl/BUILD-LAYOUT.md` — the staged runtime layout under
     `impl/build/SLeeLa/` (`bin/`, `Config/sleela.conf` with the 2 GiB default /
     256 MiB–2 GiB range, `Options/`, `lib/`, `docs/`, `README.md`).
   - `tools/sleela-build.py` — the lifecycle driver with EXACTLY these
     subcommands: `init check inventory-lib build test compile <src> <out>
     package install <dir> clean doctor version run`.
   - the root `Makefile` dispatcher whose `all` builds every subsystem via
     `$(MAKE) -C <dir>`: `core`(→impl) `java28 regex compiler decompiler vm
     scripting jetbrains install tutorial-check tests config route common-rails
     codecs churn antivirus`, plus a `clean` and a `help` target.
   - `.kiro/steering/` notes (the toolchain note: binaries are `sleela` +
     `nordshrift`, NO separate `sleelac`; and a build-status note).
   Checkpoint: on a clean checkout, `make all` builds the whole repo with the
   SHA-256 gate passing, and `make help` lists every dispatcher target."

**9.** "Finalize and prove idempotency:
   - `export SLEELA_SHA256_MANIFEST=$PWD/security/sha256-manifest.json`
   - `python3 tools/generate-sha256-manifest.py` (ensure the manifest covers all
     gated source, including every `idea/*` compiled file).
   - `make all` and confirm `make -C impl verify-security` passes.
   - `make -C impl test-sleela` (run every example) and
     `python3 tools/sleela-build.py inventory-lib` (library coverage).
   - build the optional natives: `make -C idea run`,
     `make -C idea/sleela-native run`.
   - regenerate the digest docs (`tools/generate-all-sha256-md.sh` /
     `generate-important-sha256-manifest.py`) and write/refresh `READY.1234.md`
     so it reflects the repository as built.
   - commit. Re-running steps 1–9 against this spec must reproduce the same
     repository (idempotent): same keywords/built-ins/opcode ISA, same 16 conduct
     roles, same library symbol set, same gate."

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
