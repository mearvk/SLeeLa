# SLeeLa Linux Editor Theme

**SLeeLa American Professional** is a coordinated, low-glare syntax palette for Vim and Emacs on Linux. Deep navy, clean American blue, cool teal, warm amber, slate comments, and high-contrast diagnostics make token classes distinct for long editing sessions.

## Vim install

From the repository root:

```sh
mkdir -p ~/.vim/{syntax,ftdetect,colors}
cp lib/linux-editors/vim/syntax/sleela.vim ~/.vim/syntax/
cp lib/linux-editors/vim/ftdetect/sleela.vim ~/.vim/ftdetect/
cp lib/linux-editors/vim/colors/sleela-american.vim ~/.vim/colors/
```

Add to `~/.vimrc`:

```vim
syntax enable
set background=dark
colorscheme sleela-american
```

## Emacs install

Copy the two files from `lib/linux-editors/emacs/` into a directory on your Emacs `load-path`, then add:

```elisp
(require 'sleela-mode)
(load-theme 'sleela-american t)
```

Both adapters detect `.sleela` files. Editor configuration is per-user; no sudo is required.

## Native palette utility

```sh
make -C linux-editors
./linux-editors/build/sleela-editor-theme --palette
./linux-editors/build/sleela-editor-theme --hex keyword
make -C linux-editors test
```

## Palette

| Token | Color | Hex |
|---|---|---|
| Background | Deep navy | `#101820` |
| Foreground | Soft white | `#F4F7FA` |
| Keywords | American blue | `#7FB3FF` |
| Types | Cool teal | `#73DACA` |
| Strings | Warm amber | `#F2C078` |
| Numbers | Violet | `#C4A7E7` |
| Comments | Slate | `#8A96A3` |
| Functions | Green | `#82C91E` |
| Errors | Coral | `#FF6B6B` |
