# SLeeLa command workflow

These command examples are deliberately separated by intent. Run them from the repository root after building and installing the toolchain.

| Purpose | Command | What it does |
|---|---|---|
| Inspect tool version | `sleela version` | Reports the runtime product version |
| Validate source | `sleela check path/to/program.sleela` | Parses and validates without running |
| Run source | `sleela run path/to/program.sleela` | Compiles and executes source |
| Compile artifact | `sleela compile path/to/program.sleela -o build/program.sleela` | Writes a persistent runnable Core artifact |
| Run artifact | `sleela run build/program.sleela` | Executes the compiled artifact |

The repository also builds a separate `sleelvac` compiler executable. Use its own `--help` output as the authority for compiler-only flags; do not assume every `sleela` runtime subcommand is an identical `sleelvac` subcommand.

## Multi-file compilation

Both `run` and `compile` accept multiple source files. Definitions are merged into one compilation unit; duplicate definitions are diagnosed.

```sh
sleela check src/Model.sleela
sleela run src/Model.sleela src/Main.sleela
sleela compile src/Model.sleela src/Main.sleela -o build/application.sleela
sleela run build/application.sleela
```

## Naming convention

- `CLASSIC-`: minimal, foundational language patterns.
- `ORIGINAL-`: familiar teaching exercises and canonical algorithms.
- `USED-`: established, practical language patterns.
- `NEW-`: newer language and library capabilities.

The `#sleela` pragma declares the source syntax level. Check the repository's current language reference before raising it or using a feature with a version gate.
