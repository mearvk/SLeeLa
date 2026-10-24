# SLeeLa man pages

Manual pages for the SLeeLa toolchain, in standard `man` layout.

| Page | Section | Topic |
|------|--------:|-------|
| [`man1/sleela.1`](man1/sleela.1) | 1 | the `sleela` compiler/runtime front end (Sleelvac(TM)) |

## View without installing

```sh
man ./man/man1/sleela.1
# or
groff -man -Tutf8 man/man1/sleela.1 | less -R
```

## Install (user-local)

```sh
install -Dm644 man/man1/sleela.1 "${XDG_DATA_HOME:-$HOME/.local/share}/man/man1/sleela.1"
mandb "${XDG_DATA_HOME:-$HOME/.local/share}/man" 2>/dev/null || true
man sleela
```

## Install (system-wide)

```sh
sudo install -Dm644 man/man1/sleela.1 /usr/local/share/man/man1/sleela.1
sudo mandb 2>/dev/null || true
```

The page is kept in lockstep with the binary's own usage
(`impl/frontend/driver.cpp`); the binary is the authority. Rebuild it with
`cd impl && make build/sleela`.
