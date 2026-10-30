# SLeeLa command workflow

These examples distinguish source validation, source execution, artifact creation, artifact verification, and interoperability front ends. Run them from the repository root after building the toolchain.

## Runtime and source commands

| Purpose | Command | What it does |
|---|---|---|
| Inspect tool version | `sleela version` | Reports the compiler/runtime version |
| Validate source | `sleela check path/to/program.sleela` | Resolves imports and checks source without running it |
| Run source | `sleela run path/to/program.sleela` | Compiles and executes source |
| Compile artifact | `sleela compile path/to/program.sleela -o build/program.sleela` | Writes a persistent runnable Core artifact |
| Verify artifact | `sleela validate-artifact build/program.sleela` | Validates a persistent artifact |
| Run artifact | `sleela run build/program.sleela` | Executes the compiled artifact |
| Run compiled-class input | `sleela run path/to/program.xclass` | Ingests and runs an `.xclass` input |

## Interoperability front ends

The installed manual documents these command forms:

```sh
sleela xclass --info path/to/program.xclass
sleela xclass --emit path/to/program.xclass
sleela xclass --run path/to/program.xclass

sleela langin --info path/to/source.java
sleela langin --emit-sleela path/to/source.java
sleela langin --emit-xclass path/to/source.java
sleela langin --run path/to/source.java

sleela nordshrift --target=sleela path/to/program.sleela
sleela nordshrift --target=java path/to/program.sleela
sleela nordshrift --target=c path/to/program.sleela
sleela nordshrift --emit path/to/program.sleela
sleela nordshrift --roundtrip path/to/program.sleela
```

The `langin` front end also documents Kotlin, Scala, Groovy, and Clojure source inputs. Use the installed `sleela --help` and `man sleela` output to confirm exact support in the build you are running.

## Multi-file compilation

Both `run` and `compile` accept multiple source files. Definitions are merged into one compilation unit; duplicate definitions are diagnosed.

```sh
sleela check src/Model.sleela
sleela run src/Model.sleela src/Main.sleela
sleela compile src/Model.sleela src/Main.sleela -o build/application.sleela
sleela validate-artifact build/application.sleela
sleela run build/application.sleela
```

## Sleelvac product name

The CLI manual identifies `sleela` as the authoritative compiler/runtime front end (Sleelvac™). The build also emits the separately named `sleelvac` product binary; inspect `sleelvac --help` for its current command surface rather than assuming it has a different set of subcommands.

## Example naming convention

- `CLASSIC-`: minimal, foundational language patterns.
- `ORIGINAL-`: familiar teaching exercises and canonical algorithms.
- `USED-`: established, practical language patterns.
- `NEW-`: newer language and library capabilities.
- `DRAFT-`: proposed API vocabulary that is not guaranteed to be implemented as a compiler intrinsic.

The runtime README documents supported syntax **1.3–1.7**. The examples in this starter set declare `#sleela 1.7`; check the current version output and language reference before using newer syntax.
