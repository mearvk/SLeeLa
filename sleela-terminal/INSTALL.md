# SLeeLaTerminal™ Installation

## Build

From the `sleela-terminal` directory:

```sh
make
```

The production shell is written to `build/slsh`.

## Test

```sh
make test
```

For AddressSanitizer and UndefinedBehaviorSanitizer validation:

```sh
make sanitize
```

For the Clang/libFuzzer target:

```sh
make fuzz
```

## Install

The default installation prefix is `/usr/local`:

```sh
sudo make install
```

This installs the shell as:

```text
/usr/local/bin/slsh
```

The installation prefix and binary directory are configurable:

```sh
make install PREFIX="$HOME/.local"
```

or:

```sh
make install PREFIX=/opt/sleela
```

`DESTDIR` is also supported for staged/package builds:

```sh
make install DESTDIR=/tmp/package-root
```

## Install With Stripping

```sh
sudo make install-strip
```

`install-strip` rebuilds with the configured compiler flags plus `-s` and then installs the resulting binary.

## Uninstall

Use the same `PREFIX`, `BINDIR`, and `DESTDIR` values used for installation:

```sh
sudo make uninstall
```

For a user-local installation:

```sh
make uninstall PREFIX="$HOME/.local"
```

The uninstall target removes only the installed `slsh` binary; it does not remove source files, build files, or unrelated files in the installation directory.

## Clean

```sh
make clean
```

This removes the generated `build/` directory.
