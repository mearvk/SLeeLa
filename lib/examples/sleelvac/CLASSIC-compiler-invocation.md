# Sleelvac compiler invocation

The compiler is built as a separate executable. From the repository root, build it using the repository's documented build target, then inspect its own help text before using optional flags.

```sh
make -C impl build
./impl/build/sleelvac --help
```

The language reference documents the source workflow through the SLeeLa command driver:

```sh
sleela check lib/examples/sleela/CLASSIC-hello-world.sleela
sleela compile lib/examples/sleela/CLASSIC-hello-world.sleela -o /tmp/hello.sleela
sleela run /tmp/hello.sleela
```

The separate `sleelvac` binary is the compiler product; `sleela` is the runtime/command driver. Do not assume a command or flag is supported by both binaries unless their respective `--help` output confirms it.
