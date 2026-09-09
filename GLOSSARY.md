# GLOSSARY

Definitions of the terms used across this project, as they are used **here**.
The glossary is organized into two respects, matching the two bodies of work in
this repository:

- **Part A — Science & Engineering:** the Sleela language, compiler, tooling,
  and formal vocabulary.
- **Part B — United States (Evidentiary Method):** the disciplined reasoning
  vocabulary of the `politico/` documents (the SAKE method).

Each term cites the document that governs it. Where a term is a registered mark
(**Wrapper™**), it is shown as such.

---

## Part A — Science & Engineering

### A.1 Core project terms

| Term | Definition | Source |
|------|-----------|--------|
| **Sleela** | A Java-like programming language that runs on a Turing-complete C/C++ execution core. | `README.md`, `impl/README.md` |
| **Wrapper™** | The name of the `.sleela` filetype: a Sleela **source file** — the *program* — carrying the *metadocument addend* (governed by SL-META-0001). "`.sleela` file", "Sleela source file", and "Wrapper™" name the same thing. | `SLEELA.md`, `SOURCE.md` |
| **Source file** | A `.sleela` Wrapper™; the human-authored program unit the toolchain compiles and runs. A `.sst` sheet is **not** source. | `SOURCE.md` |
| **Nordshrift** | The transpiler *driver* for Sleela: it reads a `.sst` control sheet and transpiles the sources it names. | `NORDSHRIFT.md`, `SST.model` |
| **Sleela Core** | The embeddable, stack-based bytecode virtual machine (stable C ABI) beneath the language. | `impl/DESIGN.md` |
| **Exchange API** | The single core entry point `slcore_exchange()` through which everything above the core drives execution. | `impl/README.md` |
| **Triplet** | Nordshrift's three transpilation targets: **Java**, **Sleela**, **C** (selected by `target-language`). | `NORDSHRIFT.md` |
| **Metadocument addend** | The property that a Wrapper™ is governed by, and consistent with, the Sleela Language Metadocument. | `SLEELA.md` |

### A.2 Documents & filetypes

| Term | Definition | Source |
|------|-----------|--------|
| **SL-META-0001** | The **Sleela Language Metadocument** — the constitutional (pre-normative) document governing the language's design, meta-model, and rules. | `src/Sleela.manifest` |
| **NS-SST-0001** | The **normative** specification of the `.sst` format. | `SST.model` |
| **`.sst`** | A **Scripting Sheet**: an indentation-significant, pragma-first *control surface* naming sources and selecting a target. Not a program. | `NORDSHRIFT.md`, `SST.model` |
| **`.sheet` / `SHEET.sheet`** | The catalog of common system objects (129 objects, 16 role categories) backing conducted methods and the object-compatibility list. | `SHEET.sheet` |
| **`.manifest`** | The file holding the SL-META-0001 metadocument text. | `src/Sleela.manifest` |
| **`.xclass`** | SecureJDK 28 ingest input the CLI reconstructs into a Sleela program. | `impl/frontend/driver.cpp` |

### A.3 Meta-model primitives (SL-META-0001 §2.2)

The ontological primitives over which all Sleela constructs exist.

| Primitive | Definition |
|-----------|-----------|
| **Entity** | The root concept of any named, typed, bounded construct; the fundamental unit of program structure. |
| **Contract** | A behavioral promise an Entity makes, expressed independently of implementation; a set of named operations with signatures and optional Effects. |
| **Scope** | The lexical/semantic boundary within which an Entity, Binding, or Contract is visible; Scopes nest into an acyclic tree rooted at the module Scope. |
| **Binding** | The association of an identifier with an Entity in a Scope; final by default (no rebinding without an explicit mutable declaration). |
| **Flow** | A directed sequence of operations yielding a value or Effect; a first-class value with an input type, output type, and declared Effect set. A **pure flow** declares no Effects; an **effectful flow** declares one or more. |
| **Effect** | A side-effecting operation declared as part of a Flow's type signature (I/O, mutation, exceptions, external calls). Undeclared Effects are a compile error. |
| **Context** | An ambient carrier of typed values implicitly available within a Scope (e.g. transaction handles, auth tokens, logging sinks), propagated to enclosed Scopes. |
| **Lens** | A composable get/set accessor-mutator pair over an Entity's field(s), satisfying algebraic laws; composes sequentially and in parallel. |
| **Rule** | A declarative constraint or transformation applied at compile time (Phase 7) to the AST/IR; failures produce `SL-E-XXXX` diagnostics. |
| **Projection** | A read-only, derived view of an Entity exposing a subset of fields or computed values; carries no identity and admits no mutation. |

