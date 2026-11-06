# Compiler example: source validation workflow

This example uses the public command-line interface documented in the repository language reference.

```sh
# Parse and validate a source file without executing it.
sleela check lib/examples/sleela/CLASSIC-hello-world.sleela

# Compile a runnable artifact.
sleela compile lib/examples/sleela/CLASSIC-hello-world.sleela -o build/hello.sleela

# Execute the generated artifact.
sleela run build/hello.sleela
```

Use `sleelvac --help` for the installed compiler executable's current options. This guide intentionally does not invent flags or promise that compiler-only and runtime commands are interchangeable.
