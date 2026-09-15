# SLeeLaTerminal™ Installation

## Build

From the `sleela-terminal` directory:

```sh
make
```

The build produces both the shell and the dedicated graphical terminal:

```text
build/slsh
build/SleelaTerminal
```

The GUI uses GTK 4 and VTE. On Debian/Ubuntu systems, install the corresponding GTK 4 and VTE GTK 4 development packages before building the GUI.

To build only the graphical terminal:

```sh
make gui
```

## Run the GUI

From the `sleela-terminal` directory:

```sh
./build/SleelaTerminal
```

The GUI opens a dedicated terminal window and starts the SLeeLa shell (`slsh`) inside a real pseudo-terminal. The window title is **SleelaTerminal™ — MEARVK LLC** and uses a dark rich-purple title bar.

You may explicitly select a shell executable:

```sh
./build/SleelaTerminal ./build/slsh
```

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

This installs:

```text
/usr/local/bin/slsh
/usr/local/bin/SleelaTerminal
/usr/local/share/applications/SleelaTerminal.desktop
```

The desktop entry launches the GUI as a normal desktop application; it does not open another terminal emulator around SLeeLa.

The installation prefix and binary directory are configurable:

```sh
make install PREFIX="$HOME/.local"
```

or:

```sh
make install PREFIX=/opt/sleela
```

`DESTDIR` is also supported for staged/package builds.

## Install With Stripping

```sh
sudo make install-strip
```

`install-strip` rebuilds the production targets with the configured compiler flags plus `-s` and then installs them.

## Uninstall

Use the same `PREFIX`, `BINDIR`, `DESKTOPDIR`, and `DESTDIR` values used for installation:

```sh
sudo make uninstall
```

For a user-local installation:

```sh
make uninstall PREFIX="$HOME/.local"
```

The uninstall target removes only the SLeeLa shell, GUI executable, and SLeeLa desktop entry installed by this Makefile.

## Clean

```sh
make clean
```

This removes the generated `build/` directory.
