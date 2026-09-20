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
against the current directory, then `examples/`, then `impl/examples/`.

```sh
SLeeLa hello                     # runs hello.sleela (from examples/impl-examples)
SLeeLa examples/hello.sleela     # runs an explicit path
SLeeLa run object                # explicit run form
SLeeLa check file.sleela         # validate (incl. #sleela version), don't run
SLeeLa compile src.sleela -o out.sleela   # compile a Wrapper to an artifact
SLeeLa native /usr/bin/env       # run a native OS executable from the terminal
SLeeLa exec ./tool --flag value  # `exec` is an alias of `native`
SLeeLa version                   # version + supported syntax range
SLeeLa help                      # usage
```

The launcher resolves the built `sleela` binary (an `SLEELA_BIN` override, a
copy alongside the script, or `impl/build/sleela`), and sets the `SHEET.sheet`
catalog and the SHA-256 execution-gate manifest that the runtime requires,
before handing off to the binary and preserving its exit status.

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

Build the runtime first if needed:

```sh
make -C impl                     # produces impl/build/sleela
```

To make `SLeeLa` available everywhere, put `bin/` on your `PATH` (or copy the
script and the built binary into a directory that already is). If executable
permission was lost on checkout: `chmod +x bin/SLeeLa`.

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

The implementation uses the fixed native mapping documented in [`../OS_SUPPORT.md`](../OS_SUPPORT.md): Windows uses `mearvk/Windows.Admin.Defender`, and Linux uses `mearvk/Linux.Admin.Defender`.

After checkout, if executable permissions were not preserved by the checkout method, run:

```sh
chmod +x bin/OSsupport
```

The command does not bypass Secure Boot, UAC, Defender, driver signing, or other operating-system security controls.
