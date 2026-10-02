<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa SST Library Package

`/lib/sst` is the canonical SLeeLa source-facing representation of the SST (`.sst`) Scripting Sheet layer.

## Source-time role

SST remains the declarative authoring/control surface. `SSTSource` records the source sheet and its selected library root, target, profile, and discovered library inventory. `SSTLibrary` represents the source-time relationship with the canonical `/lib` tree.

## Compile-time role

The SST path is:

`.sst -> Nordshrift -> .sleela -> SLeeLa Compiler -> SLeeLa IR -> SLVM/SLJVM`

SST does not become a second implementation language and does not maintain a duplicate package registry.

## Authority

The filesystem-backed `/lib` collection and its shared compiler/Nordshrift index are authoritative. These SLeeLa classes provide the source-level contract used to represent that relationship.

## Symbol contract

SST classes and declarations are compiler symbols. `/lib/sst` provides `SSTSymbol`, `SSTSymbolKind`, `SSTSymbolReference`, and `SSTSymbolTable` for package-qualified identity, lifecycle, and resolution state. See `impl/nordshrift/SST.SYMBOLS.md`.

## Tutorials and examples

### Tutorials

- `tutorials/01-basic-symbol.sst.md`
- `tutorials/02-library-symbols.sst.md`
- `tutorials/03-symbol-table.sst.md`
- `examples/basic-symbols.sst`
- `examples/cross-package.sst`
- `examples/symbol-resolution.sst`