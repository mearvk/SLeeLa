# MUAL SLeeLa GUI Example

This directory contains the MUAL SLeeLa GUI example source.

## Run

From the repository root, with the SLeeLa runtime and GUI dependencies built:

```sh
SLEELA_SHEET=./SHEET.sheet \
SLEELA_SHA256_MANIFEST=./security/sha256-manifest.json \
./impl/build/sleela run mual/gui-mual-text-editor.sleela
```

The original copy under `example/mual/` and other existing `.sleela` files are intentionally retained.
