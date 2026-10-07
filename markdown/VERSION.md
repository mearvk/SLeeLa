# SLeeLa Version

## Current Development Version

**SLeeLa:** 0.3.25-dev
**Sleela-Complete:** 0.3.2-dev  
**Nordshrift Complete:** 2.7.1-dev  
**Native Foundation:** 0.3.2-dev  
**Sleela Language Syntax:** 1.6 (supported range 1.3 .. 1.6)  
**Compiler Compatibility Gate:** 2.9-dev  
**Java Authorship Transition Gate:** 1.19-dev
**Edition:** SLeeLa Complete / Native Foundation  
**Status:** Active Development  
**Repository:** mearkv/SLeeLa

## 0.3.9 Development Increment

This increment advances Java source-equivalence semantics with overload and override resolution foundations: applicability phases, conversion-aware candidate filtering, most-specific selection, override-equivalence, and basic return compatibility.

### Java compatibility boundary

- Java compatibility keywords and modifiers are explicitly treated as authorship-preservation vocabulary.
- Java annotations are explicitly treated as compatibility metadata rather than native SLeeLa annotations.
- Java API counterparts under `lib/java` are explicitly separated from native SLeeLa symbols.
- Java compatibility counts are required to remain separate from native SLeeLa symbol counts.
- Added `JAVA.COMPATIBILITY.SCOPE.md`.
- Added a Java compatibility boundary fixture.
- Updated `QUALIFICATION.JAVA.md` to focus on source/API congruence rather than JVM/SLVM execution.
- Clarified that Java compatibility does not require an SLVM.

The Java Language Specification provides the source-level reference for Java lexical, type, declaration, class, interface, annotation, and related constructs. citeturn0search0

**SLeeLa — MEARVK LLC — 2026**

## 0.3.11 Development Increment

This increment refines Java overload and override semantics beyond the 0.3.9 foundation. It adds generic-method inference foundations, lambda/method-reference pertinence, dedicated most-specific comparison, covariant reference return checks, checked-exception restrictions, access/static/final/private override restrictions, and variable-arity applicability.

This remains a source-level Java congruence implementation; it is not a JVM or SLVM implementation. Full Java overload and override qualification remains subject to the JLS rules for generic inference, functional target typing, subsignatures, interface/default inheritance, and related corner cases.

**Java Authorship Transition Gate:** 1.9-dev


## 0.3.11 Development Increment

This increment extends the Java overload/override foundation with maximally-specific selection, generic invocation-type inference, target-type compatibility, concrete/abstract/default tie handling, covariant return preference, and interface/default-method conflict detection.

**Java Authorship Transition Gate:** 1.9-dev


## 0.3.13 Development Increment

Java checked-exception source semantics foundation added: exception hierarchy/subtyping, checked-vs-unchecked classification, catch-or-declare coverage, redundant catch detection, and overriding throws compatibility. Full Java exception propagation remains in qualification.

**Java Authorship Transition Gate:** 1.12-dev


## 0.3.14 Development Increment

Java API dependency-closure foundation added: Java-qualified source-reference discovery, counterpart path mapping, missing-type diagnostics, and deterministic isolated-fixture testing.

**Java Authorship Transition Gate:** 1.12-dev


## 0.3.15 Development Increment

Java negative/constraint qualification corpus added: ten deterministic source fixtures with explicit expected diagnostic families, a manifest, a qualification runner, and a `java-constraints` Makefile target. The suite connects the corpus to the existing flow, checked-exception, overload/override, and Java API dependency-closure foundations.

**Java Authorship Transition Gate:** 1.13-dev

## 0.3.16 Development Increment

Normalized Java/SLeeLa declaration and signature comparison added. The qualification tool compares normalized declarations, methods, constructors, generic parameter shape, parameter/return types, throws types, modifiers, and fields using a deterministic paired-source fixture.

**Java Authorship Transition Gate:** 1.14-dev


## 0.3.17 Development Increment

