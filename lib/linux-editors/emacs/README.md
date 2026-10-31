# SLeeLa major mode and color schemes for Emacs

Emacs support for `.sleela` files: a `prog-mode`-derived major mode
(`sleela-mode`), an optional philosophers Guide-chain overlay, and nine Custom
themes.

## Install

The optional installer copies the integration into a user-local directory:

```sh
make install-editors EDITORS=emacs
```

Preview with `make install-editors EDITORS=emacs EDITOR_ARGS=--dry-run`.
Uninstall managed files with `make uninstall-editors`.

Then add the following to your `~/.emacs` (or `~/.emacs.d/init.el`):

```elisp
(add-to-list 'load-path "~/.emacs.d/sleela")
(add-to-list 'custom-theme-load-path "~/.emacs.d/sleela")
(require 'sleela-mode)
(load-theme 'sleela-kiro t)        ; or sleela-sleela, sleela-american, ...
;; Optional Guide-chain highlighting:
(require 'sleela-philosophers)
```

For manual installation, copy `lib/linux-editors/emacs/*.el` into
`~/.emacs.d/sleela/` first.

`sleela-mode.el` adds `("\\.sleela\\'" . sleela-mode)` to
`auto-mode-alist`, so opening a `.sleela` file selects the mode automatically.

## Themes (9)

`load-theme` names match the files (`sleela-<name>-theme.el` →
`'sleela-<name>`):

Brand themes (palette: purple, black, dark blue, yellow, white) — `sleela-kiro`
and the more potent `sleela-sleela`. Regional themes: `sleela-american`,
`sleela-eurasia`, `sleela-russian-slavic`, `sleela-silk-road`,
`sleela-east-asian`, `sleela-greek`, `sleela-turkish`.

## Files

| File | Role |
|---|---|
| `sleela-mode.el` | the `sleela-mode` major mode (`(provide 'sleela-mode)`) |
| `sleela-philosophers.el` | optional Guide-chain highlighting (`(provide 'sleela-philosophers)`) |
| `sleela-*-theme.el` | the nine Custom themes (`deftheme sleela-*`) |

## Source of truth

The Emacs themes share their palette with the Vim `colors/*.vim` files, which
are the palette source of truth for the editor set; the generator mirrors that
palette into Neovim, Helix, nano, and VS Code. Keep the Emacs `deftheme`
colours in step with the Vim palette when it changes.
