# SLeeLa syntax and color schemes for Vim

Classic Vim support for `.sleela` files: a filetype detector, a syntax file,
and nine color schemes.

## Install

From the SLeeLa repository root:

```sh
make install-editors EDITORS=vim
```

Preview with `make install-editors EDITORS=vim EDITOR_ARGS=--dry-run`.
Remove managed files with `make uninstall-editors`; backed-up files are
restored and locally modified files are preserved.

For manual installation instead:

```sh
mkdir -p ~/.vim/{ftdetect,syntax,colors}
cp lib/linux-editors/vim/ftdetect/sleela.vim  ~/.vim/ftdetect/
cp lib/linux-editors/vim/syntax/sleela.vim    ~/.vim/syntax/
cp lib/linux-editors/vim/colors/sleela-*.vim  ~/.vim/colors/
```

Then in `~/.vimrc`:

```vim
syntax enable
colorscheme sleela-kiro      " or sleela-sleela, sleela-american, ...
```

`ftdetect/sleela.vim` sets `filetype=sleela` for `*.sleela`, and
`syntax/sleela.vim` provides the highlighting; a `colorscheme` supplies the
palette. If you use a Vim plugin manager, point it at this directory instead of
copying.

## Themes (9)

Brand themes (palette: purple, black, dark blue, yellow, white) — `sleela-kiro`
and the more potent `sleela-sleela`. Regional themes: `sleela-american`,
`sleela-eurasia`, `sleela-russian-slavic`, `sleela-silk-road`,
`sleela-east-asian`, `sleela-greek`, `sleela-turkish`.

## Files

| File | Role |
|---|---|
| `ftdetect/sleela.vim` | sets `filetype=sleela` for `*.sleela` |
| `syntax/sleela.vim` | the SLeeLa syntax highlighting |
| `syntax/sleela-philosophers.vim` | highlighting for the philosophers Guide-chain surface |
| `ftplugin/sleela-philosophers.vim` | per-buffer settings for that surface |
| `colors/sleela-*.vim` | the nine color schemes |

## Source of truth

These Vim `colors/*.vim` files are the **palette source of truth** for the whole
editor set: `../generate-editor-themes.py` reads their hex values and mirrors
them into Neovim, Helix, nano, and VS Code. Edit the palette here, then run:

```sh
python3 lib/linux-editors/generate-editor-themes.py
```
