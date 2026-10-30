# Compiler examples — what to learn here

These examples teach the **compiler workflow** through its public command-line
interface: how to check a source, build an artifact, run it, combine several
sources, and validate a shipped artifact. They are command guides (`.md`), not
`.sleela` programs, because the lesson is *how to drive the toolchain*.

Read them in order.

## The three cases

| Level | File | The one new idea |
|---|---|---|
| Basic | [`BASIC-check-source.md`](BASIC-check-source.md) | `sleela check` — parse and type-check without running. |
| Moderate | [`MODERATE-compile-and-run-artifact.md`](MODERATE-compile-and-run-artifact.md) | `sleela compile -o` then `sleela run` — build once, run many. |
| Advanced | [`ADVANCED-multi-source-and-validate.md`](ADVANCED-multi-source-and-validate.md) | Multi-file builds and `sleela validate-artifact` before release. |

The original [`CLASSIC-source-validation-workflow.md`](CLASSIC-source-validation-workflow.md)
walks the whole check → compile → run sequence.

## What we want you to take away

1. **`check` early and often.** It is the fastest feedback loop the compiler
   offers — syntax and semantic errors with file/line/column, no execution, no
   artifact. Make it a habit before you run anything.
2. **Compile and run are separate steps.** `sleela compile ... -o build/x.sleela`
   produces a runnable artifact; `sleela run build/x.sleela` executes it. This
   separation is what makes builds repeatable and shippable.
3. **Validate before you trust an artifact.** `sleela validate-artifact` confirms
   an artifact is well-formed and passes the integrity gate *without* running it
   — the right final check in a release pipeline.
4. **Two binaries, two roles.** `sleela` is the runtime/command driver; `sleelvac`
   is the separate compiler product (see the [`sleelvac`](../sleelvac/) folder).
   Confirm compiler-only flags with `sleelvac --help` rather than assuming the
   subcommands match.

## A note on the integrity gate

Before any build or execution, the toolchain verifies the source tree against a
SHA-256 manifest and **fails closed** on any mismatch. This is why the antivirus
and crypto examples in this catalogue emphasize the same discipline — it is the
same gate protecting the compiler itself.

## Try it

```sh
sleela check   lib/examples/sleela/CLASSIC-hello-world.sleela
sleela compile lib/examples/sleela/CLASSIC-hello-world.sleela -o build/hello.sleela
sleela run     build/hello.sleela          # prints: Hello, SLeeLa.
sleela validate-artifact build/hello.sleela
```
