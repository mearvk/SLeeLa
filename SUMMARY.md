# SUMMARY — The SLeeLa Repository, Accounted in Full

A single-file accounting of everything in the [`mearvk/SLeeLa`](https://github.com/mearvk/SLeeLa)
repository: its documents, source trees, specifications, methods, and special
functions. This summary is descriptive — it records what each artifact *is* and
where it lives, so the whole project can be read from one place.

The repository holds **two distinct bodies of work** that share one home:

- **Part A — Science & Engineering:** the **Sleela** programming language, its
  C/C++ execution core, the **Nordshrift** transpiler driver, the `SHEET.sheet`
  object catalog, the governing specifications, and the Sigil tooling.
- **Part B — United States (Evidentiary Method) and applied documents:** the
  `politico/` **SAKE** method, the `LENS.md` history-of-America lens, and
  `GERALDINE.FERRARO.md` (presidential governance analysis + the "Golden"
  representation model).

The [`GLOSSARY.md`](GLOSSARY.md) defines every term across both parts.

---

## 1. Top-Level Documents

| File | What it is |
|------|------------|
| [`README.md`](README.md) | Project front page. Introduces Sleela + Nordshrift, the **Wrapper™** filetype, the document map, and the **Constitution** of ordained constraints ("congrains") in five Articles (System invariants · Congrains · Limits · Health · IQ). |
| [`GLOSSARY.md`](GLOSSARY.md) | Definitions of all project terms, in two respects: **Part A** (science & engineering) and **Part B** (the evidentiary method), plus **B.1** (the Golden premise & classic representation terms). |
| [`SLEELA.md`](SLEELA.md) | Reference for the **`.sleela` filetype (Wrapper™)**, the syntax-version `#sleela` pragma, and the two distribution marks: the configurable **Sigil QR code** and the deterministic **248×48 steganographic frame**. |
| [`SOURCE.md`](SOURCE.md) | Characteristics of the Sleela **source file** (the Wrapper™): structure, encoding, the version pragma, lifecycle, and what is explicitly **not** source. |
| [`COMPILER.md`](COMPILER.md) | The Sleela compiler: pipeline stages, accepted language surface, and **version awareness** (accept / warn / reject rules per SL-META-0001 §4.4). |
| [`NORDSHRIFT.md`](NORDSHRIFT.md) | Concise overview of the `.sst` transpiler driver, **the triplet** (Java / Sleela / C), and the sheet structure — deferring to `SST.model` and `impl/nordshrift/NORDSHRIFT.md` as authoritative. |
| [`VERSION.md`](VERSION.md) | The single record of all versions: toolchain **0.1.2**, language syntax **1.0**, Nordshrift **1.0**, NS-SST-0001 **1.0.0**, SL-META-0001 **1.0.0**, with the versioning policy and bump procedure. |
| [`LENS.md`](LENS.md) | Applies the Sleela **Lens** idea (a focused get/set view) to *how the United States was created* — who built it, who bought it (land acquisitions), how it was governed into being, and the 1967 "reliefs" — as dated, sourced facts kept apart from framing. Includes appendices on all 27 amendments and statehood order. |
| [`GERALDINE.FERRARO.md`](GERALDINE.FERRARO.md) | Presidential governance analysis 1947–present (excl. Obama, Biden), the **Golden premise** baseline model, and a classic **trustee/delegate** model of representation applied qualitatively (manner, not magnitude). |

---

## 2. Governing Specifications

Three normative/pre-normative specification files define the formal core.

| File | Series | Status | Purpose |
|------|--------|--------|---------|
| [`src/Sleela.manifest`](src/Sleela.manifest) | **SL-META-0001** (rev 1.0.0) | Pre-Normative | The **Sleela Language Metadocument** — the constitutional document governing the language's design mandate, meta-model, type system, semantics, the compilation pipeline, and the §4.4 syntax-versioning policy. Governed by the Sleela Language Design Council (SLDC). |
| [`SST.model`](SST.model) | **NS-SST-0001** (rev 1.0.0) | Normative | The **`.sst` Scripting Sheet format** — lexical conventions, section schemas (`sheet`, `import`, `source`, `target`, `pipeline`, `rules`, `effects`, `derive`, `guards`, `interop`, `profile`), and the full `NSS-*` diagnostic index. |
| [`impl/xclass/XCLASS.model`](impl/xclass/XCLASS.model) | **XCI-0001** (rev 1.0.0) | Normative (v1 loader) | The **`.xclass` ingestion model** — the mapping from SecureJDK 28 XML class files into a runnable Sleela program. |
| [`src/Sleela.formal-core`](src/Sleela.formal-core) | — | Supporting | The formal-core companion to the metadocument (892 lines). |
| [`SLEELA.syntax`](SLEELA.syntax) | — | Supporting | The Sleela surface-syntax reference. |

---

## 3. The Implementation (`impl/`) — the working C/C++ system

The current, buildable implementation. Build with `cd impl && make` (needs a C11
and a C++17 compiler); run `make test` for the full example + version suite.

### 3.1 The execution core (`impl/core/`)

The Turing-complete engine that knows nothing about Sleela — it understands only
opcodes and values, and is driven entirely through one dispatch symbol.

| File | Purpose |
|------|---------|
| [`impl/core/sleela_core.h`](impl/core/sleela_core.h) | Public **C ABI**: the `SLValue` value model, opcode set, and the **exchange API**. |
| [`impl/core/sleela_core.c`](impl/core/sleela_core.c) | The stack VM: operand stack, constant pool, globals, call frames, opcode dispatch loop, and the bounded threading runtime. |
| [`impl/core/exchange_smoke.c`](impl/core/exchange_smoke.c) | Proof that the core is fully drivable via `slcore_exchange` **alone** (assembles and runs `main(){print(6*7);}` → `42`). |
| [`impl/core/thread_smoke.c`](impl/core/thread_smoke.c) | Threading smoke test. |

**The exchange API — the central special function:**

```c
SLResult slcore_exchange(SLVM* vm, SLExchangeOp op, SLExchangeArg* arg);
```

`SLExchangeOp` selects the operation (`RESET`, `ADD_CONST`, `DECLARE_GLOBAL`,
`BEGIN_FUNC`, `END_FUNC`, `EMIT`, `PATCH`, `SET_ENTRY`, `RUN`, `GET_RESULT`);
`SLExchangeArg` is the tagged in/out packet. Typed builder helpers
(`slvm_emit`, `slvm_add_const_*`, `slvm_begin_func`, `slvm_patch`, …) are thin
wrappers over this one dispatch entry point.

**Opcode set:** `NOP`, `CONST/POP/DUP`, `LOADG/STOREG/LOADL/STOREL`,
`ADD/SUB/MUL/DIV/MOD`, `NEG`, `EQ/NE/LT/LE/GT/GE`, `AND/OR/NOT`, `JMP/JMPF`,
`CALL/RET`, `PRINT`, threading (`SPAWN/JOINALL/LOCK/UNLOCK/SEND/RECV`), `HALT`.

**Threading model (bounded, clean):** up to **128** threads, **32** lock-table
slots, and a **32-slot 2-tuple mailbox** ("the burble line"). Class fields are
thread-safe globals; method locals are per-thread; `print` lines are atomic.

### 3.2 The language front end (`impl/frontend/`)

The C++ compiler that lowers Java-like Sleela source to core bytecode — owns
**no** execution logic.

| File | Stage |
|------|-------|
| [`version.{h,cpp}`](impl/frontend/version.h) | **Version resolution** — resolve/enforce the `#sleela` pragma *before* parsing; holds `min`/`maxSupportedSyntax()` (`1.0 .. 1.0`). |
| [`lexer.{h,cpp}`](impl/frontend/lexer.h) | **Lexing** — Java-like tokenizer. |
| [`ast.h`](impl/frontend/ast.h) | AST node definitions. |
| [`parser.{h,cpp}`](impl/frontend/parser.h) | **Parsing** — recursive-descent parser (classes, fields, methods, statements, precedence-climbing expressions). |
| [`compiler.{h,cpp}`](impl/frontend/compiler.h) | **Codegen** — AST → core bytecode; resolves conducted-method built-ins at compile time. |
| [`driver.cpp`](impl/frontend/driver.cpp) | The `sleela` CLI (`run` / `check` / `version` / `xclass`); holds `kVersion` = **0.1.2**. |

**CLI:** `sleela run file.sleela` · `sleela check file.sleela` · `sleela version`
· `sleela run file.xclass` / `sleela xclass --emit|--info`.

### 3.3 Nordshrift — the `.sst` transpiler driver (`impl/nordshrift/`)

Reads a `.sst` control sheet and transpiles the sources it names into **the
triplet** (Java / Sleela / C); the `sleela` target additionally runs on the C
core.

| File | Purpose |
|------|---------|
| `nordshrift.cpp` | The `nordshrift` CLI (`check` / `build` / `version` / `objects` / `relevance`); holds `kVersion` = **1.0**. |
| `sst_lexer.{h,cpp}`, `sst_parser.{h,cpp}` | Lexer/parser for the indentation-significant, pragma-first `.sst` format. |
| `sheet_model.h` | The parsed sheet model. |
| `source_resolve.{h,cpp}` | Resolves the `source:` glob into Wrapper™ files. |
| `sleela_emit.{h,cpp}` | Emits the selected target. |
| `object_compat.{h,cpp}` | The **object compatibility list**: per-target **relevance** (`direct` / `model` / `none`) for each `SHEET.sheet` object. |
| `diagnostics.h` | The `NSS-{E\|W\|N}-{XXXX}` diagnostic codes. |
| [`NORDSHRIFT.md`](impl/nordshrift/NORDSHRIFT.md) | Implementation guide (section status, diagnostics, module layout). |

**Special functions (CLI):** `nordshrift objects` (the 129-object list) ·
`nordshrift relevance --target=c Thread` (→ `direct -> pthread_t`).

### 3.4 The shared catalog (`impl/catalog/`)

`sheet_catalog.{h,cpp}` — the single parser for `SHEET.sheet`, used by **both**
the Sleela compiler (conducted methods) and Nordshrift (compatibility list), so
the two tools agree object-for-object.

### 3.5 `.xclass` ingestion (`impl/xclass/`)

`xclass_loader.{h,cpp}` reconstructs a runnable Sleela program from a SecureJDK
28 `.xclass` (XML class) file per `XCLASS.model` (XCI-0001). Samples:
`Greeter.xclass`, `Person.xclass`, `Role.xclass`. An `.xclass` carries
**structure, not bytecode**, so methods become typed skeletons with synthesized
default-return bodies; the reconstruction round-trips through `--emit`.

### 3.6 Examples, tests, build

- [`impl/examples/`](impl/examples/) — `hello`, `factorial`, `fibonacci`,
  `fizzbuzz`, `threads`, `conduct`, `versioned` (`.sleela`).
- [`impl/tests/version/`](impl/tests/version/) — version-awareness fixtures
  (`malformed`, `minor_ahead`, `too_new`) + `run_version_tests.sh`.
- [`impl/Makefile`](impl/Makefile) — builds `build/sleela` and `build/nordshrift`.

---

## 4. The Object Catalog — `SHEET.sheet`

[`SHEET.sheet`](SHEET.sheet) is the catalog of common system objects: **129
objects across 16 role categories**, carrying the `System` root and its
invariants — system **depth 3024** and maximum **complexity degree 4**. It backs
two things:

- **Sleela conducted methods** (compile-time, no runtime library):
  `conduct("Name")`, `role("Name")`, `insight("Name")`, `congruent("A","B")`,
  `route("A","B")`, `sysdepth()` (→ 3024), `degreemax()` (→ 4).
- **Nordshrift's object compatibility list** and per-target relevance.

Its `congrains` and `limits` sections plus the `system` invariants are the
source of the **Constitution** printed in `README.md`.

---

## 5. The Sigil Tooling — `tools/sigil/`

A dependency-free (pure Python standard library, offline) generator for the two
Sleela **distribution marks**.

| File | Purpose |
|------|---------|
| `sigil.py` | CLI entry point — resolves the URL, emits the QR + frame. |
| `qr.py` | Pure-Python **QR encoder** (byte mode, Reed–Solomon ECC over GF(256), masking/penalty scoring, versions 1–10). |
| `png.py` | Minimal grayscale PNG writer (stdlib `zlib`). |
| `stego.py` | The deterministic **248×48 steganographic frame**: `generate` / `recover` / `verify`. |
| `qr_decode_check.py` | Self-decoder proving the QR round-trips to its input URL (zero RS syndromes). |
| `sigil.config.json` | The configurable default URL. |
| `out/` | Generated artifacts (QR + frame, PNG + ASCII). |

**Special functions / properties:**

- **Configurable QR code** — encodes one URL; resolution precedence
  `--url` → `SLEELA_QR_URL` → config file → built-in default.
- **Deterministic Sigil frame** — 11,904-bit field seeded by
  `SHA-256("SLEELA-SIGIL-v1" ‖ 0x00 ‖ input)`, filled by an **HMAC-DRBG**
  (HMAC-SHA-256) keystream, with a steganographic header (magic `SLSG`, version,
  width, height, and a 128-bit digest) XOR-woven at keyed positions. It is
  **idempotent** (same input → byte-identical output), **unique per input**, and
  **verifiable** (`sigil.py --verify` recovers the embedded digest).

---

## 6. Legacy Java Exploration — `src/implementations/_001_/`

The older (2019-era) Java "Nordshrift" exploration, retained untouched as
project history. It contains the original Nordshrift model, drivers, agents
(`BodiAgent`, `StackAgent`), descriptors, contexts, monitors, threading,
structures (`Bowl`/`Cube`/`Sphere`), and `.ns` documents
(`chatclient.ns`, `chatserver.ns`, `nordshrift.driver.ns`). The current,
authoritative implementation is the C/C++ tree under `impl/`.

---

## 7. The `politico/` Documents — the SAKE Evidentiary Method (Part B)

A disciplined method for **reasoning about public records** concerning U.S.
figures and institutions. Its terms are *methodological* — they describe how to
weigh documentary evidence, and the governing rule throughout is that
**interpretation must not be turned into fact** (`METAPHOR ≠ FACT`,
`ASSOCIATION ≠ CAUSATION`, `LABEL ≠ DOCTRINE`, `UNKNOWN ≠ TRUE/FALSE`).

| File | What it defines |
|------|-----------------|
| [`politico/SAKES.md`](politico/SAKES.md) | The core **SAKE** method: simple fact, stirred ratio, close-circuit ratio, the two-event continuity test, corrected values, and the interpretive rule. |
| [`politico/SAKES_SIZE.md`](politico/SAKES_SIZE.md) | **SAKES** extension — *evidentiary size, root, disclosure*: the chain `PERSON → DOCUMENT → DATE → CONTENT → PROVENANCE → CONTEXT → LIMITED CONCLUSION`. |
| [`politico/SAKES_BINARY_TIMELINE.md`](politico/SAKES_BINARY_TIMELINE.md) | The **universal binary rule** (`EVERY = ALL`, `ALL = each occurrence`): a proposition passes only if every required occurrence holds. |
| [`politico/1982.md`](politico/1982.md) | **Ordered continuance** — the first-whole-bearing rule that a search does not assume continuity; continuance must be established before the inquiry advances. |

> **Method note.** In Part B, labels and associations are treated as *claims to
> be tested against the documented record*, never as established fact. This
> summary preserves that discipline: it records what the documents *are*, not
> conclusions about any person.

---

## 8. Project Configuration

- [`SLeeLa.iml`](SLeeLa.iml) / `.idea/` — IntelliJ IDEA project files (module,
  encodings, VCS, dictionaries).
- [`impl/.gitignore`](impl/.gitignore), `tools/sigil/.gitignore` — build/output ignores.

---

## 9. How the Pieces Fit (one diagram)

```
  Wrapper™ (.sleela source)                         .sst control sheet (NS-SST-0001)
        │                                                     │
        ▼                                                     ▼
  [ Sleela front end — C++ ]  ◄── shared front end ──  [ Nordshrift driver ]
    version → lexer → parser → AST → compiler                 │  resolves source: glob
        │  emits opcodes + drives the core                    ▼
        ▼                                          target: java | sleela | c  (the triplet)
  [ Sleela Core — C, stable ABI ]
    stack VM · slcore_exchange() · 128-thread runtime
        ▲                                   ▲
        │                                   │
   SHEET.sheet catalog  ──────────  shared catalog/ module
   (129 objects · depth 3024 · degree 4)   (conducted methods + compat list)
```

---

## Bibliography / Citation

**Max Rupplin — MEARVK LLC — 2026.**

- Rupplin, Max. *SLeeLa: Optional Compile-Time Support — Sleela language,
  Nordshrift transpiler driver, SHEET.sheet catalog, Sigil tooling, and the
  SAKE evidentiary method.* MEARVK LLC, 2026.
  Repository: <https://github.com/mearvk/SLeeLa>.

### Governing specifications cited herein

- **SL-META-0001** — *Sleela Language Metadocument* (Pre-Normative, rev 1.0.0),
  September 2026. `src/Sleela.manifest`.
- **NS-SST-0001** — *Nordshrift `.sst` Scripting Sheet Format Specification*
  (Normative, rev 1.0.0), September 2026. `SST.model`.
- **XCI-0001** — *Sleela `.xclass` Ingestion Model* (Normative, rev 1.0.0),
  September 2026. `impl/xclass/XCLASS.model`.

### In-repository references

- `README.md`, `GLOSSARY.md`, `SLEELA.md`, `SOURCE.md`, `COMPILER.md`,
  `NORDSHRIFT.md`, `VERSION.md`, `LENS.md`, `GERALDINE.FERRARO.md`.
- `impl/README.md`, `impl/DESIGN.md`, `impl/nordshrift/README.md`,
  `impl/nordshrift/NORDSHRIFT.md`, `impl/xclass/README.md`,
  `tools/sigil/README.md`.
- `politico/SAKES.md`, `politico/SAKES_SIZE.md`,
  `politico/SAKES_BINARY_TIMELINE.md`, `politico/1982.md`.

*Prepared as a descriptive accounting of the repository as of 2026. Versions
(toolchain 0.1.2 · syntax 1.0 · Nordshrift 1.0) are current as of this writing;
consult `VERSION.md` for the authoritative record.*
