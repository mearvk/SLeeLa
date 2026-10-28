# SLeeLa Linux Editor Themes

A coordinated collection of professional syntax palettes for Vim and Emacs on Linux. The original **American Professional** theme remains the default; four companion schemes are available as independent selections.

## Themes

- **American Professional** — deep navy, American blue, teal and warm amber.
- **Pan-Eurasian** — sapphire, jade, silk gold and burgundy, a broad cross-regional interpretation rather than a claim to represent one country.
- **Russian and Slavic (medium)** — balanced charcoal-slate, muted imperial blue, restrained ruby and antique gold. Contrast and saturation are intentionally moderate for a calm professional workspace.
- **Central Asian / Silk Road** — turquoise, lapis lazuli, desert gold and warm textile ivory.
- **East Asian** — ink black, lacquer red, jade and ceremonial gold.

These are design interpretations inspired by broad cultural color references, not official national or cultural palettes.

## Vim install

From the repository root:

```sh
mkdir -p ~/.vim/{syntax,ftdetect,colors}
cp lib/linux-editors/vim/syntax/sleela.vim ~/.vim/syntax/
cp lib/linux-editors/vim/ftdetect/sleela.vim ~/.vim/ftdetect/
cp lib/linux-editors/vim/colors/sleela-*.vim ~/.vim/colors/
```

Add one scheme to `~/.vimrc`:

```vim
syntax enable
set background=dark
colorscheme sleela-american
```

Choose one of `sleela-eurasia`, `sleela-russian-slavic`, `sleela-silk-road`, or `sleela-east-asian` instead.

## Emacs install

Copy `lib/linux-editors/emacs/sleela-mode.el` and the desired `sleela-*-theme.el` file into a directory on your Emacs `load-path`. Then load the mode and one theme:

```elisp
(require 'sleela-mode)
(load-theme 'sleela-russian-slavic t)
```

Theme IDs: `sleela-american`, `sleela-eurasia`, `sleela-russian-slavic`, `sleela-silk-road`, `sleela-east-asian`. Theme files are independent; load only the theme you want.

## More editors (Neovim, Helix, nano, VS Code)

The same palettes are available for four more editors, generated from one
palette table so the colours stay identical across all of them. Each has its
own install guide:

- **Neovim** — native Lua colorschemes + Tree-sitter capture links.
  See [`lib/linux-editors/neovim/README.md`](../lib/linux-editors/neovim/README.md).
- **Helix** — `languages.toml` language definition + per-theme TOML.
  See [`lib/linux-editors/helix/README.md`](../lib/linux-editors/helix/README.md).
- **nano** — `nanorc` syntax files (24-bit colour, nano >= 5.0).
  See [`lib/linux-editors/nano/README.md`](../lib/linux-editors/nano/README.md).
- **VS Code** — a TextMate grammar + one color theme per palette.
  See [`lib/linux-editors/vscode/README.md`](../lib/linux-editors/vscode/README.md).

All seven palettes (`american`, `eurasia`, `russian-slavic`, `silk-road`,
`east-asian`, `greek`, `turkish`) are provided for each editor. Regenerate every
editor's files after a palette change with
`python3 lib/linux-editors/generate-editor-themes.py`.

> **Ed / Sed** are non-interactive line editors with no colour-scheme or
> syntax-highlighting concept, so there is nothing to theme for them; use them
> for scripted `.sleela` edits.

## What is highlighted

Both the Vim syntax (`vim/syntax/sleela.vim`) and the Emacs mode
(`emacs/sleela-mode.el`) recognise:

- **Keywords** — control flow, declarations, access modifiers, literals.
- **Types** — `String`, `Int`, `Double`, `Boolean`, `List`, `Map`, … .
- **Document pragmas** — `#sleela X.Y` and `#schema …`.
- **Document markers** — the contextual, modifier-less words `author` /
  `authorial` (syntax 2.4) and `material` (syntax 2.5). They are highlighted
  **only** when they stand alone at the start of a line (with an optional `;`),
  exactly as the compiler treats them — so an ordinary identifier named, say,
  `material` elsewhere in the code is left unstyled. Markers and pragmas use a
  distinct face/group (Vim `PreProc`; Emacs `font-lock-builtin-face`) so they
  read as document markers rather than ordinary keywords.
- **Comments** — all four SLeeLa forms: `//` line, `/* */` block, and the
  go-forward house style `<< >>` block and `< … >` single-line angle comments
  (angle openers must begin a line).
- **The `∴` brander** (U+2234), the originated-source analogue of `@`.
- Strings, numbers, `@`-annotations, function-call heads, and
  `TODO`/`FIXME`/`NOTE`/`XXX` tags.

Markers, pragmas, and the brander inherit the active palette automatically
(they map to groups/faces each bundled theme already defines), so no per-theme
change is needed to colour them.

## Native C/C++ palette utility

```sh
make -C linux-editors
./linux-editors/build/sleela-editor-theme --themes
./linux-editors/build/sleela-editor-theme --theme eurasia --palette
./linux-editors/build/sleela-editor-theme --theme russian-slavic --hex keyword
make -C linux-editors test
```

For backward compatibility, `--palette` and `--hex COLOR` without `--theme` continue to use American Professional.

## Palette reference

| Scheme | Background | Keyword | Type | String | Comment |
|---|---|---|---|---|---|
| American Professional | `#101820` | `#7FB3FF` | `#73DACA` | `#F2C078` | `#8A96A3` |
| Pan-Eurasian | `#111827` | `#7AA7E8` | `#69C9B3` | `#D7B56D` | `#8995A5` |
| Russian and Slavic (medium) | `#202733` | `#A9C0DD` | `#8FC4B2` | `#D6BD8B` | `#9AA3AF` |
| Central Asian / Silk Road | `#1C2529` | `#7FADE0` | `#45C6C2` | `#D9B56D` | `#92A3A0` |
| East Asian | `#171A1C` | `#D97878` | `#72C7A5` | `#D8B76A` | `#969F9B` |

---

# SLeeLa
<p align="center"><img src="images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>
