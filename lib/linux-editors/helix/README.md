# SLeeLa color schemes for Helix

Helix is modal with built-in Tree-sitter/LSP; these files give it the SLeeLa
language definition and the SLeeLa palettes.

## Install

```sh
mkdir -p ~/.config/helix/themes
# merge this language block into your languages.toml (or copy if you have none)
cat lib/linux-editors/helix/languages.toml >> ~/.config/helix/languages.toml
cp lib/linux-editors/helix/themes/*.toml ~/.config/helix/themes/
```

Then in `~/.config/helix/config.toml`:

```toml
theme = "sleela-american"
```

Themes: `sleela-american`, `sleela-eurasia`, `sleela-russian-slavic`,
`sleela-silk-road`, `sleela-east-asian`, `sleela-greek`, `sleela-turkish`.

> Full token highlighting uses a `sleela` Tree-sitter grammar; the theme colours
> map standard Helix scopes (`keyword`, `type`, `string`, `comment`, `function`,
> `keyword.directive`, …) so they apply to any grammar Helix resolves for the
> language, and the palette is identical to the vim/emacs schemes.
