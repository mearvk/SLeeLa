# Java references in the frontend (Kiro settings for this repo)

Repo-scoped, always-on note.

## Use `impl/frontend/java_references.{h,cpp}`

Java name references in the SLeeLa frontend, currently `extends` / superclass
resolution, live in **one module**: `impl/frontend/java_references.cpp`
(header `java_references.h`, namespace `sleela::javaref`). It is linked at
compile time through `FE_SRC` in `impl/Makefile`.

- Any code that follows a class `extends` edge (semantic analysis, instance
  field layout, runtime parent registration, method aliasing, Sleela.home
  lineage) MUST call `javaref::superclassOf(...)` or `javaref::resolveBaseName(...)`.
  Do NOT read `cls.java.superclass` directly: `semantic.cpp` and `compiler.cpp`
  would then disagree on `extends java.lang.X` (check passes, run loses fields).
- A qualified name (`java.lang.Throwable`) falls back to its simple name only
  for classes loaded from `/lib` (`fromLibrary`); a local class cannot
  impersonate a JDK class.
- Java type relations also live here: `javaref::isSubtypeOf` (extends +
  implements, used by the semantic `assignable` rule and by virtual dispatch)
  and `javaref::inheritanceDepth` (deepest-first dispatch order).
- New Java name-reference helpers go in this file, not inline in
  `semantic.cpp` / `compiler.cpp`.

## Sleela.home lineage lives here too

The Sleela.home lineage logic (SLEELA.syntax section 42) is in the same module:
`javaref::lineageParentOf`, `javaref::lineageRootOf`, `javaref::lineageValue`.
`compiler.cpp` only keeps a one-line `lineageValue(x)` wrapper that passes its
`lineageClasses_` and `currentClass_`. Change lineage behavior in
`java_references.cpp`, not in `compiler.cpp`. Regression test:
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
