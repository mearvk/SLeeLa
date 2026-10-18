# `/example` — worked, runnable SLeeLa examples

This folder holds hands-on, self-contained starting points you can read, run,
and adapt. It now covers three things:

1. **One GUI example per `lib/gui` version line** — the full spread of known UI
   lines, from the 1.0 baseline to the 5.0 military build.
2. **A full set of science examples** — one per executable BODI discipline
   (math, physics, chemistry, data-analytics) plus a social-science example and
   a grand tour, all grounded in the real [`churn/`](../churn) datasets. These
   live in [`example/science/`](science/).
3. **A worked XLT CPU-model sheet.**

| File | What it is |
| --- | --- |
| [`gui-mual-text-editor.sleela`](gui-mual-text-editor.sleela) | GUI **MUAL (1.0 baseline)** — toolkit-neutral window, the three integration paths, the 1..14 document listener. |
| [`gui-gual-settings-console.sleela`](gui-gual-settings-console.sleela) | GUI **GUAL (2.0)** — a hardened, settings-first preferences console; every knob clamps to a safe window, backend is fail-closed. |
| [`gui-thual-effects-showcase.sleela`](gui-thual-effects-showcase.sleela) | GUI **THUAL (3.0)** — the visual-effects layer: glow / shine / shadow / metal / rotation / fade + branding + audio cues, with a scheduled rotation that drives an internal memo key epoch. |
| [`gui-dual-media-studio.sleela`](gui-dual-media-studio.sleela) | GUI **DUAL (4.0)** — the rich component set: audio recorder, video canvas, URL bar, layout, graphics, all codec-aware. |
| [`gui-military-command-console.sleela`](gui-military-command-console.sleela) | GUI **MILITARY (5.0)** — the hardened military build: metal/blue surface, strong branding, Component-vs-Container taxonomy, icons, a crypto selector, a certificate store, and window key-listeners. |
| [`run-gui-examples.sh`](run-gui-examples.sh) | Runs the four self-contained GUI-line examples (MUAL→DUAL) in one go. |
| [`run-gui-military.sh`](run-gui-military.sh) | Runs the military-line example with its one-class-per-file Master Classes. |
| [`science/`](science/) | Runnable BODI science examples for **every executable discipline** — see [`science/README.md`](science/README.md). |
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

The MILITARY (5.0) example is modeled directly on the one-class-per-file
military Master Classes, so it has its own wrapper that lists them:

```sh
./example/run-gui-military.sh
```

> Toolchain note: the self-contained MUAL→DUAL examples use an inline model +
> `main`, in the style of [`lib/gui/version-mual/gui-demo.sleela`](../lib/gui/version-mual/gui-demo.sleela),
> so they run faithfully on the toolchain as it is today. The MILITARY example
> follows the same compiler caveats documented for the military line (a
> `String` argument passed to a null-checking method does not thread through,
> and a `static final` constant accessed via an instance is rejected), so it
> uses the constants' literal values and sets string fields directly.

---

## 2. Science examples — every executable BODI discipline

[`example/science/`](science/) contains one deterministic BODI XML project per
executable science discipline, grounded in the real, sourced
[`churn/`](../churn) datasets, each with a recorded `*.expected.txt` witness:

- [`math.xml`](science/math.xml) — every math op (`abs`, `sqrt`, `exp`, `log`,
  `log10`, `sin`, `cos`, `tan`, `pow`, `hypot`, `min`, `max`, `clamp`).
- [`physics.xml`](science/physics.xml) — kinematics, dynamics, gravitation,
  special relativity, electricity, waves, neutrino oscillation; inputs from the
  CODATA physics constants and the Solar System datasets.
- [`chemistry.xml`](science/chemistry.xml) — `ratio` and `similarity` over
  IUPAC standard atomic weights.
- [`data-analytics.xml`](science/data-analytics.xml) — `sum`/`mean`/`min`/`max`/
  `variance`/`stddev`/`correlation` of planetary distance vs. orbital period.
- [`social.xml`](science/social.xml) — descriptive statistics over real US
  labor figures plus population density; the social face of the runner.
- [`all-disciplines.xml`](science/all-disciplines.xml) — a grand tour across all
  four executable disciplines.

```sh
cd api/bodi && make                # builds bodi-xml-runner
cd ../../example/science
make                               # run every example, print witnesses
make check                         # run every example and diff vs *.expected.txt
```

See [`science/README.md`](science/README.md) for the full table and data
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
