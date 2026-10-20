# MUAL GUI Examples

This directory contains the runnable MUAL GUI example.

- [gui-mual-text-editor.sleela](./gui-mual-text-editor.sleela)

From the repository root, run it with:

```sh
./impl/build/sleela run example/mual/gui-mual-text-editor.sleela
```

Or run all GUI examples with `./example/run-gui-examples.sh`.

## Platform Launchers

Run this example from the repository root:

- **Linux:** `./example/mual/run-mual-gui.sh`
- **macOS:** `./example/mual/run-mual-gui.sh`
- **Windows PowerShell:** `./example/mual/run-mual-gui.ps1`
- **Windows CMD:** `example\mual\run-mual-gui.cmd`

The launchers discover the SLeeLa executable under `impl/build` or `bin`, unless `SLEELA_BIN` is set. They default `SLEELA_SHEET` and `SLEELA_SHA256_MANIFEST` to files at the repository root. A desktop session is required to display the native GUI window.
