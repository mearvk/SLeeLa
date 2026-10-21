# `/example` — worked, runnable SLeeLa examples

This folder holds hands-on, self-contained starting points you can read, run,
and adapt. It now covers three things:

1. **One GUI example per `lib/gui` version line** — the full spread of known UI
   lines, from the 1.0 baseline to the 5.0 military build. Each one renders a
   **real, on-screen window** through SleelaUI (SLeeLa's own native C/C++
   toolkit) via the `ui*` SLVM bridge, and each is now **interactive**: the
   buttons actually function. Instead of calling the toolkit's own
   `uiAppRun()` (which just waits for the window to close), each example runs
   its **own event loop** — every turn it pumps the toolkit and then polls each
   control's one-shot activation latch (`uiWidgetTakeActivated`) and reacts, so
   clicking a button **switches the visible panel** (`uiWidgetVisible`) and
   **updates the status line** (`uiWidgetSetText`) live. The MUAL editor's
   document panel is a `uiScrolledView`, so its long watch-list scrolls and a
   scrollbar appears **automatically** on overflow — the newest GUI feature.
2. **A full set of science examples** — runnable SLeeLa programs on the science
   subject libraries (math, physics, chemistry, astrophysics, sociology) **and**
   BODI XML projects for every executable runner discipline, all grounded in the
   real [`churn/`](../churn) datasets. These live in
   [`example/science/`](science/).
3. **A worked XLT CPU-model sheet.**

| File | What it is |
| --- | --- |
| [`gui-mual-text-editor.sleela`](mual/gui-mual-text-editor.sleela) | GUI **MUAL (1.0 baseline)** — interactive: the **Editor / Documents / About** buttons switch the visible panel, **Save / New** update the status line, and the Documents panel is a `uiScrolledView` whose 1..14 watch-list **scrolls with an automatic scrollbar** on overflow. |
| [`gui-gual-settings-console.sleela`](gual/gui-gual-settings-console.sleela) | GUI **GUAL (2.0)** — interactive: clicking an accepted backend (**javafx / swing / native**) selects it live (suggested highlight + status), while **qt** is refused on click (fail-closed `configureStrict`). Clamped knobs ride live sliders/toggles/combo. |
| [`gui-thual-effects-showcase.sleela`](thual/gui-thual-effects-showcase.sleela) | GUI **THUAL (3.0)** — interactive: **Confirm** fires the audio cue and advances the schene (rotating the live throbber hue a quarter-turn per the memo key epoch); **Toggle fade** fades the primary action in/out. |
| [`gui-dual-media-studio.sleela`](dual/gui-dual-media-studio.sleela) | GUI **DUAL (4.0)** — interactive: the **Play / Pause / Stop** transport and **Record / Stop** controls change the player + recorder state live, all codec-gated (the UI only offers media it can play). |
| [`gui-military-command-console.sleela`](military/gui-military-command-console.sleela) | GUI **MILITARY (5.0)** — interactive and **fail-closed**: **Run** is locked until **Authorize** is clicked (then it executes under authority), and **Reset** clears authority and re-locks Run — `SLGuiAdminAuthority` made tangible. Metal/blue surface, strong branding, taxonomy, crypto selector, certificate store. |
| [`gui-military-window.sleela`](../military/gui-military-window.sleela) | GUI **MILITARY (5.0)** — the designed command console as a real, on-screen window (the original native `ui*` showcase these examples now all follow). |
| [`gui-buffering-resize.sleela`](../demo/gui-buffering-resize.sleela) | **Double / triple buffering** — a resizable window that selects a presentation buffering mode (`uiWindowSetBuffering`) so a resize lands atomically with no tearing; buttons switch single/double/triple live. Also a validated `SLGuiSettings.bufferingMode` knob (2.0+). |
| [`run-gui-examples.sh`](run-gui-examples.sh) | Runs all five version-line examples plus the military-window and buffering-resize demos. |
| [`run-gui-military.sh`](run-gui-military.sh) | Runs just the military-line command console (self-contained `ui*` program — no dep files needed). |
| [`science/`](science/) | Science examples for **every discipline** — runnable SLeeLa programs (`*.sleela`) on the subject libraries **and** BODI XML projects. See [`science/README.md`](science/README.md). |
| [`threadripper-workstation.xlt`](xlt/threadripper-workstation.xlt) | An **XLT (`.xlt`) CPU-model sheet** — declaratively authors a CPU, board, tiered memory, capabilities, and a program to run on the assembled machine. |

---

## Shared GUI Color Theme

All five runnable GUI examples now use the same SLeeLa family palette, matching
`lib/gui/GUI-THEME.md` and the `SLGuiTheme` classes:

