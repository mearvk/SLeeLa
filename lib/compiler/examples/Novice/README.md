# Novice — HelloCompiler

**Goal:** understand the smallest useful compiler pipeline.

Recognize a tiny source statement such as `say "Hello, SLeeLa!"`. The grammar is `statement := "say" string`. Scan the keyword and quoted string, reject missing keywords or unterminated strings, and emit a normalized record only when parsing succeeds.

See [HelloCompiler.sleela](HelloCompiler.sleela). It is a learning scaffold; verify its syntax against the current compiler version before compiling it.

## Exercises

- Accept `say "Hello, SLeeLa!"`.
- Reject a missing string, unterminated quote, or unknown keyword.
- Include a source position in diagnostics.
- Prove that an invalid statement cannot produce output.

**Done when:** you can explain scanning, parsing, and emitting and show one passing and three failing cases.
