# Java support method (Kiro settings for this repo)

Repo-scoped, always-on. Normative source: `SLEELA.syntax` section 43.

## The method

1. **SLeeLa constructs.** Java source style is written as ordinary SLeeLa
   constructs (classes, `extends`, constructors, overloads, String methods,
   `instanceof`, casts, `try`/`catch`/`throw`, `Sleela.home` lineage).
2. **`spark` Preonem = Java style declarative.** `spark;` indicates Java style
   declarative: a document of such constructs declares itself Java style by
   carrying `spark;` at document scope before any import or class
   (`#sleela 3.0` required). It is declarative standing only: no parsing/typing/codegen change, not a reserved
   word (`spark` stays a normal identifier elsewhere). Implemented as
   `Program::sparkMarked`, parsed in `parser.cpp`, version-gated in
   `semantic.cpp`, read via `javaref::sparkDocument`.
3. **Interpreter and VM level, NO housing class.** Do NOT create a SLeeLa
   housing/wrapper/dispatcher class to carry Java meaning. Put Java support in
   the frontend (`impl/frontend/java_references.cpp` first, see
   `java-references.md`; compiler; semantic) and in VM opcodes
   (`OP_JSTR_*`, `OP_INSTANCEOF`, `OP_CHECKCAST`, `OP_TRY_PUSH/POP`, `OP_THROW`).
4. **Keep the artifacts, relative to the document.** Keep the Java-style
   supporting artifacts as they are and read them relative to the using
   document and their own documents: `/lib/java` units (`javaType` /
   `javaModule` / `javaSource` fields and header line, SLEELA.syntax 38.4),
   `lib/java/{README,STATUS,COVERAGE,JDK28-SOURCE-ENVELOPE}.md`, and
   `examples/java/differential/`. They are reference material and test
   oracles, not runtime housing.

## When changing Java support

- Document any new behavior in SLEELA.syntax section 43 first (43.4 table,
  43.5 not-done list); `spark` must stay declarative unless 43 says otherwise.
- Back up edited frontend files under `impl/backup/<topic>-<date>/` with
  `BASE-COMMIT.txt` and `SHA256SUMS`.
- Regenerate `security/sha256-manifest.json`, rebuild `impl/build/sleela`, then run
  the fast checks and the program you changed. The full list below is the
  occasional suite (see `validation-cadence.md`): run it only when asked, at a
  milestone, or after a broad compiler/VM change:
  `bash impl/tests/java_references/spark/run_spark_tests.sh`,
  `bash impl/tests/java_references/oop/run_oop_tests.sh` (sets `SLEELA_LIB`;
  without it, runs from under `impl/tests` cannot find `/lib`),
  every `examples/java/Java*.sleela` conformance program (`failures=0`),
  `bash impl/tests/lineage/run_lineage_tests.sh`,
  `bash examples/java/differential/run-differential.sh` (needs a JDK), and
  `examples/java/JavaLangThrowableConformance.sleela` (`failures=0`).

## Java object behavior (SLEELA.syntax 43.6, syntax 3.0)

Implemented in the frontend, no housing class, no new opcode: subtype
assignment and `implements`; `super(args)` calls the base constructor
(`super("constructor")` stays lineage-only); `this(args)`; implicit `super()`
and default constructors for the program's OWN classes only (`/lib` units
construct themselves; their unimplemented constructors throw); inherited field
initializers; virtual dispatch lowered to `DUP`/`INSTANCEOF`/`JMPF`/`CALL`;
`super.m()`; bodiless abstract/interface methods; enhanced for over arrays and
`iterator()` objects (`ForEachStmt`); numeric literal suffixes `L`/`d`/`f` and `_`.
Typed / multi `catch` and `finally` lowered on the existing exception opcodes
(no VM change); `++`/`--` everywhere (lexer: `-` continues an identifier only
before an identifier character).

## Still not supported (SLEELA.syntax 43.5)

`enum`, return/break/continue out of a finally-protected block, non-static
`protected` methods, runtime `getClass().getName()` (a user Throwable subclass
prints the library class name). Add these at the interpreter/VM level.