| Role | Color |
|---|---|
| Title/menu/header chrome | `#1D2025` |
| URL-entry / raised surface | `#383C47` |
| Application background | `#22262D` |
| Panels and cards | `#292D35` |
| Primary text | `#E8EAED` |
| Muted text | `#B0B7C3` |
| Borders | `#444A56` |
| Accent | `#4F8CFF` |
| Focus | `#8AB4F8` |
| Danger | `#D9534F` |
| Success | `#3FA776` |

The example theme builders configure the SleelaUI theme tokens consistently.
Native operating-system title-bar tinting is platform/window-manager dependent;
the `#1D2025` token is applied to the toolkit's chrome/header surface.

---

## 1. GUI examples — one per `lib/gui` version line

SLeeLa's GUI vocabulary ships as five source-compatible version lines (see
[`lib/gui/ARCHITECTURE.md`](../lib/gui/ARCHITECTURE.md)); each higher line keeps
every entry point of the one below and adds its surface on top:

| Line | Dir | `SLPackage.version()` | Surface it adds |
|---|---|---|---|
| **MUAL** | [`version-mual`](../lib/gui/version-mual) | `1.0` | The original 25-class baseline: backend selector, window intent, actions, the 1..14 document listener, and the three integration paths. |
| **GUAL** | [`version-gual`](../lib/gui/version-gual) | `2.0` | A hardened, settings-first posture: one validated `SLGuiSettings`, a fail-closed `configureStrict()` backend, a clamped-resize window, and a backpressure-aware listener. |
| **THUAL** | [`version-thual`](../lib/gui/version-thual) | `3.0` | A clean visual-effects layer: `SLGuiEffects` (glow/shine/shadow/metal/rotation/fade/audio), a scheduled-rotation "schene" tied to a memo key epoch, `SLGuiBranding`, and `SLGuiAudioCues`. |
| **DUAL** | [`version-dual`](../lib/gui/version-dual) | `4.0` | A rich professional component set: `SLGuiAudioRecorder`, `SLGuiVideoCanvas`, `SLGuiUrlBar`, `SLGuiLayout`, `SLGuiGraphics`, and a codec-aware media gate (`SLGuiCodecs`). |
| **MILITARY** | [`version-military`](../lib/gui/version-military) | `5.0-military` | The 4.0 surface hardened: metal/blue aesthetic + 3D-lit frame, Component-vs-Container taxonomy, icons, a crypto selector, an X.509-style certificate store, and window key-listeners. |

### Running the GUI examples

A built `sleela` binary is required — `./scripts/build-linux.sh` (or
`make -C impl`) produces one at `impl/build/sleela`.

**These examples now draw real windows.** Each one is self-contained (an inline
model + `main` using only `ui*` built-ins) and paints its scene through
SleelaUI — the project's own native C/C++ toolkit — via the `ui*` SLVM bridge
(`OP_UI_CALL` → [`user-interface/`](../user-interface/) C ABI → X11 / Cocoa /
Win32), the same path as [`gui-military-window.sleela`](gui-military-window.sleela).
On a host with a display they open a window and render the widgets; with no
display (headless/CI) each prints a one-line "no display" notice and exits 0.

Run the five version-line examples plus the two additional native-window demos at once:

```sh
./example/run-gui-examples.sh
```

or one at a time:

```sh
export SLEELA_SHEET="$PWD/SHEET.sheet"
export SLEELA_SHA256_MANIFEST="$PWD/security/sha256-manifest.json"
./impl/build/sleela run example/thual/gui-thual-effects-showcase.sleela
```

> **A display is needed to see the window.** On Linux that means an X server
> (`DISPLAY` set) plus the runtime libs the toolkit links (`libX11`,
> `libfreetype`, `libfontconfig`); macOS/Windows use the native backend. Without
> one, the programs still run and exit cleanly — they just report "no display".

> **The SHA-256 security gate is fail-closed.** `sleela` refuses to run unless
> `SLEELA_SHA256_MANIFEST` points at the trusted manifest, and it also needs
> `SLEELA_SHEET`. Both wrappers set these for you.

The MILITARY (5.0) command console runs the same way; its wrapper is just a
convenience:

```sh
./example/run-gui-military.sh
# or, equivalently:
./impl/build/sleela run example/military/gui-military-command-console.sleela
```

---

## 2. Science examples — SLeeLa programs **and** BODI XML, for every discipline

[`example/science/`](science/) holds two kinds of science example, both grounded
in the real, sourced [`churn/`](../churn) datasets, each with a recorded
expected-output evidence file:

### SLeeLa programs (`*.sleela`) — on the science subject libraries

Runnable `.sleela` programs on the science subject libraries
(`import math;`, `import chemistry;`, …): **Mathematics, Physics, Chemistry,
Astrophysics, Sociology**.

- [`math.sleela`](science/math.sleela) — the full `math` facade.
- [`physics.sleela`](science/physics.sleela) — mechanics, gravitation, special
  relativity, electricity, waves (CODATA constants + Earth data).
