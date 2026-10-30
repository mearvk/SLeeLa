# Sleelvac compiler invocation

SLeeLa builds the runtime/command driver and the separate `sleelvac` compiler product from the same front-end sources.

From the repository root:

```sh
make -C impl
./impl/build/sleelvac --help
```

For the source-to-artifact workflow, the documented SLeeLa command driver provides:

```sh
./impl/build/sleela check lib/examples/sleela/CLASSIC-hello-world.sleela
./impl/build/sleela compile lib/examples/sleela/CLASSIC-hello-world.sleela -o /tmp/hello.sleela
./impl/build/sleela run /tmp/hello.sleela
```

The separate `sleelvac` binary is the compiler product; `sleela` is the runtime/command driver. Confirm compiler-only flags with `sleelvac --help`; do not assume every `sleela` subcommand is an identical `sleelvac` subcommand.
