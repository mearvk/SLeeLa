# Nordshrift Tutorial 01 — SST to SLeeLa

Trace an SST document through Nordshrift into SLeeLa.

~~~text
.sst
  -> parse
  -> SST symbols
  -> /lib resolution
  -> Nordshrift symbols
  -> .sleela
  -> SLeeLa compiler
~~~

Example:

~~~sst
sheet HelloLibrary {
  use sst.SSTSymbol;
  target sleela;
}
~~~

Nordshrift resolves sst.SSTSymbol through the shared library index before emitting SLeeLa.

## Exercise

Replace sst.SSTSymbol with another known /lib class and trace the resolution path.
