<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian/debian-logo.png" width="75" height="75" alt="SLeeLa">

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

## Tutorials and examples

### Tutorials

- `tutorials/01-basic-symbol-to-sleela.md`
- `tutorials/02-symbol-resolution.md`
- `tutorials/03-library-bridge.md`
- `tutorials/04-invalid-symbols.md`

## SLeeLa language edition — Syntax 3.0

Nordshrift uses the shared Sleelvac frontend and therefore accepts the same
SLeeLa source syntax range: **1.0–3.0**. Syntax 3.0 is the current normative
edition. The selected source pragma and feature-specific minimum gates remain
attached to each generated or consumed SLeeLa source; Nordshrift must not
relabel legacy source as 3.0.

The NS-SST-0001 control-sheet specification, the Nordshrift product release,
and the SLeeLa language syntax edition are separate versioned contracts.
Changing the SLeeLa syntax edition does not change the SST format version or
the Nordshrift product version. See SLEELA.syntax and
impl/nordshrift/NORDSHRIFT.md.
