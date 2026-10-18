# `/example` — worked, runnable SLeeLa examples

This folder holds hands-on, self-contained starting points you can read, run,
and adapt. It now covers three things:

1. **One GUI example per `lib/gui` version line** — the full spread of known UI
   lines, from the 1.0 baseline to the 5.0 military build.
2. **A full set of science examples** — runnable SLeeLa programs on the science
   subject libraries (math, physics, chemistry, astrophysics, sociology) **and**
   BODI XML projects for every executable runner discipline, all grounded in the
   real [`churn/`](../churn) datasets. These live in
   [`example/science/`](science/).
3. **A worked XLT CPU-model sheet.**

| File | What it is |
| --- | --- |
| [`gui-mual-text-editor.sleela`](gui-mual-text-editor.sleela) | GUI **MUAL (1.0 baseline)** — toolkit-neutral window, the three integration paths, the 1..14 document listener. |
| [`gui-gual-settings-console.sleela`](gui-gual-settings-console.sleela) | GUI **GUAL (2.0)** — a hardened, settings-first preferences console; every knob clamps to a safe window, backend is fail-closed. |
| [`gui-thual-effects-showcase.sleela`](gui-thual-effects-showcase.sleela) | GUI **THUAL (3.0)** — the visual-effects layer: glow / shine / shadow / metal / rotation / fade + branding + audio cues, with a scheduled rotation that drives an internal memo key epoch. |
| [`gui-dual-media-studio.sleela`](gui-dual-media-studio.sleela) | GUI **DUAL (4.0)** — the rich component set: audio recorder, video canvas, URL bar, layout, graphics, all codec-aware. |
| [`gui-military-command-console.sleela`](gui-military-command-console.sleela) | GUI **MILITARY (5.0)** — the hardened military build: metal/blue surface, strong branding, Component-vs-Container taxonomy, icons, a crypto selector, a certificate store, and window key-listeners. |
| [`run-gui-examples.sh`](run-gui-examples.sh) | Runs the four self-contained GUI-line examples (MUAL→DUAL) in one go. |
| [`run-gui-military.sh`](run-gui-military.sh) | Runs the military-line example (its `import gui.version-military;` pulls in the Master Classes — no dep files needed). |
| [`science/`](science/) | Science examples for **every discipline** — runnable SLeeLa programs (`*.sleela`) on the subject libraries **and** BODI XML projects. See [`science/README.md`](science/README.md). |
| [`threadripper-workstation.xlt`](threadripper-workstation.xlt) | An **XLT (`.xlt`) CPU-model sheet** — declaratively authors a CPU, board, tiered memory, capabilities, and a program to run on the assembled machine. |

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

The four self-contained lines (MUAL → DUAL) run with no dependency classes:

```sh
./example/run-gui-examples.sh
```

or one at a time:

```sh
export SLEELA_SHEET="$PWD/SHEET.sheet"
export SLEELA_SHA256_MANIFEST="$PWD/security/sha256-manifest.json"
./impl/build/sleela run example/gui-thual-effects-showcase.sleela
```

> **The SHA-256 security gate is fail-closed.** `sleela` refuses to run unless
> `SLEELA_SHA256_MANIFEST` points at the trusted manifest, and it also needs
> `SLEELA_SHEET`. Both wrappers set these for you.

The MILITARY (5.0) example uses the military-line Master Classes
(`SLPackage`, `SLGuiSettings`, …) via its `import gui.version-military;` — the
import resolves them through the library index, so you do **not** pass those
files on the command line. Run it with its wrapper, or directly:

```sh
./example/run-gui-military.sh
# or, equivalently:
./impl/build/sleela run example/gui-military-command-console.sleela
```

> Do NOT also pass `lib/gui/version-military/*.sleela` on the command line: the
> import already defines those classes, so listing them again defined every
> class twice and failed with `duplicate static field 'SLPackage::FEATURE_LEVEL'`.
> The wrapper relies on the import alone.

> Toolchain note: the self-contained MUAL→DUAL examples use an inline model +
> `main`, in the style of [`lib/gui/version-mual/gui-demo.sleela`](../lib/gui/version-mual/gui-demo.sleela).
> The MILITARY example instead pulls its classes in through
> `import gui.version-military;`. All run faithfully on the toolchain as it is
> today.

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
./impl/xlt/build/xltc example/threadripper-workstation.xlt
```

`xltc` reports `SUCCESS` with a build list, or a *congruence* explaining why a
part could not be resolved. For the directive reference (capabilities, memory
tiers, `program`/`grant`), see [`lib/xlt/README.md`](../lib/xlt/README.md).

> Note: the exact SKU/memory/bus names must resolve in the model registry. If a
> part name reports a congruence on your tree, swap it for one used in the
> shipped examples under [`lib/xlt/examples`](../lib/xlt/examples) (e.g.
> `"SLAMDEPYC9575F"`, `"ddr5_6400"`, `"pcieGen5x16"`).
