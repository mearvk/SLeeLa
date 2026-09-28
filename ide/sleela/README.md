[![SLeeLa](https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png)](https://github.com/mearvk/SLeeLa)
# SLeeLa Language Integration

This is the first-class language integration.

Version: 0.2.0-dev

File type: .sleela

Source version is declared by the SLeeLa version pragma and validated using the same rules as the compiler.

Compiler path:
```
version -> lexer -> parser -> AST -> semantic analysis -> lowering -> runtime
```

IDE path:
```
file -> compiler-backed IntelliJ lexer/parser boundary -> PSI -> compiler semantic bridge
     -> diagnostics/indexing/completion/navigation
```

The two paths must agree on source meaning.