Normalized Java/SLeeLa declaration comparison was deepened from the 0.3.16 foundation. The comparison normal form now distinguishes constructors from methods, records declaration ownership/path metadata, inheritance clauses (extends, implements, permits), annotations, parameter names/types/varargs metadata, and a versioned normalization schema. The qualification suite now includes a deliberate constructor-parameter mismatch to prove that mismatches are detected rather than merely accepting paired text.

**Java Authorship Transition Gate:** 1.15-dev


## 0.3.18 Development Increment

Added the Java source-equivalence platform/reproducibility qualification system for Q10. Linux, Windows 10+, and macOS now have explicit qualification records covering architecture families, toolchain expectations, required Python tooling, and deterministic qualification commands. The runner records observed host evidence and distinguishes READY from NOT_EXECUTED rather than claiming untested remote platforms have passed.

**Java Authorship Transition Gate:** 1.16-dev


## 0.3.19 Development Increment

Added the unified Java qualification manifest, which coordinates the existing source-equivalence, expression, normalized signature, constraint, flow, exception, API-dependency, and platform qualification layers. Each child result remains visible, while the aggregate record provides deterministic counts, environment metadata, and an overall status.

**Java Authorship Transition Gate:** 1.17-dev


## 0.3.20 Development Increment

The next semantic qualification layer hardens Java control-flow and checked-exception analysis. The source-semantic work remains independent of JVM or SLVM execution and is being extended toward complete Java definite-assignment, abrupt-completion, and exception-flow joins.

**Java Authorship Transition Gate:** 1.18-dev

### 0.3.21 Development Execution Phase

The first executable Java Flow 0.3.21 phase is now implemented in `impl/frontend/java_flow.cpp`. It introduces directional true/false expression states for boolean constants, `!`, `&&`, `||`, and `?:`; structural break/continue targets; labeled-block break handling; loop normal-completion joins; and scoped abrupt-exit consumption. A dedicated `java-flow-021` qualification suite and C++ fixture exercise these rules.

This is an incremental semantic implementation, not completion of the entire Java Chapter 16 model. For-loop update semantics, full switch rules, try/finally abrupt replacement, constructor/blank-final context, lambda capture flow, and integrated checked-exception propagation remain subsequent phases.

**Java Authorship Transition Gate:** 1.19-dev

### 0.3.21 Next Execution Step — Basic `for` Flow

The Java Flow implementation now carries explicit basic-`for` initialization and incrementation components. The analyzer traverses initialization, recognizes the condition as an optional component, establishes the loop body as the condition-true path, validates `break`/`continue` targets, and traverses the update expressions on normal and continue paths. The normal result remains defined by the condition-false path and matching `break` exits, consistent with JLS Chapter 16.

This step is intentionally incremental: the current public flow model does not yet expose a full fixed-point representation for repeated loop iterations, so update traversal currently establishes diagnostic coverage rather than claiming complete iterative definite-unassignment semantics.

### 0.3.21 Loop Fixed-Point Execution

The Java Flow analyzer now performs bounded convergence for while, do, and basic for loops. Loop-head facts are repeatedly recomputed from normal body completion and matching continue paths until the assigned/unassigned facts stabilize or the deterministic iteration cap is reached. Matching break paths remain separate normal exits. A loop whose condition is statically false therefore contributes no body facts to its post-loop state, while a guaranteed assignment followed by break can establish a definite assignment on the exit path.

The iteration cap is an implementation safety bound, not a semantic claim of completeness. Full JLS Chapter 16 loop treatment still requires additional path-sensitive constant analysis and more precise abrupt-completion interactions.

## 0.3.22 Development Increment

Language and library expansion.

### Language core

- Added a first-class, dynamic, zero-indexed **array** type (`T[]`): `new T[n]`,
  `a[i]` read, runtime-bounds-checked `a[i] = v` write, and the built-ins
  `arrayNew`, `arrayLength`, `arrayGet`, `arraySet`, `arrayPush`. The feature
  spans the whole pipeline (semantic `Kind::Array`, compiler lowering, parser
  acceptance of `new T[n]`, a new `SL_ARRAY` value tag, and the appended
  `OP_NEWARRAY` / `OP_ARRGET` / `OP_ARRSET` / `OP_ARRLEN` / `OP_ARRPUSH`
  opcodes). Existing programs compile and run unchanged.
