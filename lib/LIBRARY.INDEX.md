# SLeeLa /lib Library Index

**Revision:** 0.8  
**Packages:** 75  
**SLeeLa source units:** 10033  
**Module-facade symbols:** 88  
**Total symbol records:** 10121  
**Symbol manifest:** `LIBRARY.SYMBOLS.md`

The `/lib` tree is the canonical language-facing source collection. The compiler and Nordshrift use the same recursive library discovery implementation, so a package becomes importable when its directory contains SLeeLa source.

| Coverage | Count |
|---|---:|
| Repository module families represented under /lib | 74 |
| SLeeLa source units | 953 |
| Module-facade symbols | 88 |
| Total symbol records | 1,041 |

Every repository-level module family that is a language/runtime/package concern now has at least one SLeeLa source unit under `/lib`. Documentation, images, generated build output, tests, and CI-only directories remain non-library artifacts and are intentionally not presented as language packages.

## Compiler and Loader Resolution

Resolution is shared by the SLeeLa compiler and Nordshrift:

1. `$SLEELA_LIB`
2. `lib`
3. `../lib`
4. `../../lib`

Discovery is recursive. Package presence is therefore derived from the actual `/lib` tree rather than a hand-maintained package allow-list. `validateImports()` rejects an imported package that is not present.

The VM-facing `SLVMModuleLoader` source mirrors this contract: package names are discovered from the canonical library root, registered, and checked before use. Native loader facilities remain below the explicit C/C++ OS bridge.

## Inventory

The complete path-level and facade-level symbol collection is maintained in `LIBRARY.SYMBOLS.md`.

**Max Rupplin — MEARVK LLC — 2026**


## Compiler / Nordshrift / Loader Contract

The shared `sleela::library::Index` recursively discovers the 77 package families and all 10,046 `.sleela` source units. It now exposes package counts, per-package symbol counts, symbol lookup, and source-path resolution. Compiler and Nordshrift use this index; `lib/vm/SLVMModuleLoader.sleela` represents the same discovered package/symbol state at the SLeeLa layer.

## SST / Nordshrift Symbol Contract

SST declarations are typed symbols with package-qualified identity. Nordshrift resolves them through the same `sleela::library::Index` used by the compiler; `/lib` remains the sole authoritative symbol collection. See `impl/nordshrift/SST.SYMBOLS.md` and `impl/nordshrift/NORDSHRIFT.SYMBOLS.md`.
