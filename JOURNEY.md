# JOURNEY.md — the definition of every component in SLeeLa

This document defines **every component** of the SLeeLa implementation as the
**journey** a program takes: from a `.sleela` **Wrapper™** on disk, through the
front end and into the C execution core, alongside the runtime services and
subject libraries, out to the operating system, and — for whole projects —
around through the Nordshrift `.sst` driver.

Each component below has a one-line **definition**, its **location** in the
tree, and its **stop on the journey**. The runnable roll-call of these
components lives in [`journey/`](journey/): a set of `.sleela` **register**
documents that name each component and, where it is catalogued, resolve it
against `SHEET.sheet` at compile time.

> Authoritative sources: the source tree under [`impl/`](impl/),
> [`SUMMARY.md`](SUMMARY.md) (the repository's self-accounting), and
> [`ARCHITECTURE.md`](ARCHITECTURE.md) (which tree is which). Where this file
> and the source disagree, the source wins.

---

## The journey at a glance

```
   .sleela source (Wrapper™)
        │
        ▼
  ┌─────────────────────────────  FRONT END (C++)  ─────────────────────────┐
  │  version → lexer → parser → AST → (native lowering) → compiler           │
  └──────────────────────────────────────────────────────────────────────────┘
        │  emits opcodes + constants via the stable exchange API
        ▼
  ┌─────────────────────────────  EXECUTION CORE (C)  ─────────────────────┐
  │  stack VM · slcore_exchange() · threads · sockets · files · structs      │
  │  served by RUNTIME (GC, security supervisor, parameters)                 │
  │  standing on the OS-ABSTRACTION layer (thread/net/io/path/terminal/…)    │
  └──────────────────────────────────────────────────────────────────────────┘
        ▲                                   │
        │ conducted methods (SHEET.sheet)   ▼
   CATALOG                            persisted .sleela ARTIFACT

  NORDSHRIFT (.sst driver) sits above the whole stack and drives the
  triplet of targets — Java · Sleela · C — reusing the same core path.
```

---

## Tier 0 — The program and its filetypes

| Component | Definition | Where |
|---|---|---|
| **Wrapper™ (`.sleela`)** | The Sleela source file — the program unit that carries the metadocument addend (SL-META-0001). "A `.sleela` file", "a Sleela source file", and "a Wrapper™" all name the same thing. | `*.sleela` |
| **Artifact (`.sleela` compiled)** | A persisted, runnable compiled program: opcodes + constants written to disk and reloadable without recompiling. | `impl/core/sleela_artifact.c`, `impl/frontend/artifact.{h,cpp}` |
| **Control sheet (`.sst`)** | A Nordshrift control sheet (NS-SST-0001) that names Wrapper™ inputs and a target of the triplet. | `SST.model`, `SST-2.0.model` |
| **Object catalog (`.sheet`)** | The catalog of common system objects that backs *conducted methods* and Nordshrift's object-compatibility list. | `SHEET.sheet` |
| **Metadocument (`.manifest`)** | SL-META-0001, the constitutional document governing the language. | `src/Sleela.manifest` |
| **Ingest input (`.xclass`)** | A SecureJDK 28 XML class file the CLI can turn into a Sleela program. | `impl/xclass/` |

## Tier 1 — The front end (C++): source becomes bytecode

| Component | Definition | Stop on the journey |
|---|---|---|
| **Version resolver** | Resolves and enforces the `#sleela MAJOR.MINOR` pragma *before* parsing; holds the supported syntax range. | The gate: too-new or malformed versions are rejected here. |
| **Lexer** | The Java-like tokenizer: turns source text into a token stream; strips comments (including SleelaDoc). | Text → tokens. |
| **Parser** | Recursive-descent parser: builds the AST (classes, structs, fields, methods, statements, precedence-climbing expressions). | Tokens → AST. |
| **AST** | The abstract syntax tree node definitions — the shape of a parsed program. | The in-memory program. |
| **Native lowering** | Rewrites qualified subject calls (e.g. `math.sqrt`) into synthesized native methods, validating imports and dependencies. | AST → AST (enriched). |
| **Compiler** | Lowers the AST to core bytecode: declares structs/globals, computes locals, emits methods, maps built-ins to opcodes with per-feature version gating. | AST → opcodes. |
| **Driver (`sleela` CLI, "Sleelvac™")** | The command surface: `compile`, `run`, `check`, `xclass`, `version`, `defender`. Drives the whole front-end path and the SHA-256 execution gate. | Where a person enters the journey. |

Location: [`impl/frontend/`](impl/frontend/) — `version`, `lexer`, `parser`,
`ast.h`, `compiler`, `driver.cpp`.

## Tier 2 — The execution core (C): the stack VM

| Component | Definition |
|---|---|
| **Stack VM** | A Turing-complete bytecode stack machine: operand stack, constant pool, globals, call frames, and the opcode dispatch loop. |
| **`slcore_exchange()` (stable ABI)** | The stable C exchange API through which the front end assembles and runs a program without touching VM internals. |
| **Value model (`SLValue`)** | The tagged union of runtime values: null, int, double, boolean, string, struct handle. |
| **Threading** | The bounded thread model exposed to Sleela: `spawn` / `join` / `lock` / `unlock` / `send` / `recv`, over real OS threads and mailboxes. |
| **Sockets** | TCP as VM-local integer handles (never raw sockets): `listen` / `accept` / `connect` / `sockread` / `sockwrite` / `sockclose`. |
| **Files & pipes** | Files, anonymous pipes, and named pipes/FIFOs as VM-local handles: `openFile` / `read` / `write` / `close` / `pipe` / `fifoCreate` / `unlinkFile`. |
| **Structs** | Named aggregates with `new`, `.` member access, reference semantics, and `structPack` / `structUnpack` transport. |
| **Artifact I/O** | Save/load a compiled program to/from a runnable `.sleela` artifact. |

Location: [`impl/core/`](impl/core/) — `sleela_core.{h,c}`, `sleela_artifact.c`.

## Tier 3 — Runtime services (C): what the core stands on

| Component | Definition | Where |
|---|---|---|
| **Garbage collector** | A mark-sweep collector with roots, byte accounting, and destructors: reclaims what the VM no longer reaches. | `runtime/garbage_collector.c` |
| **Security supervisor** | A role/class-based allocation policy: per-class and per-role instance and byte quotas (`reserve` / `release` / `authorize_class`). | `runtime/security_supervisor.c` |
| **Parameters** | The tunable runtime parameters the services read. | `runtime/Parameters.c` |
| **SHA-256 execution gate** | The fail-closed verifier that must confirm a trusted source manifest before build/run/diagnostics. | `tools/verify-before-execution.py` |

## Tier 4 — The OS-abstraction layer (C): one API, three backends

Every OS facility goes through this layer, which has POSIX and Win32 backends
and reports its native platform as `linux`, `macos`, or `windows`.

| Component | Definition | Where |
|---|---|---|
| **Threads backend** | Threads, mutexes, condition variables (pthreads / Win32). | `impl/core/sleela_thread.*` |
| **Network backend** | TCP sockets as opaque handles (BSD sockets / Winsock2). | `impl/core/sleela_net.*` |
| **I/O backend** | Files and pipes (POSIX / CreateFile+CreatePipe+named pipes). | `impl/core/sleela_io.*` |
| **Path backend** | Paths, cwd, absolute-ization, existence (realpath / GetFullPathName). | `impl/core/sleela_path.*` |
| **Terminal backend** | Interactive PTY / ConPTY. | `impl/core/sleela_terminal.*` |
| **Library backend** | Dynamic library loading (dlopen / LoadLibrary). | `impl/core/sleela_library.*` |
| **Time backend** | Wall-clock + monotonic time (clock_gettime / QueryPerformanceCounter). | `impl/core/sleela_time.*` |
| **Events backend** | Signal/event delivery over an in-process queue. | `impl/core/sleela_events.*` |
| **Memory backend** | Aligned allocation, page size, monotonic millis. | `impl/core/sleela_memory.*` |
| **Pixel terminal** | Frame-based, pixel-addressed terminal surface (Phraign™). | `bash/pixel_terminal.*` |

## Tier 5 — The catalog and conducted methods

| Component | Definition | Where |
|---|---|---|
| **Catalog parser** | Reads `SHEET.sheet` at compile time into the object model. | `impl/catalog/sheet_catalog.*` |
| **Conducted methods** | Compile-time built-ins backed by the catalog: `conduct` / `role` / `insight` / `congruent` / `route`, plus the system invariants `sysdepth()` (3024) and `degreemax()` (4). | resolved in the compiler |

## Tier 6 — Subject libraries: the domain layer

Two mechanisms: AST-synthesizing native modules (math/physics/economics/
inference) that lower to real bytecode, and compiled C++ reference libraries
(chemistry/finance) with their own frontend lowering.

| Component | Definition | Where |
|---|---|---|
| **Native dispatcher** | Validates subject imports and dependencies; lowers `module.fn` calls. | `impl/subjects/native/` |
| **Math** | Foundational numeric layer: sqrt, exp, log, sin/cos/tan, pow, hypot, fmod, constants. | `impl/subjects/math/` |
| **Physics** | Constants and executable formula families (mechanics, gravitation, relativity, waves). | `impl/subjects/physics/` |
| **Economics** | Time-value, elasticity, rates, profit/margin, GDP identity. | `impl/subjects/economics/` |
| **Inference** | Array-free statistics over a data series: mean/stdev, OLS trend, correlation/R², z-score, forecast, CAGR. | `impl/subjects/inference/` |
| **Chemistry** | Structured chemical inference: weighted evidence, deterministic ranking, explicit uncertainty. | `impl/subjects/chemistry/` |
| **Finance** | Financial mathematics: FV/PV, annuities, NPV, bonds, CAPM, WACC, 2×2 systems, quadratics. | `impl/subjects/finance/` |

## Tier 7 — Nordshrift: the `.sst` transpiler driver

| Component | Definition | Where |
|---|---|---|
| **Nordshrift CLI** | The `.sst` driver: `check`, `build`, `objects`, `relevance`, `version`. | `impl/nordshrift/nordshrift.cpp` |
| **SST lexer / parser** | Tokenizes and parses `.sst` control sheets. | `impl/nordshrift/sst_lexer.*`, `sst_parser.*` |
| **Subject model** | The Nordshrift 2.0 semantic model (Subject → Quantity → Unit → Assumption → Relation → Formula → … → Explanation). | `impl/nordshrift/subject_model.*`, `sheet_model.h` |
| **Source resolver** | Resolves `source:` globs to Wrapper™ inputs. | `impl/nordshrift/source_resolve.*` |
| **Sleela emitter** | Emits the `sleela` target (reusing the compiler/artifact path); java/c targets emit source text. | `impl/nordshrift/sleela_emit.*` |
| **Object compatibility** | The catalog-backed object-compatibility list. | `impl/nordshrift/object_compat.*` |
| **Component manifest** | Nordshrift's own component accounting. | `impl/nordshrift/component_manifest.*` |
| **Diagnostics** | The `NSS-*` diagnostic codes (provenance/dependency validation). | `impl/nordshrift/diagnostics.h` |

## Tier 8 — Ingest and provisioning (edges of the journey)

| Component | Definition | Where |
|---|---|---|
| **xclass loader** | Ingests `.xclass` SecureJDK 28 inputs into a runnable Sleela program. | `impl/xclass/xclass_loader.cpp` |
| **Defender / OSsupport** | The (opt-in, SHA-256-verified, `--allow-root`-gated) OS-defender provisioning path. | `impl/frontend/driver.cpp`, `impl/defender/` |

---

## The register (runnable roll-call)

The `.sleela` **register** documents in [`journey/`](journey/) are the executable
equivalent of this file: each names components on the journey and prints them,
and the catalog-backed ones are resolved against `SHEET.sheet` at compile time.

| Register | Covers |
|---|---|
| [`journey/register.sleela`](journey/register.sleela) | The master roll-call: every tier, in journey order, with a running count. |
| [`journey/register_frontend.sleela`](journey/register_frontend.sleela) | Tier 1 — the front end. |
| [`journey/register_core.sleela`](journey/register_core.sleela) | Tiers 2–4 — the core, runtime, and OS-abstraction. |
| [`journey/register_subjects.sleela`](journey/register_subjects.sleela) | Tier 6 — the subject libraries. |
| [`journey/register_nordshrift.sleela`](journey/register_nordshrift.sleela) | Tier 7 — Nordshrift. |
| [`journey/register_catalog.sleela`](journey/register_catalog.sleela) | Tier 5 — conducted methods, resolved live against `SHEET.sheet`. |

Run any of them:

```sh
export SLEELA_SHEET="$PWD/SHEET.sheet"
export SLEELA_SHA256_MANIFEST="$PWD/security/sha256-manifest.json"
./impl/build/sleela run journey/register.sleela
```
