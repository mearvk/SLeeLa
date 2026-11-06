# Sleelvac examples — what to learn here

`sleelvac` is the standalone SLeeLa **compiler product**. These examples teach
how to build it, how it relates to the `sleela` command driver, and how to read
its diagnostics. They are command guides (`.md`) because the subject is driving
the toolchain, not writing a program.

Read them in order.

## The three cases

| Level | File | The one new idea |
|---|---|---|
| Basic | [`BASIC-build-and-help.md`](BASIC-build-and-help.md) | Build the compiler and read its own `--help` — the source of truth for its flags. |
| Moderate | [`MODERATE-compile-with-driver.md`](MODERATE-compile-with-driver.md) | Use the `sleela` driver for the stable check/compile/run path. |
| Advanced | [`ADVANCED-driver-vs-compiler.md`](ADVANCED-driver-vs-compiler.md) | Choose the right binary, read diagnostics, and respect the integrity gate. |

The original [`CLASSIC-compiler-invocation.md`](CLASSIC-compiler-invocation.md)
shows the build and the basic driver commands together.

## What we want you to take away

1. **Two binaries, one front end.** `sleela` (driver) and `sleelvac` (compiler
   product) are built from the same parser and type checker, so they agree on the
   language. They differ in packaging and command surface.
2. **Trust `--help`, not assumptions.** Because the two tools are separate
   products, the safe habit is to read `sleelvac --help` for its exact options
   rather than copying `sleela` subcommands and hoping they match.
3. **Diagnostics are addresses.** Every error names a file, line, and column.
   Treat it as a coordinate: open that spot, fix exactly what the message
   describes, re-`check`, then build.
4. **Fail closed is on your side.** The SHA-256 integrity gate stops a build when
   the tree does not match its manifest. If you edited a covered file on purpose,
   regenerate the manifest and re-check — the gate guarantees you build what you
   intend to.

## Try it

```sh
make -C impl
./impl/build/sleelvac --help
./impl/build/sleela check lib/examples/sleela/CLASSIC-hello-world.sleela
```
