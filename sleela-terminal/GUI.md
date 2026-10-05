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

The right of the title bar carries a small, quiet gear button for settings. It is flat (no loud outline — only a soft hover tint) and opens the Settings as a dedicated modal subframe window, so configuration does not require a permanent side panel or toolbar and stays on screen while the user adjusts it.

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

The upper-left of the title bar carries the SleelaTerminal brand logo (the
Debian/Windows-terminal mark). The asset is pre-trimmed to the logo's minimum
2D content bounding box with a transparent background, so it sits flush at the
left edge with no surrounding whitespace. It is a strict image — no button
outline — and is produced by `tools/logo/Trim.java` from the source artwork in
`images/`.

The title bar and footer retain the dark rich-purple visual language, bright white controls/text, and a clean flat treatment. The settings subframe uses the same purple family rather than introducing a separate theme.

The bottom edge of the title bar is a **living throbber** rather than a static border: a 2px seam that is not one uniform brightness but a **flowing field** of many small excitations — "children of wattage" — each adding a *relative* darker/lighter shade near its own position, as if voltage were being fed through a living receptor. **At rest the strip is the title-bar colour itself** (the same chrome purple), so the seam reads as the title bar coming alive rather than a separate band. It is repainted ~20 times a second while the terminal is open.

Excitations are born **anywhere across the full 0–100% width** — most enter from the left, but a share appear mid-strip and **fade in** at that spot. The flow then moves **mainly left → right**, and each pulse **travels the whole way through to 100%** even if its brightness has faded after its amplification series (the fade controls brightness, not lifetime — a pulse only dies when it exits the right edge). Roughly **1 in 30** excitations may pause or glide **right → left** for a short run (about 20–50 px) before rejoining.

Each excitation's motion is integrated to **third order** — jerk → acceleration → velocity → position — and its speed is set by a slow, reflexive "amplification" field, so the field *speeds up and slows down* smoothly (the rate of change of acceleration stays bounded) instead of scrolling at a constant mechanical rate. Normally the shades stay in the title-bar/airy-light-blue family; under **strong amplification** (around 18/20 of the field's range) latent per-cell **hues — orange, green, blue, red — surface** on the brightest pulses, then recede as the field relaxes. The animation timer is created with the window and removed when the terminal exits or the window closes.

The current GUI displays version `1.0.0`, matching the project's initial SleelaTerminal version baseline. The footer also contains the clickable **CMD** image, representing CMD as the Java native launcher associated with SecureJDK 28. It is rendered as a strict image with no additional footer-button outline.

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
