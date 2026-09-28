# SLeeLa PSI Mapping

Version: 0.2.0-dev

PSI is an IntelliJ representation of the authoritative SLeeLa AST. It is an adapter, not a replacement AST.

## Mapping requirements
Every compiler AST node that can participate in source navigation should have a stable PSI representation or a documented transparent wrapper.

At minimum the mapping covers:
- module/file;
- imports;
- declarations;
- functions;
- parameters;
- types;
- expressions;
- statements;
- literals;
- identifiers;
- compiler source ranges.

## Source identity
PSI nodes must retain the source file and exact source range from the compiler model. This allows diagnostics and navigation to point back to the original `.sleela` text.

## Invalid source
Incomplete or invalid source must remain representable so the editor can continue operating while the compiler reports diagnostics.