# SLeeLa Nordshrift Library Package

`/lib/nordshrift` is the canonical SLeeLa source-facing representation of the Nordshrift `.sst` transpiler and semantic coordination layer.

## Source-time role

`NordshriftSource` represents an SST sheet after it has been selected for resolution. It records the canonical library root, target, resolved package/symbol counts, and the resulting `.sleela` source artifact.

## Compile-time role

`NordshriftCompilerBridge` defines the handoff from resolved Nordshrift output into the ordinary SLeeLa compiler:

`.sst -> Nordshrift -> /lib discovery -> .sleela -> SLeeLa Compiler -> SLeeLa IR -> SLVM/SLJVM`

The resulting `.sleela` source uses the same recursive `/lib` discovery as ordinary SLeeLa source.

## Authority

Nordshrift remains the `.sst` semantic/transpilation layer. The SLeeLa compiler remains the compiler for the resulting SLeeLa source. `/lib` remains the canonical source collection.

## Symbol resolution

Nordshrift consumes SST declarations as symbols through `NordshriftSymbol`, `NordshriftSymbolResolver`, and `NordshriftSymbolTable`. Native resolution uses the shared `/lib` index; no duplicate package registry is introduced.