### A.4 Type-system & semantics vocabulary (SL-META-0001 §5)

| Term | Definition |
|------|-----------|
| **Denotational semantics** | Sleela's semantic model: every construct has a well-defined mathematical denotation independent of evaluation strategy. |
| **Hindley-Milner inference** | The type-inference basis, extended with row polymorphism (Contexts), structural subtyping (Contracts), and an effect system (Flows). |
| **Maybe** | The type modeling absence of a value; Sleela has **no implicit null**. Unwrapping requires an exhaustive case (SEM-10). |
| **Effect subsumption** | A Flow with a smaller Effect set is substitutable where a larger one is expected (SEM-09). |
| **Variance** | Declaration-site `in` (contravariant) / `out` (covariant); unmarked parameters are invariant. |
| **Sealed hierarchy** | Entity hierarchies are sealed by default; `open` permits cross-module extension, enabling exhaustiveness checking. |

### A.5 Compiler & versioning

| Term | Definition | Source |
|------|-----------|--------|
| **Front end** | The C++ pipeline: version check → lexer → parser → AST → compiler (codegen). Owns no execution logic. | `COMPILER.md` |
| **Compilation pipeline** | The metadocument's eleven ordered phases (Lexical Analysis → … → Diagnostics and Source Mapping). | `src/Sleela.manifest` §6.1 |
| **`#sleela` pragma** | The per-file syntax-version declaration `#sleela MAJOR.MINOR` on the first non-blank, non-comment line. | `COMPILER.md`, `SOURCE.md` |
| **Version aware** | The compiler enforces the supported syntax range: **accept** in-range, **warn** (assume the floor) when absent, **reject** out-of-range or malformed. | `COMPILER.md` §3 |
| **Supported syntax range** | `minSupportedSyntax() .. maxSupportedSyntax()` (currently `1.0 .. 1.0`) in `impl/frontend/version.h`. | `VERSION.md` |
| **Toolchain version** | The implementation version reported by `sleela version` (currently **0.1.2**), versioned independently of the language syntax. | `VERSION.md` |
| **Conducted method** | A `SHEET.sheet`-backed built-in the compiler resolves at compile time (`conduct`/`role`/`insight`/`congruent`/`route`/`sysdepth`/`degreemax`). | `impl/README.md` |
| **Congrain** | A governing constraint from `SHEET.sheet` (Invariant, Precondition, Postcondition, Constraint, Dependency, Coupling, Congruence, Contract). | `README.md`, `SHEET.sheet` |
| **NSS-\* diagnostic** | A Nordshrift diagnostic code `NSS-{E\|W\|N}-{XXXX}` with file, line, message, and governing rule. | `impl/nordshrift/NORDSHRIFT.md` |

### A.6 Sigil tooling (SLEELA.md)

| Term | Definition |
|------|-----------|
| **Sigil** | The pair of distribution marks generated for a Sleela distribution: a QR code and a steganographic frame. |
| **QR code** | A configurable QR encoding a URL (default: the project repository); configurable via `--url`, `SLEELA_QR_URL`, or a config file. |
| **Steganographic frame** | A deterministic 248 × 48-bit bitmap that looks like noise but is an idempotent function of its input, with a recoverable embedded digest. |
| **Idempotent** | The same input always yields the byte-for-byte identical output (the frame is unique to its input→output mapping). |
| **HMAC-DRBG** | The keyed deterministic random bit generator (HMAC-SHA-256) producing the frame's reproducible "stochastic" field. |

