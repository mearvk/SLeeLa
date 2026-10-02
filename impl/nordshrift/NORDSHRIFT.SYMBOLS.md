# Nordshrift Symbol Resolution

Nordshrift consumes SST declarations as typed symbols before emitting SLeeLa.

## Required stages

1. Parse an SST declaration.
2. Construct an `SSTSymbol` with package-qualified identity.
3. Resolve it through the shared `sleela::library::Index`.
4. Produce a `NordshriftSymbol`.
5. Preserve source path, kind, signature, and resolution status.
6. Reject unresolved or ambiguous symbols.
7. Emit SLeeLa only after symbol resolution succeeds.

## No duplicate registry

Nordshrift must not invent a parallel `/lib` symbol database. The recursive library index remains the authoritative source for package and source-symbol existence.

## Relationship

`SSTSymbol -> SSTSymbolResolution -> NordshriftSymbol -> SLeeLa symbol/IR`

This contract applies to the new SST/Nordshrift classes and to future classes added to `/lib`.
