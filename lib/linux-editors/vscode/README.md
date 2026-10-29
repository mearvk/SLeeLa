# SLeeLa grammar and color themes for VS Code

A minimal extension: a TextMate grammar for `.sleela` plus one color theme per
SLeeLa palette (mirroring the vim/emacs schemes).

## Install (from source, for local use)

```sh
mkdir -p ~/.vscode/extensions/sleela
cp -r lib/linux-editors/vscode/* ~/.vscode/extensions/sleela/
```

Restart VS Code, then pick a theme via **Preferences: Color Theme** — the brand
themes *Kiro* and the more potent *SLeeLa* (palette: purple, black, dark blue,
yellow, white), or a regional one: *SLeeLa American Professional*, *SLeeLa
Pan-Eurasian*, *SLeeLa Russian and Slavic (Medium)*, *SLeeLa Central Asian /
Silk Road*, *SLeeLa East Asian*, *SLeeLa Greek*, or *SLeeLa Turkish*.

The grammar highlights keywords, types, strings, numbers, `@`-annotations, the
`#sleela`/`#schema` pragmas, the document markers (`author`/`authorial`/
`material`/`memory`/`determine`, matched only when alone on a line), and the
`//`, `/* */`, `<< >>`, and `< >` comment forms.

> For Vim-style modal editing in VS Code, pair this with a Vim-emulation
> extension; the grammar and themes are independent of the keymap.
