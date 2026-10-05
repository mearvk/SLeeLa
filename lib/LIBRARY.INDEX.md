# SLeeLa /lib Library Index

**Revision:** 0.13  
**Packages:** 79  
**SLeeLa source units:** 10184  
**Module-facade symbols:** 90  
**Total symbol records:** 10274  
**Symbol manifest:** `LIBRARY.SYMBOLS.md`

> Revision 0.9 adds the new `opcodes` package family: one SLeeLa class per
> canonical VM opcode (103 classes, codes 0–102; the base 98 are OP_NOP..
> OP_AUDIO_PLATFORM) plus `SLOpcodeBase` and `SLOpcodeStream` — 105 `.sleela`
> source units. Each class carries a single opcode and honours the fetch-then-
> execute-one contract against the VM. See `opcodes/OPCODES.md`.
>
> Revision 0.10 adds the `opcodes/governance` series — 8 classes giving SLeeLa
> procedural discretion over opcode execution: a Registrar considers a program
> A→B BEFORE it runs, a Listener confirms live fit DURING, and an Event Observer
> judges the whole as a musical, ordered process AFTER (with graded verdicts,
> base-concept checks, and known-symbol-map patching). See
> `opcodes/governance/GOVERNANCE.md`.
>
> Revision 0.11 adds the `opcodes/running` sub-family — 6 classes for richer
> opcode execution: grouping (`SLOpcodeGroup`, `SLOpcodeGroupSet`), a
> conditional-reactive layer (`SLOpcodeCondition`, `SLOpcodeConditionalReactive`,
> `SLOpcodeReactorBank`) that warms/gates/runs/skips groups on program state, and
> warming (`SLOpcodeWarmer`). See `opcodes/running/RUNNING.md`.
>
> Revision 0.12 adds the new `sldocument` package family and the `.sldocument`
> format — 6 classes for an ordered, top-down document that compiles against and
> with standard SLeeLa source (`SLDocument`, `SLDocumentStep`,
> `SLDocumentAnnotation`, `SLVeritable`, `SLDocumentResult`,
> `SLDocumentCompiler`). Each step is an annotated method that runs in order and
> usually returns a single binary veritable-and-kind value. See
> `sldocument/SLDOCUMENT.md`.
>
> Revision 0.13 makes `.sldocument` a selectable compile choice and adds naming
> conventions for comparing/converting the forms: `lib/compiler/SLSourceForm` and
> `lib/compiler/SLCompileChoice` let the compiler be told to compile a `.sleela`
> or a `.sldocument`; and `sldocument/SLDocumentNaming`,
> `sldocument/SLSourceNameComparison`, and `sldocument/SLDocumentConverter`
> synthesize method names for anonymous document steps so an engineer can convert
> a `.sldocument` to a named `.sleela` for safekeeping. 5 new `.sleela` classes.

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

The shared `sleela::library::Index` recursively discovers the 77 package families and all 10,054 `.sleela` source units. It now exposes package counts, per-package symbol counts, symbol lookup, and source-path resolution. Compiler and Nordshrift use this index; `lib/vm/SLVMModuleLoader.sleela` represents the same discovered package/symbol state at the SLeeLa layer.

## SST / Nordshrift Symbol Contract

SST declarations are typed symbols with package-qualified identity. Nordshrift resolves them through the same `sleela::library::Index` used by the compiler; `/lib` remains the sole authoritative symbol collection. See `impl/nordshrift/SST.SYMBOLS.md` and `impl/nordshrift/NORDSHRIFT.SYMBOLS.md`.
