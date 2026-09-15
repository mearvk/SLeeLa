# SleelaTerminal™ Graphical Terminal

SLeeLa now provides a dedicated graphical terminal window for the SleelaTerminal™ shell.

## Architecture

```text
SleelaTerminal GUI
        │
        ├── GTK 4 window
        ├── VTE terminal widget
        └── PTY
             │
             ▼
           slsh
             │
             ▼
        SLeeLa shell
```

The GUI is a terminal emulator front end, not a second shell implementation. It starts the existing `slsh` executable inside a pseudo-terminal, preserving the existing command parser, executor, builtins, pipelines, jobs, M5 features, and shell behavior.

VTE supplies the virtual terminal widget and PTY integration used by GTK terminal applications.

## Build

From `sleela-terminal`:

```sh
make gui
```

The executable is:

```text
build/SleelaTerminal
```

The default `make` target now builds the GUI in addition to `build/slsh` and the smoke-test executable.

## Run

```sh
./build/SleelaTerminal
```

The GUI automatically looks for `slsh` beside itself, so the development build launches `build/slsh`. An explicit shell path may also be supplied:

```sh
./build/SleelaTerminal ./build/slsh
```

## Branding

The application window is branded:

```text
SleelaTerminal™ — MEARVK LLC
```

The title bar uses a dark rich-purple presentation with white product text.

A version number is deliberately not hard-coded until the project establishes a canonical SLeeLa version source. This prevents the GUI from presenting an invented release number.

## Desktop installation

`make install` installs both executables and the desktop launcher:

```text
$(PREFIX)/bin/slsh
$(PREFIX)/bin/SleelaTerminal
$(PREFIX)/share/applications/SleelaTerminal.desktop
```

The desktop entry declares `Terminal=false`, so launching SleelaTerminal does not wrap the shell inside another terminal emulator.

## Dependencies

The graphical target requires GTK 4 and the GTK 4 VTE development package. On Debian/Ubuntu systems the corresponding development packages are typically named:

```sh
sudo apt install libgtk-4-dev libvte-2.91-gtk4-dev
```

The existing CLI shell remains a C++17 program and does not depend on GTK or VTE.
