# SLeeLa command workflow

These examples distinguish source validation, source execution, artifact creation, artifact verification, and interoperability front ends. Run them from the repository root after building the toolchain.

## Global options

These appear before the command:

```sh
sleela --memory-manager run program.sleela
sleela --mm=512M run program.sleela
```

The memory-manager option enables the memory manager with its default 2 GiB limit or a specified byte limit.

## Source and artifact commands

| Purpose | Command | What it does |
|---|---|---|
| Inspect tool version | `sleela version` | Reports the compiler/runtime version |
| Validate source | `sleela check path/to/program.sleela` | Resolves imports and checks source without running |
| Run source or artifact | `sleela run path/to/program.sleela` | Compiles and executes supported source or runs an artifact |
| Compile artifact | `sleela compile path/to/program.sleela -o build/program.sleela` | Writes a persistent runnable Core artifact |
| Verify artifact | `sleela validate-artifact build/program.sleela` | Validates the runnable artifact ABI and format |

## Interoperability front ends

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

## Other documented commands

The command manual also documents these operational commands. Read the manual before invoking commands that change machine state or provision services.

- `sleela http-server <1|2|3> [options]` — serve over HTTP/1, HTTP/2, or HTTP/3.
- `sleela vm list|select|info|run ...` — inspect and select VM generations.
- `sleela modium scan|status|restore ...` — module-integrity scan/status/restore.
- `sleela registry review|get|set|feature|save|export ...` — inspect and manage runtime registry settings.
- `sleela native [options] program [args...]` and `sleela exec ...` — execute with native configuration.
- `sleela design-activity ...` — report a design-activity score.
- `sleela defender detect|fetch|build|install|provision ...` — OS Defender provisioning; mutating/privileged operations require explicit opt-in.
- `sleela docs [package] [ClassName]` and documented `--generate`, `--live`, and `--facade` modes — terminal documentation.

These are command-family summaries, not copy-and-paste authorization. Consult `man sleela` for each exact argument grammar, global option placement, safety requirements, and environment variables.

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

The authoritative supported syntax range is defined by `impl/frontend/version.h` (currently **1.0–2.8**) and displayed by `sleela version`. Some older README text may lag behind the current compiler source.
