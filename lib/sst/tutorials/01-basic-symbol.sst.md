# SST Tutorial 01 — Basic Symbol Loading

## Goal

Reference a class from the canonical SLeeLa /lib collection.

~~~sst
sheet BasicSymbols {
  use sst.SSTSymbol;
  use sst.SSTSymbolKind;
  target sleela;
}
~~~

The semantic operation is a package-qualified reference. Nordshrift constructs an SST symbol, resolves it through the shared library index, and emits SLeeLa for the compiler.

## Key rule

A short name such as SSTSymbol is not the canonical cross-package identity. Use sst.SSTSymbol.

## Exercise

Replace SSTSymbolKind with another known /lib class and trace its resolution.
