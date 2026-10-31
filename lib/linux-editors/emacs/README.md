# SLeeLa major mode and color schemes for Emacs

Emacs support for `.sleela` files: a `prog-mode`-derived major mode
(`sleela-mode`), an optional philosophers Guide-chain overlay, and nine Custom
themes. There is no installer — Emacs loads these from your `load-path` and
`custom-theme-load-path`, so installation is a copy plus two lines of config.

## Install

```sh
mkdir -p ~/.emacs.d/sleela
cp lib/linux-editors/emacs/*.el ~/.emacs.d/sleela/
```

Then in your `~/.emacs` (or `~/.emacs.d/init.el`):

```elisp
;; make the files loadable and the themes trusted
(add-to-list 'load-path "~/.emacs.d/sleela")
(add-to-list 'custom-theme-load-path "~/.emacs.d/sleela")

;; the major mode: binds .sleela -> sleela-mode via auto-mode-alist
(require 'sleela-mode)

;; pick one color scheme (t = don't prompt for confirmation)
(load-theme 'sleela-kiro t)        ; or sleela-sleela, sleela-american, ...

;; optional: the philosophers Guide-chain highlighting overlay
(require 'sleela-philosophers)
```

`sleela-mode.el` already adds `("\\.sleela\\'" . sleela-mode)` to
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
are the palette source of truth for the editor set; `../generate-editor-themes.py`
mirrors that palette into Neovim, Helix, nano, and VS Code. Keep the Emacs
`deftheme` colours in step with the Vim palette when it changes.