- Added multi-file compile/run input and the `/languages` localization packs.

### Library (`/lib`)

- Added the `opcodes` package family: one SLeeLa class per VM opcode (103
  `SLOp*` classes plus `SLOpcodeBase` and `SLOpcodeStream` — 105 source units).
- Added `opcodes/governance` (Registrar / Listener / Event Observer discretion,
  8 classes) and `opcodes/running` (grouping, conditional-reactive, warming,
  6 classes).
- Added the `sldocument` package family and the `.sldocument` ordered,
  top-down document format, and made `.sldocument` a selectable compile choice
  (`lib/compiler/SLSourceForm`, `lib/compiler/SLCompileChoice`) with naming
  conventions for converting documents to named `.sleela`.
- Added the `autocad` and `website` package families (each with a 0.1.0 Maven
  GUI module).

### New subsystems

- SleelaTerminal™ graphical front-end work (see `sleela-terminal/VERSION.md`,
  now 1.1.0): living title-bar throbber, recoloured window controls, brand
  logo, Settings subframe, `sleela$` default prompt, and the gradient title
  bar, plus five shell runtime-bug fixes.

**Java Authorship Transition Gate:** 1.19-dev

## 0.3.23 Development Increment

Version-control and consistency refresh. Advanced the active development line to
0.3.23-dev and reconciled the previously divergent version strings across the
repository so every record agrees:

- Regenerated `lib/LIBRARY.SYMBOLS.md` from the live `/lib` tree (82 package
  families; 10,186 source classes + 55 module facades = 10,241 symbol records)
  and mirrored the class vocabulary into `SLEELA.syntax` §11,
  `impl/nordshrift/SST.SYMBOLS.md`, and `impl/nordshrift/NORDSHRIFT.SYMBOLS.md`
  as informative (non-authoritative) appendices.
- Reconciled the **syntax range** to the code-enforced `1.3 .. 1.6`
  (`impl/frontend/version.h`): updated the stale `SLEELA.syntax` header
  (previously syntax 1.3 / range 1.0 .. 1.3) and the `run_version_tests.sh`
  header comment.
- Reconciled the **compiler/tool version**: `Sleelvac™` now reports
  `0.3.23-dev` in `impl/frontend/driver.cpp` and in the `SLEELA.syntax` header,
  tracking the active development line.
- Reconciled the **Nordshrift** version to `2.7.1-dev` in
  `impl/nordshrift/nordshrift.cpp` and `SST-2.0.model` (previously 2.6-dev /
  2.7-dev), matching the "Nordshrift Complete" registry line.
- Reconciled the **IDE** development version to `0.2.0-dev` in `ide/VERSION.md`
  to match `ide/sleela-intellij/gradle.properties`.
- Corrected the stale **Java Authorship Transition Gate** header above
  (was 1.17-dev) to the body's current `1.19-dev`.

**Java Authorship Transition Gate:** 1.19-dev

## 0.3.24 Development Increment

Library expansion and version-control/inventory reconciliation. Advanced the
active development line to 0.3.24-dev.

### Library (`/lib`) — modular multi-language compiler framework

- Added a modular framework to the `compiler` package for building compilers for
  any publicly known programming language. A developer adds a language by writing
  one small SLeeLa front end that extends a common contract; many independent
  front ends register into one shared catalog with no per-language allow-list.
  New SLeeLa units: `SLLanguageCompiler`, `SLCompileRequest`, `SLCompilePlan`,
  `SLCompilerRegistry`, and seven modular front ends under
  `lib/compiler/frontends/<lang>/` (C, C++, Java, Python, JavaScript, Rust, Go).
  Each front end lowers its own language toward the common SLeeLa IR and onward
  to a VM-ready artifact. The framework identifies, plans, and reports only — it
  never executes an input program; compilation is not execution. New native
  support crosses the explicit VM/OS bridge: a stable C ABI
  (`lib/compiler/include/sleela_langc.h` / `src/sleela_langc.c`), a C++
  orchestration facade (`sleela_langc.hpp` / `.cpp`), the `.sleela` bridge
  (`sleela_lang_bridge.h` / `.c`), and a behavioral self-test
  (`tests/langc_selftest.c`). See `lib/compiler/MULTI-LANGUAGE.FRAMEWORK.md` and
  tutorial `lib/compiler/tutorials/04-building-a-language-front-end.md`.

