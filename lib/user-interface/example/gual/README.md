# GUAL GUI Examples

This directory contains the runnable GUAL GUI example.

- [gui-gual-settings-console.sleela](./gui-gual-settings-console.sleela)

From the repository root, run it with:

```sh
./impl/build/sleela run example/gual/gui-gual-settings-console.sleela
```

Or run all GUI examples with `./example/run-gui-examples.sh`.

## Platform Launchers

Run this example from the repository root:

- **Linux:** `./example/gual/run-gual-gui.sh`
- **macOS:** `./example/gual/run-gual-gui.sh`
- **Windows PowerShell:** `./example/gual/run-gual-gui.ps1`
- **Windows CMD:** `example\gual\run-gual-gui.cmd`

The launchers discover the SLeeLa executable under `impl/build` or `bin`, unless `SLEELA_BIN` is set. They default `SLEELA_SHEET` and `SLEELA_SHA256_MANIFEST` to files at the repository root. A desktop session is required to display the native GUI window.
