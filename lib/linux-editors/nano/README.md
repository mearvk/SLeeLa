# SLeeLa color schemes for nano

nano highlights via `nanorc` files. Each `sleela-<theme>.nanorc` carries the
full SLeeLa syntax rules painted in that palette's 24-bit colours (nano >= 5.0).

## Install

```sh
mkdir -p ~/.nano
cp lib/linux-editors/nano/sleela-kiro.nanorc ~/.nano/sleela.nanorc
echo 'include "~/.nano/sleela.nanorc"' >> ~/.nanorc
```

Swap `sleela-kiro.nanorc` for the more potent `sleela-sleela.nanorc`, or for
any regional palette: `sleela-american`, `sleela-eurasia`,
`sleela-russian-slavic`, `sleela-silk-road`, `sleela-east-asian`,
`sleela-greek`, or `sleela-turkish`.

> nano colours the *buffer* (syntax), not the whole UI chrome; the palette files
> use each scheme's keyword/type/string/comment/number hues so the code reads in
> the chosen scheme. On nano < 5.0 (no 24-bit colour), replace the `#RRGGBB`
> values with nano's named colours (e.g. `brightblue`, `cyan`, `yellow`).

## Ed / Sed

`ed` and `sed` are non-interactive line editors with **no** colour-scheme or
syntax-highlighting concept, so there is nothing to theme for them. Use them for
scripted `.sleela` edits; see the top-level `linux-editors/README.md`.
