# Backup: before java_references.cpp (2026-11-12)

Pre-change copies of the files edited when Java `extends` / superclass
resolution was contained in `impl/frontend/java_references.{h,cpp}`.

- `frontend/semantic.cpp`, `frontend/compiler.cpp`, `Makefile` -- originals
- `BASE-COMMIT.txt` -- git commit they were taken from
- `SHA256SUMS` -- verify with `cd impl/backup/java-references-2026-11-12 && sha256sum -c SHA256SUMS`

Restore: copy the three files back over `impl/`, delete
`impl/frontend/java_references.*`, then regenerate `security/sha256-manifest.json`.
(Git history also holds the originals at the commit in BASE-COMMIT.txt.)

## Addendum: Sleela.home lineage

`lineageParentOf` / `lineageRootOf` / `lineageValue` were then moved from
`compiler.cpp` into `java_references.cpp` (`javaref::lineage*`). The original
`frontend/compiler.cpp` here already contains the pre-move versions.

## Addendum: spark Preonem (`spark/`)

`spark/` holds the pre-change `ast.h`, `parser.cpp`, `semantic.cpp`,
`java_references.{h,cpp}` and `SLEELA.syntax` (base commit and SHA256SUMS inside)
from before the `spark` marker and SLEELA.syntax section 43 were added.
