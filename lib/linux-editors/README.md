# SLeeLa editor integrations — syntax and color schemes

Syntax highlighting and color schemes for `.sleela` across six editors. The
optional user-local installer deploys integrations without elevated
permissions; it does not edit an editor's main configuration file.

| Editor | Folder | Install guide |
|---|---|---|
| Vim | [`vim/`](vim/) | [`vim/README.md`](vim/README.md) |
| Emacs | [`emacs/`](emacs/) | [`emacs/README.md`](emacs/README.md) |
| Neovim | [`neovim/`](neovim/) | [`neovim/README.md`](neovim/README.md) |
| VS Code | [`vscode/`](vscode/) | [`vscode/README.md`](vscode/README.md) |
| Helix | [`helix/`](helix/) | [`helix/README.md`](helix/README.md) |
| nano | [`nano/`](nano/) | [`nano/README.md`](nano/README.md) |

## Install and uninstall

From the SLeeLa repository root:

```sh
make install-editors
```

The installer detects installed editors and copies their integration files to
user-local configuration directories. It never invokes `sudo` and does not
modify editor startup/configuration files. Preview operations first:

```sh
make install-editors EDITOR_ARGS=--dry-run
```

Select editors explicitly with a comma-separated list:

```sh
make install-editors EDITORS=vim,neovim,emacs
```

Remove files managed by the installer with:

```sh
make uninstall-editors
```

The installer backs up files it replaces and restores them on uninstall. If a
managed file has been edited since installation, uninstall preserves that
local edit. State and backups are kept under
`$XDG_DATA_HOME/sleela/editor-installer` (normally
`~/.local/share/sleela/editor-installer`).

**Editor-specific notes:** nano's `.nanorc` files are copied into
`~/.config/nano/sleela`; add
`include "~/.config/nano/sleela/*.nanorc"` to your nano configuration.
Emacs still needs the load-path entries and mode/theme selection shown in its
README. VS Code is placed in the per-user extensions directory; restart VS Code
after installation.

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
shares the same palette, kept in step by hand). Edit the palette in
`vim/colors/`, then regenerate:

```sh
python3 lib/linux-editors/generate-editor-themes.py
```

Reference palette SVGs live under [`palettes/`](palettes/). The main SLeeLa
runtime installer remains separate; editor integrations are an explicit,
optional action.
