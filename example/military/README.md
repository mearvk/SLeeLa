# MILITARY GUI Examples

This directory contains the runnable MILITARY GUI command-console example.

- [gui-military-command-console.sleela](./gui-military-command-console.sleela)

The separate native window demo is maintained at [`/military/gui-military-window.sleela`](../../military/gui-military-window.sleela).

From the repository root, run the command console with:

```sh
./example/run-gui-military.sh
```

Or run all GUI and native-window demos with `./example/run-gui-examples.sh`.

## Platform Launchers

Run this example from the repository root:

- **Linux:** `./example/military/run-military-gui.sh`
- **macOS:** `./example/military/run-military-gui.sh`
- **Windows PowerShell:** `./example/military/run-military-gui.ps1`
- **Windows CMD:** `example\military\run-military-gui.cmd`

The launchers discover the SLeeLa executable under `impl/build` or `bin`, unless `SLEELA_BIN` is set. They default `SLEELA_SHEET` and `SLEELA_SHA256_MANIFEST` to files at the repository root. A desktop session is required to display the native GUI window.
