# SLeeLa command workflow

These examples distinguish source validation, source execution, artifact creation, artifact execution, and interoperability front ends. Run them from the repository root after building and installing the toolchain.

## Runtime and source commands

| Purpose | Command | What it does |
|---|---|---|
| Inspect tool version | `sleela version` | Reports the runtime product version |
| Validate source | `sleela check path/to/program.sleela` | Parses and validates without running |
| Run source | `sleela run path/to/program.sleela` | Compiles and executes source |
| Compile artifact | `sleela compile path/to/program.sleela -o build/program.sleela` | Writes a persistent runnable Core artifact |
| Run artifact | `sleela run build/program.sleela` | Executes the compiled artifact |
| Run SecureJDK input | `sleela run path/to/program.xclass` | Ingests and runs an `.xclass` input |

## Interoperability front ends

The language reference documents these command forms:

```sh
sleela xclass --info path/to/program.xclass
sleela xclass --emit path/to/program.xclass
sleela xclass --run path/to/program.xclass

sleela langin --info path/to/source-file
sleela langin --emit-sleela path/to/source-file
sleela langin --emit-xclass path/to/source-file
sleela langin --run path/to/source-file

sleela nordshrift --target=sleela path/to/program.sleela
sleela nordshrift --target=java path/to/program.sleela
sleela nordshrift --target=c path/to/program.sleela
sleela nordshrift --emit path/to/program.sleela
```

Use the installed command's `--help` output to confirm exact option ordering and supported input types for the build you have installed.

## Multi-file compilation

Both `run` and `compile` accept multiple source files. Definitions are merged into one compilation unit; duplicate definitions are diagnosed.

```sh
sleela check src/Model.sleela
sleela run src/Model.sleela src/Main.sleela
sleela compile src/Model.sleela src/Main.sleela -o build/application.sleela
sleela run build/application.sleela
```

## Sleelvac is a separate compiler product

The repository also builds a separate `sleelvac` compiler executable. Inspect its own `--help` output before using compiler-only flags. Do not assume every `sleela` runtime subcommand is an identical `sleelvac` subcommand. See the sibling `/lib/examples/sleelvac/` directory.

## Example naming convention

- `CLASSIC-`: minimal, foundational language patterns.
- `ORIGINAL-`: familiar teaching exercises and canonical algorithms.
- `USED-`: established, practical language patterns.
- `NEW-`: newer language and library capabilities.
- `DRAFT-`: proposed API vocabulary that is not yet guaranteed as a compiler intrinsic.

The `#sleela` pragma declares the source syntax level. Check the repository's current language reference before raising it or using a feature with a version gate.
