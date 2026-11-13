# Compiler review: Java nativeness moved behind a language-handler module

Date: 2026-11-13. Backup of every edited file: `impl/backup/compiler-java-handler-2026-11-13/`.
Spec home: Java Specific Section, `lib/java/JAVA.SPEC.md` (not the SLeeLa language spec).

## Rule

The compiler (lexer, parser, semantic analysis, code generation, driver), the VM and the
runtime must not contain Java logic. Java-referencing and Java-authored documents are served
by a **handler module** that the compiler reaches only through a neutral interface.

## Architecture (Stage 1, done)

| Piece | File | Role |
|---|---|---|
| Interface + registry | `frontend/language_handler.{h,cpp}` | `LanguageHandler` hooks; `lang::` dispatch; no language knowledge |
| Class relations | `frontend/class_relations.{h,cpp}` (`sleela::classrel`) | extends/implements, subtype test, inheritance depth, most-derived-first, `Sleela.home` lineage (SLEELA.syntax 42, a SLeeLa feature) |
| Java handler | `frontend/java/java_handler.{h,cpp}` | the only Java-aware code the compiler loads; self-registers |
| Java models | `frontend/java/java_{equivalence,flow,exceptions}.*`, `java_compat.h` | moved here unchanged (qualification models) |

Hooks: `documentMarkers` (the `spark` Preonem marker and its minimum syntax), `resolveReference`
(`java.lang.X` -> `X` only for a `fromLibrary` class), `eraseSignatureType` (type variables and
qualified types -> `Any`, `java.lang.String` -> `String`), `stringMethodLibrary`
(`JavaStringMethods`), `faultFunction` (`SLJavaFaults::make`), `libraryNeeds` (driver seeding of
`SLJavaFaults`), `validateProgram` (Java compatibility metadata).

Registration is a static `Registrar` in `java_handler.cpp`. `FE_SRC` in `impl/Makefile` lists the
handler; removing `frontend/java/java_handler.cpp` from it removes Java support and leaves
the language, VM and runtime unchanged (every hook then answers "no opinion").

## Inventory and disposition

| Touchpoint | Before | After |
|---|---|---|
| `java_references.{h,cpp}` | one module: generic relations + qualified-name fallback + spark | split: `class_relations` (generic) + handler (`resolveReference`, marker) |
| `Program::sparkMarked` (ast.h) | Java field in AST | `Program::markers` (generic set; handler claims words) |
| parser `spark` branch | hard-coded word | `lang::isDocumentMarker` |
| parser `javaErase` | Java erasure rules in parser | `Parser::eraseSignatureType` -> handler |
| semantic spark gate | `javaref::sparkDocument` | generic marker loop using the handler minimum syntax |
| semantic/compiler/driver `"JavaStringMethods"` (many) | literal | `lang::stringMethodLibrary()` |
| compiler `"SLJavaFaults::make"` | literal | `lang::faultFunction()` |
| driver `SLJavaFaults` seeding | Java policy in driver | `lang::libraryNeeds` |
| compiler `validateJavaCompatibilityMetadata` | direct include | `lang::validateProgram` |

Behaviour is unchanged: all regression suites pass (see the commit message). One message changed:
a String method with no string library now says "needs the String method library (X)".

## Still Java-flavoured in the core (documented, not moved)

* **Stage 2 (driver):** `import java.jdk-N` tier resolution (preferred roots, tie-break order) and the
  JVM host launch (`runJavaFrameworkSource`, `runJavaApplication`, `runJavaJar`, `runJavaClass`, the
  `.java` branch of `langin`). These are policy and process-launch code; they need a host-launch
  hook and a tier-resolution hook, and are the next extraction.
* **Stage 3 (VM/semantic builtins):** `javaString*` builtins (semantic.cpp/compiler.cpp) and the
  `OP_JSTR_*` opcodes/utf16 value kind (`core/sleela_core.c`, `core/sleela_java_string*.c`).
  This is a VM string kind with Java-flavoured names, not Java logic. A rename is an ISA change
  (opcode names in OSLS docs and the opcode-coverage gate), so it is deferred.
* **AST names:** `ClassDecl::java`, `Method::java`, `JavaTypeKind`, `JavaModifier` remain in
  `ast.h` as shared declaration metadata (superclass, interfaces, modifiers). The compiler
  reads only the generic parts (`superclass`, `interfaces`, `constructor`).
* **Lexer/parser surface:** Java-style declarations (`parseJavaModifiers`, generics, records) are
  accepted syntax of the SLeeLa 1.x-3.1 language, not handler behaviour.

## Verification

Handler registered: syntax31 19/19, version tests, spark, oop, lineage, converter differential
32/32, Java differential 18, runtime pressure 6, `test-sleela`, `test-nordshrift*`,
`test-exceptions`, `test-runtime`, `test-subjects`, `test-unsigned-integer`,
`test-java-string-vm`, `check-manifest`. Unit test
`impl/tests/java_references/test_java_references.cpp` builds both with the handler and with
`-DNO_HANDLER` (no marker, no string library, no qualified-name resolution).
Not done: a full compiler binary linked without the handler.
