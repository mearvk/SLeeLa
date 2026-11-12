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
- Regenerate `security/sha256-manifest.json`, rebuild `impl/build/sleela`, then run:
  `bash impl/tests/java_references/spark/run_spark_tests.sh`,
  `bash impl/tests/lineage/run_lineage_tests.sh`,
  `bash examples/java/differential/run-differential.sh` (needs a JDK), and
  `examples/java/JavaLangThrowableConformance.sleela` (`failures=0`).

## Known gap

`super(message)` in a user subclass does not forward arguments (lineage rule
42.7), so a subclass of a Throwable gets `getMessage() == null`. Fix at the
interpreter level, not with a housing class.