- [`chemistry.sleela`](science/chemistry.sleela) — the native `chemistry` facade
  (`ratio`, `similarity`) over IUPAC atomic weights.
- [`astrophysics.sleela`](science/astrophysics.sleela) — flux, redshift, Wien
  peak, Kepler period, escape velocity (solar constant ≈ 1361 W/m²).
- [`sociology.sleela`](science/sociology.sleela) — proportion, rate, growth
  rate, transition probability over real US labor/population figures.

```sh
./example/science/run-science-sleela.sh    # run all five
# or: cd example/science && make sleela-check
```

### BODI XML projects (`*.xml`) — on the BODI XML runner

One deterministic BODI XML project per executable runner discipline:

- [`math.xml`](science/math.xml) — every math op (`abs`, `sqrt`, `exp`, `log`,
  `log10`, `sin`, `cos`, `tan`, `pow`, `hypot`, `min`, `max`, `clamp`).
- [`physics.xml`](science/physics.xml) — kinematics, dynamics, gravitation,
  special relativity, electricity, waves, neutrino oscillation.
- [`chemistry.xml`](science/chemistry.xml) — `ratio` and `similarity`.
- [`data-analytics.xml`](science/data-analytics.xml) — `sum`/`mean`/`min`/`max`/
  `variance`/`stddev`/`correlation` of planetary distance vs. orbital period.
- [`social.xml`](science/social.xml) — descriptive statistics over US labor
  figures plus population density; the social face of the runner.
- [`all-disciplines.xml`](science/all-disciplines.xml) — a grand tour.

```sh
cd api/bodi && make                # builds bodi-xml-runner
cd ../../example/science
make                               # run every XML example, print witnesses
make check-all                     # check BOTH the XML and .sleela examples
```

See [`science/README.md`](science/README.md) for the full tables and data
provenance.

---

## 3. `threadripper-workstation.xlt` — an XLT CPU model

An `.xlt` sheet declaratively authors a **CPU model**: a chosen SKU plus the
board, memory, refresh cadence, storage, and bus parts that assemble it. No
setters and no new keyword — each part is declared once, in order, and the
document's orthogonality is what guarantees the model's integrity.

This example goes end to end: it picks a Threadripper SKU on a DDR5 board,
splits memory into two tiers (DDR4 for *usual* activities, DDR5 for
*accuracy-critical* work), requests the native-compute capabilities (`gpu`,
`dma`, `nativeram`), and then **politely runs a program** on the machine it
assembled — all from one document.

Build the compiler and compile the model:

```bash
make -C impl/xlt                                   # produces impl/xlt/build/xltc
./impl/xlt/build/xltc example/xlt/threadripper-workstation.xlt
```

`xltc` reports `SUCCESS` with a build list, or a *congruence* explaining why a
part could not be resolved. For the directive reference (capabilities, memory
tiers, `program`/`grant`), see [`lib/xlt/README.md`](../lib/xlt/README.md).

> Note: the exact SKU/memory/bus names must resolve in the model registry. If a
> part name reports a congruence on your tree, swap it for one used in the
> shipped examples under [`lib/xlt/examples`](../lib/xlt/examples) (e.g.
> `"SLAMDEPYC9575F"`, `"ddr5_6400"`, `"pcieGen5x16"`).


## Cross-Platform Launchers

Each GUI folder includes a dedicated launcher for **Linux and macOS** (`run-*-gui.sh`), **Windows PowerShell** (`run-*-gui.ps1`), and **Windows Command Prompt** (`run-*-gui.cmd`). From the repository root, the launchers locate the built SLeeLa executable and set the default sheet and SHA-256 manifest environment variables.

Run all five GUI examples:

- Linux/macOS: `./example/run-gui-examples.sh`
- Windows PowerShell: `./example/run-gui-examples.ps1`
- Windows Command Prompt: `example\run-gui-examples.cmd`

Set `SLEELA_BIN` to override executable discovery. The GUI examples require a desktop session for their native windows; the scripts do not install the compiler or GUI dependencies.

**Source-of-Truth Rule:** The five GUI examples and `SLGuiTheme.sleela` define the palette directly in SLeeLa source code. The `SLGuiApp` implementations apply those tokens to the live window theme. Do not depend on external theme files, environment variables, or generated settings to select these colors; editing the source palette is the authoritative change.


## Connectivity status control

The five interactive GUI examples (`dual`, `gual`, `mual`, `thual`, and `military`) include a clickable **📶 Wi-Fi** control at the lower-right of the status bar. It opens a connectivity panel with scan, connect, disconnect, settings, connection-details, and close actions. The panel closes when the icon is toggled, an action is selected, or another example control is activated. Until an SLVM-to-native Wi-Fi host bridge is bound, network-changing actions report that limitation rather than claiming to change the real radio.
