# Compiler example (basic): check a source file

The cheapest, safest thing the compiler does: parse and type-check a source file
**without executing it**. Use this constantly while writing code — it catches
syntax and semantic errors in milliseconds.

```sh
sleela check lib/examples/sleela/CLASSIC-hello-world.sleela
```

Expected: a line like

```
lib/examples/sleela/CLASSIC-hello-world.sleela: ok (syntax declared 1.7)
```

If the file has an error, `check` prints the file, line, and column and exits
non-zero. Nothing is run and no artifact is written. This guide uses only the
public commands documented in the language reference; it does not invent flags.
