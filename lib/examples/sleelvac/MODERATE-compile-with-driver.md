# Sleelvac example (moderate): the source-to-artifact workflow

For turning source into a runnable artifact, the documented, stable path is the
`sleela` command driver. Use it for check/compile/run; reach for `sleelvac`
directly only for compiler-specific tasks its `--help` describes.

```sh
# Check, compile, and run via the command driver:
./impl/build/sleela check   lib/examples/sleela/CLASSIC-hello-world.sleela
./impl/build/sleela compile lib/examples/sleela/CLASSIC-hello-world.sleela -o /tmp/hello.sleela
./impl/build/sleela run     /tmp/hello.sleela
```

Expected run output:

```
Hello, SLeeLa.
```

The driver and the compiler share one front end, so a program that `sleela
check` accepts is parsed and type-checked by the same code `sleelvac` uses. The
difference is packaging and command surface, not the language rules.
