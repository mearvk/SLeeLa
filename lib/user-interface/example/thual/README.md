# THUAL GUI Examples

This directory contains the runnable THUAL GUI example.

- [gui-thual-effects-showcase.sleela](./gui-thual-effects-showcase.sleela)

From the repository root, run it with:

```sh
./impl/build/sleela run example/thual/gui-thual-effects-showcase.sleela
```

Or run all GUI examples with `./example/run-gui-examples.sh`.

## Platform Launchers

Run this example from the repository root:

- **Linux:** `./example/thual/run-thual-gui.sh`
- **macOS:** `./example/thual/run-thual-gui.sh`
- **Windows PowerShell:** `./example/thual/run-thual-gui.ps1`
- **Windows CMD:** `example\thual\run-thual-gui.cmd`

The launchers discover the SLeeLa executable under `impl/build` or `bin`, unless `SLEELA_BIN` is set. They default `SLEELA_SHEET` and `SLEELA_SHA256_MANIFEST` to files at the repository root. A desktop session is required to display the native GUI window.