### Inventory reconciliation (`/lib`)

- Regenerated `lib/LIBRARY.SYMBOLS.md` from the live `/lib` tree
  (`collection-revision: 2.1`) and reconciled every count across the repository
  so each record agrees: **83** package families; **10,291** `.sleela` source
  classes + **55** `SLPackage.sleela` module facades = **10,346** total symbol
  records (10,330 -> 10,346; +16 units). The increment folds in the compiler
  framework's 12 new units plus four previously-uncounted units from the
  intervening operating-system system-call work (`os/SLLinuxOS`, `os/SLMacOS`,
  `os/SLWindowsOS`, `vm/SleelaVMSystemCallBridge`).
- Updated the matching records: `lib/LIBRARY.INDEX.md` (Revision 0.30),
  `test-suites/test-library-inventory.sh` (`EXPECTED_*` totals), `SST.model` and
  `SST-2.0.model` (Revision 2.0.3) current-inventory lines, and the informative
  mirror appendices in `impl/nordshrift/SST.SYMBOLS.md`,
  `impl/nordshrift/NORDSHRIFT.SYMBOLS.md`, and `SLEELA.syntax` (now 10,346 / 83).
- Counts are verified in lockstep (filesystem == manifest header == manifest
  body) by `test-suites/test-library-inventory.sh`.

**Java Authorship Transition Gate:** 1.19-dev

**SLeeLa — MEARVK LLC — 2026**

## 0.3.25 Development Increment

Library expansion and inventory recount. Advanced the active development line to
0.3.25-dev.

### Library (`/lib`) — Skya telephony emblematic Master Classes

- Promoted the emblematic Skya telephony modules into first-class `/lib`
  **Master Classes**, one class per file under `lib/telephony-skya/`: `Socio`
  (social fabric), `Network` (transport reachability), `Servers` (server-side
  presence), `Communication` (message exchange), `RealAcquaintances` (confirmed
  trust roster), and the `SkyaModules` loader. These were previously facade
  stubs / top-level-only runnables; as `/lib` source units they are now counted
  and discoverable as Master Classes. The matching stubs were removed from
  `telephony-skya/SLPackage.sleela` to avoid duplicate class declarations; the
  runnable orchestration examples remain under `telephony-skya/sleela/`.

### Inventory recount (`/lib`)

- Regenerated `lib/LIBRARY.SYMBOLS.md` from the live `/lib` tree and reconciled
  every record so each agrees: **83** package families; **10,297** `.sleela`
  source classes + **55** module facades = **10,352** total symbol records
  (10,346 -> 10,352; +6 units). Repository-wide SLeeLa source files: **10,583**.
- Updated the matching records: `lib/LIBRARY.INDEX.md` (Revision 0.31),
  `markdown/CLASS.INVENTORY.md` (Revision 2.3), `test-suites/test-library-
  inventory.sh` (`EXPECTED_*` totals), `SST.model` and `SST-2.0.model`
  (Revision 2.0.4) current-inventory lines, `lib/README.md`, and the informative
  mirror appendices in `impl/nordshrift/SST.SYMBOLS.md`,
  `impl/nordshrift/NORDSHRIFT.SYMBOLS.md`, and `SLEELA.syntax` (now 10,352 / 83).
- Counts verified in lockstep by `test-suites/test-library-inventory.sh`.

**Java Authorship Transition Gate:** 1.19-dev

**SLeeLa — MEARVK LLC — 2026**