---

## Part B — United States (Evidentiary Method)

The `politico/` documents define a **disciplined method for reasoning about
public records** concerning United States figures and institutions. These terms
are *methodological*: they describe how to weigh documentary evidence, not
political conclusions. The governing rule throughout is that interpretation must
not be turned into fact.

| Term | Definition | Source |
|------|-----------|--------|
| **SAKE / SAKES** | The named method for evaluating a proposition about a person against the documentary record; SAKES extends it to size, root, disclosure, and meaning. | `politico/SAKES.md`, `politico/SAKES_SIZE.md` |
| **Simple fact** | The factual anchor that survives comparison and testing — a demonstrable, not imagined, relation. | `politico/SAKES.md` |
| **Stirred ratio** | A method of comparison: facts placed beside one another, questioned, and tested for relationship. | `politico/SAKES.md` |
| **Close-circuit ratio** | An unreliable inference pattern in which association is repeatedly used to prove the proposition from which the association was itself inferred. | `politico/SAKES.md` |
| **Evidentiary size** | The amount, density, continuity, and provenance of documentary detail legitimately attachable to a proposition about a person (not greatness or rank). | `politico/SAKES_SIZE.md` |
| **Root node** | A primary, dated, attributable source that anchors an inquiry — e.g. the **1982** correspondence held by Emory University. | `politico/SAKES_SIZE.md`, `politico/1982.md` |
| **Provenance chain** | The ordered test `PERSON → DOCUMENT → DATE → CONTENT → PROVENANCE → CONTEXT → LIMITED CONCLUSION`; it may be extended by evidence but never shortened by interpretation. | `politico/SAKES_SIZE.md` |
| **Primary documentary node** | Direct evidence of what a contemporaneous document contains (e.g. a 1982 letter), distinct from a later interpretive node. | `politico/SAKES_SIZE.md` |
| **Interpretive node** | Later commentary about an earlier record; it must not silently overwrite the primary document. | `politico/SAKES_SIZE.md` |
| **Ordered continuance** | The rule that a search does **not** assume continuity: continuance from one record to the next must itself be established before the inquiry advances. | `politico/1982.md` |
| **First whole bearing** | The starting node of an inquiry (here, **1982**) from which relevance may enter only upon an established continuation. | `politico/1982.md` |
| **Universal binary rule** | The strict test where **EVERY = ALL** and **ALL = each occurrence**: the proposition passes only if every required occurrence holds; any unestablished or contradicted occurrence yields **NO**. | `politico/SAKES_BINARY_TIMELINE.md` |
| **Limited conclusion** | The only kind of conclusion the method permits: one bounded strictly by what the documented chain supports. | `politico/SAKES_SIZE.md` |

> **Method note.** In Part B, labels (e.g. political-economic labels) and
> associations are treated as *claims to be tested against documented record*,
> never as established fact. Race, office, ideology, or proximity do not
> constitute evidence. This is the discipline the SAKE documents impose on
> themselves, and the glossary preserves it.

---

## See also

- [`README.md`](README.md) — project overview and the Constitution of congrains.
- [`SLEELA.md`](SLEELA.md) — the `.sleela` filetype (Wrapper™) and Sigil tooling.
- [`SOURCE.md`](SOURCE.md) — source-file characteristics.
- [`COMPILER.md`](COMPILER.md) — the compiler and its versions.
- [`NORDSHRIFT.md`](NORDSHRIFT.md) — the `.sst` transpiler driver.
- [`VERSION.md`](VERSION.md) — the single record of all versions.
- `src/Sleela.manifest` (SL-META-0001) and `SST.model` (NS-SST-0001) — the governing specifications.
