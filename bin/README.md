<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-logo-004.jpg" alt="SLeeLa">






# SLVM `bin/`

The `bin/` directory contains command-line entry points intended for use from the SLVM environment.

## SLeeLa

[`SLeeLa`](SLeeLa) is the OS-terminal launcher for the SLeeLa VM. It lets you run
a SLeeLa **object** — a `.sleela` Wrapper™ or a compiled `.sleela` artifact —
straight from the operating-system shell:

```sh
$> SLeeLa object
```

`object` may be a bare name (`hello` → `hello.sleela`), a `.sleela` path, a
compiled `.sleela` artifact, or an `.xclass` input. Bare names are resolved
against the current directory, then the repository root, then `examples/`, then
`impl/examples/`.

```sh
SLeeLa hello                     # runs hello.sleela (from examples/impl-examples)
SLeeLa examples/hello.sleela     # runs an explicit path
SLeeLa run object                # explicit run form
SLeeLa check file.sleela         # validate (incl. #sleela version), don't run
SLeeLa compile src.sleela -o out.sleela   # compile a Wrapper to an artifact
SLeeLa native /usr/bin/env       # run a native OS executable from the terminal
SLeeLa exec ./tool --flag value  # `exec` is an alias of `native`
SLeeLa keysearch                 # verify local Secret.key against the repo's
SLeeLa version                   # version + supported syntax range
SLeeLa help                      # usage
```

The launcher resolves the built `sleela` binary (an `SLEELA_BIN` override, a
copy alongside the script, or `impl/build/sleela`; on Windows the `.exe`
variants `sleela.exe` / `impl/build/sleela.exe` are also tried), and sets the
`SHEET.sheet` catalog and the SHA-256 execution-gate manifest that the runtime
requires, before handing off to the binary and preserving its exit status.

### Platform support

Both scripts are POSIX `sh` and run on **Linux**, **macOS**, and **Windows 10+**:

- **Linux / macOS** — run directly from any POSIX shell. Build the runtime
  with `make -C impl` (Linux) or `scripts/build-macos.sh` (macOS, Apple
  clang). The launcher auto-detects `impl/build/sleela`.
- **Windows 10+** — run from a POSIX shell such as **Git Bash**, **MSYS2**, or
  **WSL** (these scripts are not `cmd.exe`/PowerShell batch files). Build the
  MinGW runtime with `powershell -ExecutionPolicy Bypass -File build-windows.ps1`,
  which produces `impl/build/sleela.exe`; the launcher resolves the `.exe`
  automatically.

### Native executables and the Memory Manager

`SLeeLa native <program> [args...]` (alias `exec`) runs a native OS executable
from the terminal under a real pseudo-terminal, honoring the SHA-256 execution
gate and relaying the child's output. A leading `--memory-manager[=<size>]`
enables the SLeeLa Memory Manager (raw process-memory accounting with an
optional fail-closed hard byte limit; `<size>` accepts `K`/`M`/`G` suffixes),
which is enabled automatically for `native`/`exec`:

```sh
SLeeLa --memory-manager=64M hello        # run a Wrapper with a 64 MiB cap
SLeeLa native --memory-manager ./tool    # run a native under the manager
```

See [`../MEMORY_MANAGER.md`](../MEMORY_MANAGER.md) for the full reference.

### Startup key verification and `keysearch`

On normal startup (any VM run), the launcher performs a quiet integrity probe:
it fetches the published master-branch key over HTTPS (read-only `GET`) and
compares it, by SHA-256 digest, with the local
[`../psychiatry/Secret.key`](../psychiatry/Secret.key). A match is silent; a
connectivity problem or a mismatch prints one small note and never prevents
SLeeLa from starting. The key itself is never uploaded — only digests are
compared.

```sh
SLeeLa keysearch                 # explicit, verbose form of the same check
```

`keysearch` is the manual/verbose form of that probe and requires `python3`
(it runs [`../psychiatry/keysearch.py`](../psychiatry/keysearch.py)). It does
not run the VM, so it works even before the `sleela` binary has been built.
`help` is likewise network-free and needs no binary.

Build the runtime first if needed:

```sh
make -C impl                     # produces impl/build/sleela
```

To make `SLeeLa` available everywhere, put `bin/` on your `PATH` (or copy the
script and the built binary into a directory that already is). If executable
permission was lost on checkout: `chmod +x bin/SLeeLa`. On Windows, invoke it
through a POSIX shell (Git Bash / MSYS2 / WSL).

## OSsupport

[`OSsupport`](OSsupport) is the command-layer entry point for native operating-system support and Defender provisioning.

```sh
OSsupport
OSsupport detect
OSsupport status
OSsupport fetch [directory]
OSsupport build [directory]
OSsupport install [directory]
OSsupport provision [directory]
```

Without arguments, `OSsupport` displays the available choices and prompts for an operation. With an argument, it is directly scriptable and preserves the exit status returned by the underlying `sleela defender` command.

The implementation uses the fixed native mapping documented in [`../OS_SUPPORT.md`](../OS_SUPPORT.md):

| Host OS | Native support repository | Provisioning |
|---|---|---|
| Windows 10+ | `mearvk/Windows.Admin.Defender` | supported (elevated terminal + signed WDK driver) |
| Linux | `mearvk/Linux.Admin.Defender` | supported (kernel module via `make install`) |
| macOS | `MacOS.Admin.Defender` | **detect only** — no privileged backend is implemented |

On macOS, `OSsupport detect`/`status` report the mapping, but `fetch`, `build`,
`install`, and `provision` are refused with a clear message (no privileged
Defender backend is implemented for macOS) and a non-zero exit status. The
`sleela` runtime itself still builds, runs, and detects the host on macOS.

After checkout, if executable permissions were not preserved by the checkout method, run:

```sh
chmod +x bin/OSsupport
```

The command does not bypass Secure Boot, UAC, Defender, driver signing, or other operating-system security controls.