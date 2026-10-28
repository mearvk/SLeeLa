# SLeeLa color schemes for Neovim

Native Lua colorschemes mirroring the vim/emacs palettes. Neovim reads the
shared Vim syntax file (`../vim/syntax/sleela.vim`); these supply the colours.

## Install

```sh
mkdir -p ~/.config/nvim/{lua/sleela,colors,ftdetect,syntax}
cp -r lib/linux-editors/neovim/lua/sleela/* ~/.config/nvim/lua/sleela/
cp lib/linux-editors/neovim/colors/*.lua     ~/.config/nvim/colors/
cp lib/linux-editors/neovim/ftdetect/*.lua   ~/.config/nvim/ftdetect/
cp lib/linux-editors/vim/syntax/sleela.vim   ~/.config/nvim/syntax/
```

Then in `init.lua`:

```lua
vim.cmd.colorscheme("sleela-american")
```

Themes: `sleela-american`, `sleela-eurasia`, `sleela-russian-slavic`,
`sleela-silk-road`, `sleela-east-asian`, `sleela-greek`, `sleela-turkish`.

Each theme also links the common Tree-sitter capture groups (`@keyword`,
`@type`, `@string`, …) so it works with `nvim-treesitter` highlighting as well
as the classic regex syntax.
