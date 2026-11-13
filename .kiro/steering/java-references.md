# Java references in the frontend (Kiro settings for this repo)

Repo-scoped, always-on note.

## Java lives in a handler module, not in the compiler

The compiler (parser, semantic, compiler, driver), the VM and the runtime carry NO
Java logic. Java-referencing documents are served by the Java handler,
`impl/frontend/java/java_handler.cpp`, reached only through the neutral interface
`impl/frontend/language_handler.h` (`sleela::lang::*`). It registers itself with a
static `Registrar` and is linked through `FE_SRC` in `impl/Makefile`; removing it from
`FE_SRC` removes Java support. Review and staged plan: `impl/frontend/COMPILER-JAVA-REVIEW.md`.

- Code that follows a class `extends` edge (semantic analysis, instance field
  layout, runtime parent registration, method aliasing, Sleela.home lineage) MUST
  call `classrel::superclassOf(...)` / `classrel::resolveBaseName(...)`
  (`impl/frontend/class_relations.{h,cpp}`). Do NOT read `cls.java.superclass`
  directly: `semantic.cpp` and `compiler.cpp` would then disagree.
- A qualified name (`java.lang.Throwable`) falls back to its simple name only for
  classes loaded from `/lib` (`fromLibrary`). That rule is the handler hook
  `resolveReference`; `classrel` offers exact match first, then the handlers.
- Type relations (`classrel::isSubtypeOf`, `inheritanceDepth`, `mostDerivedFirst`) are
  language-neutral.
- Java library-tier selection (`filterLibraryCandidates`) and the JVM launch for `sleela run` (`java/java_host.cpp`, `runHost`) are handler hooks too; `driver.cpp` has no Java policy or `java` process code.
- New Java-specific behaviour (markers, erasure, library names/seeding, validation)
  goes in `impl/frontend/java/`, as a hook on `LanguageHandler`. Do NOT add
  `Java`/`java` names, literals or branches to `parser.cpp`, `semantic.cpp`,
  `compiler.cpp` or the VM; add a neutral hook instead.

## Sleela.home lineage lives in class_relations

The Sleela.home lineage logic (SLEELA.syntax section 42, a SLeeLa feature) is in `class_relations`:
`classrel::lineageParentOf`, `classrel::lineageRootOf`, `classrel::lineageValue`.
`compiler.cpp` only keeps a one-line `lineageValue(x)` wrapper that passes its
`lineageClasses_` and `currentClass_`. Change lineage behavior in
`class_relations.cpp`, not in `compiler.cpp`. Regression test:
`bash impl/tests/lineage/run_lineage_tests.sh` (must print `lineage regression: PASS`).

## Backup and tests

- Pre-change originals: `impl/backup/java-references-2026-11-12/` (README has
  restore steps; verify with `sha256sum -c SHA256SUMS`).
- Unit test: `impl/tests/java_references/test_java_references.cpp`.
- Editing `impl/frontend/**` requires regenerating `security/sha256-manifest.json`
  (`python3 tools/generate-sha256-manifest.py --root . --output security/sha256-manifest.json`)
  and `cd impl && make check-manifest` must print OK.

## Not yet moved

The rest of the Java code (`java_compat.h`, `java_equivalence`, `java_exceptions`,
`java_flow`, the `javaString*` builtins in `compiler.cpp`, Java launch in
`driver.cpp`) is unchanged.
