# Compiler example (advanced): multi-source builds and artifact validation

Real programs span several source files, and shipped artifacts should be
validated before they run. This example shows both.

## Compile several sources into one artifact

List every source file on the command line. Exactly one of them must declare
`void main()`.

```sh
sleela compile \
  lib/examples/nordshrift/CLASSIC-Ledgerize.sleela \
  more/Helpers.sleela \
  -o build/app.sleela

sleela run build/app.sleela
```

The compiler links the files together; a symbol defined in one file is visible to
the others. (When you `run` a source file directly, the driver auto-links sibling
`.sleela` files in the same directory, which is why library-style classes can be
validated through a sibling that has `main`.)

## Validate an artifact before running it

```sh
sleela validate-artifact build/app.sleela
```

This confirms the artifact is well-formed and passes the integrity gate without
executing it — a good final step in a release pipeline.

## Pick the right binary

- `sleela` is the runtime/command driver (`check`, `compile`, `run`,
  `validate-artifact`).
- `sleelvac` is the separate compiler product. Confirm its flags with
  `sleelvac --help`; do not assume every `sleela` subcommand exists verbatim on
  `sleelvac`. See [`../sleelvac/`](../sleelvac/).
