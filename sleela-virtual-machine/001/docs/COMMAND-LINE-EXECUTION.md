# SLeeLa Command-Line Execution Modes

The `sleela` command-line program supports both forms of `.sleela` execution that belong to the authoritative SLeeLa toolchain:

1. **Native source execution** — provide a textual `.sleela` source file directly to `sleela run`. The command performs the authoritative SLeeLa version check, lexing, parsing, semantic processing, and compilation, then executes the resulting program in the native C Core SLVM in the same process.
2. **Persistent SLVM artifact execution** — compile `.sleela` source into a persistent Core execution artifact, then run that artifact with the same `sleela` command. The loader recognizes the artifact format, validates its ABI, and sends the loaded program to the Core SLVM without lexing or parsing it again.

These are two command-line input modes, not two different SLeeLa languages.

## Source mode

```sh
./impl/build/sleela run examples/versioned.sleela
```

The authoritative path is:

`.sleela source → version resolution → Lexer → Parser → AST → semantic/compiler lowering → Core instruction representation → SLVM → runtime services/native boundaries`

The source is compiled in memory for execution. It does **not** use a parallel grammar or a second SLeeLa interpreter.

## Persistent SLVM artifact mode

Compile a source file:

```sh
./impl/build/sleela compile examples/versioned.sleela -o build/program.sleela
```

Then execute the resulting artifact:

```sh
./impl/build/sleela run build/program.sleela
```

The `run` command distinguishes a persistent Core artifact from textual source by the artifact format/magic. A valid artifact follows:

`.sleela source → authoritative compiler → persistent Core artifact → artifact validation/loader → SLVM`

The artifact contains the executable Core representation required by the existing SLeeLa Core ABI. It is not reparsed as source.

Validate an artifact without executing it:

```sh
./impl/build/sleela validate-artifact build/program.sleela
```

## Why both modes use SLVM

The phrase **native SLeeLa execution** means that the SLeeLa command-line executable uses its native C/C++ toolchain and Core runtime rather than launching Java or another language runtime.

The phrase **SLVM execution** identifies the execution engine receiving the compiled Core representation.

Therefore:

- `sleela run source.sleela` = native SLeeLa source-to-SLVM execution in one process.
- `sleela compile source.sleela -o artifact.sleela` = source-to-persistent-SLVM-artifact compilation.
- `sleela run artifact.sleela` = persistent artifact-to-SLVM execution.

The native command-line path and the SLVM are complementary layers, not competing interpreters.

## `.sleela` file extension

The `.sleela` suffix is used by the project for both textual SLeeLa source and the persistent runnable Core artifact format. The command-line loader therefore checks the file representation before choosing source parsing or artifact loading.

## `.sst` / Nordshrift

`.sst` input remains a separate source/control-sheet path:

`.sst → Nordshrift → shared SLeeLa frontend/compiler representation → Core artifact or SLVM execution`

It does not introduce a second execution engine.

## Authoritative implementation boundary

The command-line execution path is anchored in:

- `impl/frontend/driver.cpp` — `sleela` command-line dispatch;
- `impl/frontend/lexer.*` — source lexing;
- `impl/frontend/parser.*` — source parsing;
- `impl/frontend/compiler.*` — lowering/code generation;
- `impl/core/` — native C Core and stable execution ABI;
- `sleela-virtual-machine/` — dedicated SLVM architecture, quality, capability, security, and source-to-VM integration work.

The VM documentation must remain consistent with this boundary: it must not describe a parallel SLeeLa grammar or an independent language interpreter.

Copyright (c) Max Rupplin - MEARVK LLC - 2026
