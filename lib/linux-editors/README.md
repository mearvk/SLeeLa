# SLeeLa editor integrations — syntax and color schemes

Syntax highlighting and color schemes for `.sleela` across five editors. Each
subfolder has its own install steps; there is **no installer** — editors load
these from their own config directories, so installing is a copy (the per-editor
README gives the exact `cp` commands).

| Editor | Folder | Install guide |
|---|---|---|
| Vim | [`vim/`](vim/) | [`vim/README.md`](vim/README.md) |
| Emacs | [`emacs/`](emacs/) | [`emacs/README.md`](emacs/README.md) |
| Neovim | [`neovim/`](neovim/) | [`neovim/README.md`](neovim/README.md) |
| VS Code | [`vscode/`](vscode/) | [`vscode/README.md`](vscode/README.md) |
| Helix | [`helix/`](helix/) | [`helix/README.md`](helix/README.md) |
| nano | [`nano/`](nano/) | [`nano/README.md`](nano/README.md) |

## The nine themes

Every editor ships the same nine schemes, so a developer keeps one look across
tools:

- **Brand** (palette: purple, black, dark blue, yellow, white) — `sleela-kiro`
  and the more potent `sleela-sleela`.
- **Regional** — `sleela-american`, `sleela-eurasia`, `sleela-russian-slavic`,
  `sleela-silk-road`, `sleela-east-asian`, `sleela-greek`, `sleela-turkish`.

## Source of truth

The **Vim `colors/*.vim` files are the palette source of truth.**
[`generate-editor-themes.py`](generate-editor-themes.py) reads their hex values
and generates the **Neovim, Helix, nano, and VS Code** schemes from them (Emacs
shares the same palette, kept in step by hand). So edit the palette in
`vim/colors/`, then regenerate to propagate the change:

```sh
python3 lib/linux-editors/generate-editor-themes.py   # mirror vim -> neovim/helix/nano/vscode
```

Reference palette SVGs live under [`palettes/`](palettes/).

> These are source assets the developer copies into their editor config; the
> main SLeeLa installer does not deploy them into `~/.vim`, `~/.emacs.d`, etc.
