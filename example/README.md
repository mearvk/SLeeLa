# `/example` — worked, runnable SLeeLa examples

This folder holds hands-on, self-contained starting points you can read, run,
and adapt. It now covers four things:

1. **One GUI example per `lib/gui` version line** — the full spread of known UI
   lines, from the 1.0 baseline to the 5.0 military build. Each one renders a
   **real, on-screen window** through SleelaUI (SLeeLa's own native C/C++
   toolkit) via the `ui*` SLVM bridge, and each is now **interactive**: the
   buttons actually function.
2. **A full set of science examples** — runnable SLeeLa programs on the science
   subject libraries (math, physics, chemistry, astrophysics, sociology) **and**
   BODI XML projects for every executable runner discipline, all grounded in the
   real [`churn/`](../churn) datasets.
3. **A dedicated menubar showcase** with a horizontal File/Edit/View/Help bar,
   working dropdowns, and nested Recent Files / Panels groups that expand and
   collapse. See [`menus/`](./menus/README.md).
4. **A worked XLT CPU-model sheet.**

## Unified layout standard

All GUI examples in this directory follow the same bounded-frame policy:

- The frame/card is a hard container boundary.
- Internal content keeps at least 10px inset padding on all sides.
- Child widgets keep at least 10px spacing between them.
- If content grows beyond the frame, it scrolls or clips instead of overflowing.
- The status/footer area remains visually grounded within the same 10px rhythm.

This keeps the examples consistent, readable, and professional for engineers,
developers, and system designers.

---

## Shared GUI Color Theme

The four `lib/gui` version-line demos each use one of the toolkit's four
**Standard Pattern** theme presets. **MILITARY keeps its own metal/blue branded
palette** and is left unchanged.

| Demo | Standard Pattern | Preset id | Character |
|---|---|---|---|
| **MUAL** | WHITE | 4 | Light surfaces, dark text, blue accent |
| **GUAL** | BLACK | 5 | Deep black surfaces, light text |
| **THUAL** | BLUE | 6 | Blue-tinted surfaces, light text |
| **DUAL** | GRAY | 7 | Neutral mid-gray surfaces, light text |
| MILITARY | (own) | — | Metal/blue branded palette, unchanged |

---

## Running the GUI examples

A built `sleela` binary is required — `./scripts/build-linux.sh` (or
`make -C impl`) produces one at `impl/build/sleela`.

```sh
./example/run-gui-examples.sh
```

This runs the five GUI-line demos plus the expandable-menubar showcase with a
consistent shell and bounded layout behaviour.
