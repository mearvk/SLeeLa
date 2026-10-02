# SST Symbol Contract

**NS-SST-0001 — SST Symbol Resolution Contract**

An SST class or other named declaration is a compiler symbol, not merely text metadata.

## Identity

The canonical identity is `<package>.<symbol>`.

Examples: `sst.SLPackage`, `sst.SSTSymbol`, `nordshrift.NordshriftSymbol`, `vm.SleelaVMCompilerManager`.

Short names are never sufficient for cross-package resolution.

## Lifecycle

`declared -> resolved -> emitted`

A declaration that cannot resolve remains unresolved and is a compile diagnostic. An ambiguous reference is never silently selected.

## Authority

The canonical filesystem-backed `/lib` collection and `sleela::library::Index` are authoritative. SST does not maintain a second package or symbol registry.

## Symbol kinds

The normative kinds are: `class`, `interface`, `function`, `field`, `constant`, `variable`, `module`, `package`, `type`.

## Compile path

`.sst -> SST symbol table -> Nordshrift resolver -> /lib Index -> Nordshrift symbol table -> .sleela -> SLeeLa compiler`

The SLeeLa classes in `/lib/sst` and `/lib/nordshrift` are the source-level contracts; `impl/nordshrift/sst_symbol.*` is the native resolution boundary.
