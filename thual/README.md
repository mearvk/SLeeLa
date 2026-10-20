# THUAL SLeeLa GUI Example

This directory contains the THUAL SLeeLa GUI example source.

## Run

From the repository root, with the SLeeLa runtime and GUI dependencies built:

```sh
SLEELA_SHEET=./SHEET.sheet \
SLEELA_SHA256_MANIFEST=./security/sha256-manifest.json \
./impl/build/sleela run thual/gui-thual-effects-showcase.sleela
```

The original copy under `example/thual/` and other existing `.sleela` files are intentionally retained.
