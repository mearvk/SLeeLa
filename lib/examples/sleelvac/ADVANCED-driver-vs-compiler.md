# Sleelvac example (advanced): driver vs compiler, and reading diagnostics

This example is about *judgment*: knowing which binary to use and how to read
what it tells you.

## Which binary?

| Task | Use | Why |
|---|---|---|
| Check / compile / run a program | `sleela` | The runtime/command driver with the stable, documented subcommands. |
| Compiler-product–specific options | `sleelvac` | The standalone compiler. Its flags are listed by `sleelvac --help`. |

Do **not** assume a `sleela` subcommand exists verbatim as a `sleelvac`
subcommand. They share a front end, not a command surface.

## Reading a diagnostic

Both tools report errors the same way — file, line, column, and a message:

```
sleelvac: path/to/File.sleela: Syntax error (line 14, col 16): unexpected token ']' in expression
```

How to act on it:

1. Open the named file at the given line/column.
2. Fix exactly what the message names — here, an array/new expression the parser
   did not expect in that position.
3. Re-run `sleela check` on just that file to confirm, then build.

## The integrity gate applies here too

Before building or executing, the toolchain verifies the tree against a SHA-256
manifest and refuses to proceed on a mismatch. If you intentionally edited a
covered file, regenerate the manifest with the repository's manifest tool and
re-check before building. This "fail closed" behavior is a feature, not an
obstacle — it guarantees you built what you think you built.
