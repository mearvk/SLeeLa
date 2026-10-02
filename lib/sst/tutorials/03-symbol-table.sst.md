# SST Tutorial 03 — Symbol Tables

The formal SST symbol classes are SSTSymbol, SSTSymbolKind, SSTSymbolReference, and SSTSymbolTable.

## Lifecycle

~~~text
declared -> resolved -> emitted
~~~

A declaration becomes a compiler symbol with package-qualified identity. The shared /lib index resolves it. Only resolved symbols proceed to emission.

## Failure states

An unresolved symbol remains unresolved and becomes a diagnostic. An ambiguous symbol must not be silently selected.

## Exercise

Create one valid reference and one intentionally invalid package-qualified reference and compare their resolution states.
