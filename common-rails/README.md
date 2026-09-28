# CommonRails — SST + Sleela edition

Professional printing component for the SLeeLa toolchain, ported from the Java
`CommonRails` in the Tiamat project. It provides:

1. **Professional component lines** — an aligned, column-formatted status line:

   ```
   -- : [Object ID: 0000001234] [Date: 1] [Current: @SearchEngineClient           ] . category images now crawling .
   ```

   Object ID is zero-padded to 10 digits, `Current` is padded to a fixed inner
   width for column alignment, and the message is bracketed with `.` decorators
   — the same layout as the Java edition's `print-method.xml`.

2. **A 21×21 progress square (441 cells)** that fills **bottom-right → left**
   across a row, then **up** one row at a time. One filled cell lights the
   bottom-right corner; 100% fills all 441 cells.

   ```
   ░░░░░░░░░░░░░░░░░░░░░           █████████████████████
   ░ ... (21 rows) ... ░    ...    █ ... (21 rows) ... █
   ░░░░░░░░░░░░░░░░░░░░█           █████████████████████
     1 cell (corner)                 100% (441 cells)
   ```

3. **A single-pixel progress indicator** — the most compact rendering: the
   square collapsed to one cell (one glyph). It reads `░` while at 0% and `█`
   as soon as there is any progress. Call `printPixel(filledCells)` or
   `printPixelPercent(percent)`.

   ```
   ░   (0%)      █   (started)
   ```

This directory ships **two editions** of that component, matching the two
first-class file types of the toolchain.

## Files

| File | Edition | What it is |
|------|---------|------------|
| [`CommonRails.sleela`](CommonRails.sleela) | **Sleela** (Wrapper™) | The program — the printer written in the Sleela source language. |
| [`common-rails.sst`](common-rails.sst) | **SST** (Nordshrift) | The control sheet — declares the square geometry as a semantic subject and drives the Wrapper™ through the triplet. |
| `.gitignore` | — | Excludes generated `build/` / `out/` artifacts. |

### The Sleela edition — `CommonRails.sleela`

A `#sleela 1.1` Wrapper™ written against the accepted Sleela surface
(`impl/README.md`): classes, `int`/`String`/`boolean`, `if`/`else`,
`while`/`for`, arithmetic and comparisons, `+` string concatenation, `print`,
recursion, and class fields. Because the core value model has no arrays or
character indexing, the square is produced entirely with nested integer loops,
and field widths are measured arithmetically. The lexer decodes only
`\n \t \r \\ \"`, so the block glyphs `█` / `░` are embedded as raw UTF-8.

```sh
# from the repository root
./impl/build/sleela check common-rails/CommonRails.sleela   # validate (incl. #sleela version)
./impl/build/sleela run   common-rails/CommonRails.sleela   # run on the C/C++ core
```

### The SST edition — `common-rails.sst`

A Nordshrift control sheet (`#nordshrift 1.0` pragma, using the **2.0 semantic
layer** from NS-SST-0001 Part XVII — the same combination the shipped
`kinematics-subject.sst` uses). It names the `.sleela` source, selects the
`sleela` target, and declares the square as an explicit **math subject**
`CommonRailsSquare`: the side (21 cells), the derived cell count (441), the
per-cell percent, the fill-index relation, the bottom-right→left→up fill
`transformation`, a comparison against the prior 10×10 square, evidence
provenance, an explanation chain, and a colorization work-plan item.

```sh
# from the repository root
./impl/build/nordshrift check common-rails/common-rails.sst   # validate; NSS-* diagnostics
./impl/build/nordshrift build common-rails/common-rails.sst   # transpile + run on the core
```

## Building the toolchain

Both binaries come from the `impl/` tree (C11 + C++17):

```sh
cd impl && make        # produces build/sleela and build/nordshrift
```

## Relation to the Java edition

The fill mathematics are identical to the Java `CommonRails`: a display cell at
`(displayRow, displayCol)` maps to fill index
`((size-1)-displayRow)*size + ((size-1)-displayCol)`, and a cell is filled when
its fill index is `< filledCells`. The Java edition uses ANSI color (orange
filled on white empty); here the filled/empty distinction is carried by the
`█` / `░` glyphs, and colorizing the `c`/`java` targets is tracked as the
`colorizeTargets` work-plan item in the SST sheet.
