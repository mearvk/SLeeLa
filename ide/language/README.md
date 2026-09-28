# IDE Language Layer

The language layer defines the IntelliJ-facing representation of SLeeLa.

The repository already contains a C++ lexer and recursive-descent parser under impl/frontend. The lexer exposes token kind, text, line and column; the parser builds the SLeeLa AST. The IDE must use those definitions as its semantic authority.

Components:
- language/file-type registration;
- lexer adapter;
- parser definition;
- PSI elements;
- references and symbol resolution;
- syntax highlighting;
- inspections;
- formatter;
- documentation provider.

Generated parser technology may be used for the IDE layer, but its grammar must remain conformant with the compiler grammar and be tested against the SLeeLa corpus.
