# DUAL GUI Examples

This directory contains the runnable DUAL GUI example.

- [gui-dual-media-studio.sleela](./gui-dual-media-studio.sleela)

From the repository root, run it with:

```sh
./impl/build/sleela run example/dual/gui-dual-media-studio.sleela
```

Or run all GUI examples with `./example/run-gui-examples.sh`.

## Platform Launchers

Run this example from the repository root:

- **Linux:** `./example/dual/run-dual-gui.sh`
- **macOS:** `./example/dual/run-dual-gui.sh`
- **Windows PowerShell:** `./example/dual/run-dual-gui.ps1`
- **Windows CMD:** `example\dual\run-dual-gui.cmd`

The launchers discover the SLeeLa executable under `impl/build` or `bin`, unless `SLEELA_BIN` is set. They default `SLEELA_SHEET` and `SLEELA_SHA256_MANIFEST` to files at the repository root. A desktop session is required to display the native GUI window.
