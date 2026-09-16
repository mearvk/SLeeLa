# SleelaTerminal™ Graphical Terminal

SLeeLa provides a dedicated graphical terminal window for the SleelaTerminal™ shell.

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

## Settings menu

The title bar includes a three-horizontal-line settings/menu button. It opens a compact GTK popover so configuration does not require a permanent side panel or toolbar.

The menu currently provides:

- font family and size choices;
- terminal font/foreground color choices;
- input/output language orientation;
- declared national/channel feed categories;
- declared persons-of-interest categories;
- declared parametric document/résumé categories; and
- declared national-identifier categories.

The terminal remains the centered primary workspace. Text selection and copy/paste remain available through the terminal's contextual right-click menu.

## Parametric Ring of Trust / Orientation

The settings model treats these source categories as local, human-readable declarations. They describe an orientation between a terminal, a parameter, and a possible information source. They do **not** independently establish identity, authority, truth, provenance, legal status, or trust.

The current implementation deliberately keeps source switches off by default. Enabling a switch records the user's local configuration choice; it does not silently fetch, transmit, authenticate, identify, or ingest a source.

This gives future feed, document, or identity-adapter work a clear extension point without making external data acquisition part of the terminal's presentation layer.

## Configuration

Settings are loaded and saved in a simple text configuration file:

```text
~/.config/sleela/sleela-terminal.conf
```

An example schema is provided in `sleela-terminal.conf.example`. The file uses straightforward `key=value` entries so it can be inspected or edited without a specialized configuration editor.

## Build

From `sleela-terminal`:

```sh
make gui
```

The executable is:

```text
build/SleelaTerminal
```

The default `make` target builds the GUI in addition to `build/slsh` and the smoke-test executable.

## Run

```sh
./build/SleelaTerminal
```

The GUI automatically looks for `slsh` beside itself, so the development build launches `build/slsh`. An explicit shell path may also be supplied:

```sh
./build/SleelaTerminal ./build/slsh
```

## Branding and visual frame

The application window is branded:

```text
SleelaTerminal™ — MEARVK LLC
```

The title bar and footer retain the dark rich-purple visual language, bright white controls/text, and three-dimensional highlight treatment. The settings popover uses the same purple family rather than introducing a separate theme.

The current GUI displays version `1.0.0`, matching the project's initial SleelaTerminal version baseline.

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
