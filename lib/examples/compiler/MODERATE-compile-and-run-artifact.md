# Compiler example (moderate): compile an artifact, then run it

Once a source checks cleanly, compile it into a runnable artifact and execute the
artifact separately. Separating "compile" from "run" is what lets you build once
and run many times.

```sh
# 1. Compile source -> artifact
sleela compile lib/examples/sleela/CLASSIC-hello-world.sleela -o build/hello.sleela

# 2. Run the compiled artifact
sleela run build/hello.sleela
```

Expected output from the run step:

```
Hello, SLeeLa.
```

Notes:

- `-o <path>` names the output artifact. Create the `build/` directory first if
  your shell does not.
- `sleela run` also accepts a source file directly (it compiles in memory), but
  compiling to an artifact first is the pattern you want for repeatable builds.
- Before build or execution the toolchain performs a SHA-256 integrity check of
  the tree; a failure stops the operation (fail closed).
