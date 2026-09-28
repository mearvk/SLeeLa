# SLeeLa Language Integration

This is the first-class language integration.

File type: .sleela

Source version is declared by the SLeeLa version pragma and validated using the same rules as the compiler.

Compiler path:
```
version -> lexer -> parser -> AST -> semantic analysis -> lowering -> runtime
```

IDE path:
```
file -> IntelliJ lexer/parser/PSI -> compiler semantic bridge
     -> diagnostics/indexing/completion/navigation
```

The two paths must agree on source meaning.
