# `/example` — a worked XLT model and a military-line GUI program

This folder holds two hands-on starting points, one for each of the two recent
SLeeLa layers. They are self-contained drafts you can read, run, and adapt.

| File | What it is |
| --- | --- |
| [`threadripper-workstation.xlt`](threadripper-workstation.xlt) | An **XLT (`.xlt`) CPU-model sheet** — declaratively authors a CPU, board, tiered memory, capabilities, and a program to run on the assembled machine. |
| [`gui-military-command-console.sleela`](gui-military-command-console.sleela) | An **original SLeeLa GUI program** in the MILITARY (5.0) line, modeled on [`lib/gui/version-military`](../lib/gui/version-military). |

---

## 1. `threadripper-workstation.xlt` — an XLT CPU model

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

---

## 2. `gui-military-command-console.sleela` — a military-line GUI program

A runnable "command console" scene in the **military (5.0)** GUI line. It
exercises the hardened, strongly-branded military surface end to end:

- the metal / metal-silver / blue-hue aesthetic with a 3D-lit base frame;
- strong, facile branding (wordmark + insignia + tagline);
- the explicit **Component vs Container** taxonomy;
- carefully sized and placed **icons** on a clickable;
- the **cryptography selector**, gated to a strong / post-quantum minimum;
- the **public-key certificate store** (import → trust → select → revoke);
- **window key-listeners** (minimize-right fold + double-click restore, and
  fullscreen).

It uses only the military-line Master Classes shipped under
[`lib/gui/version-military`](../lib/gui/version-military) — `SLPackage`,
`SLGuiSettings`, `SLGuiBranding`, `SLGuiKind`, `SLGuiIcon`,
`SLGuiCryptoSelector`, `SLGuiCertificateStore`, `SLGuiWindowState` — so it is a
sibling of that line's `gui-military-demo.sleela`, telling its own scene.

Because the military line ships one class per file, run the program together
with the Master Classes it references (a built `sleela` binary is required —
`./scripts/build-linux.sh` or `make -C impl` produces one):

```bash
M=lib/gui/version-military
SLEELA_SHEET=./SHEET.sheet ./impl/build/sleela run \
  example/gui-military-command-console.sleela \
  $M/SLPackage.sleela $M/SLGuiSettings.sleela $M/SLGuiBranding.sleela \
  $M/SLGuiKind.sleela $M/SLGuiIcon.sleela $M/SLGuiCryptoSelector.sleela \
  $M/SLGuiCertificateStore.sleela $M/SLGuiWindowState.sleela
```

The program prints a banner-separated walkthrough of each feature and exits 0.

> Compiler note: with the current toolchain, a `String` argument passed to a
> method that null-checks it (several `configure(...)`/setter paths in these
> classes) does not thread through, and a `static final` constant accessed via
> an instance (`obj.CONST`) is rejected. The example therefore uses the
> constants' literal values (with the name in a comment) and sets the
> string-bearing fields directly — so the rendered scene stays faithful on the
> toolchain as it is today. Numeric setters, array/int fields, and the crypto /
> window-state logic all work through their normal methods.
