# SLeeLa Server Launcher — sleelas

`SLeeLa` now has a native compiled server launcher named **`sleelas`**.

The launcher starts the authoritative Server Edition reference:

    server-edition/src/Server.sleela

It does not replace `Server.sleela`. The executable is the native process
boundary that locates the SLeeLa engine, establishes the Server Edition state
directories, applies the single-run lock, and starts the reference program.

## Platform targets

| Platform | Binary |
|---|---|
| Linux x86_64 | `sleelas` |
| macOS | `sleelas` |
| Windows 10+ x86_64 | `sleelas.exe` |

The source is C++17 and uses native process creation on each platform:
`fork/exec/waitpid` on POSIX and `CreateProcess/WaitForSingleObject` on
Windows.

## Engine resolution

`SLEELA_BIN` may explicitly identify the engine. Otherwise the launcher looks
for:

    bin/sleela
    impl/build/sleela

`SLEELA_ROOT` can explicitly identify the SLeeLa installation/repository root.

## Commands

    sleelas
    sleelas --tick
    sleelas --foreground
    sleelas --help

The launcher preserves the Server Edition's quiet, one-pass behavior. The
server itself remains local and does not become a public network listener.

## Compilation

Linux/macOS:

    cd api/server
    make

Windows (MinGW):

    cd api/server
    make

The GitHub Actions server-build workflow produces native compiled artifacts
for all three target platforms. Source commits contain the reproducible build
inputs; generated binaries are workflow artifacts rather than source files.

## Relationship to `serverd`

`serverd` remains the POSIX shell launcher. `sleelas` is the compiled,
cross-platform launcher for the same `Server.sleela` reference.

Both launch the same engine and server source; they do not implement separate
server semantics.
