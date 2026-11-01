# `/lib/common-rails` — one professional-printing surface for the toolchain

CommonRails is the SLeeLa toolchain's **shared output surface**. It gives the
four output producers — the compiler (**Sleelvac**), the **Nordshrift**
transpiler, a **running program**, and the **debugger** — one aligned,
column-formatted, box-drawn look instead of four ad-hoc ones.

## The entry class: `PrintRail`

`PrintRail.sleela` is the resolvable, one-class-per-file library unit (no
`main`). Because the driver auto-resolves `/lib` by scanning capitalized
identifiers, **just naming `PrintRail` pulls it in** — no `import` needed:

```java
#sleela 2.5
class MyApp {
    static void main() {
        PrintRail rail = new PrintRail();
        rail.printComponent("MyApp", 5, 7, 1, "started up");   // component line
        rail.printState("WORKING", 7, "crunching numbers", 17); // state line
        rail.printSquarePercent(50);                            // 21x21 square
        rail.debugFrame("compute()", 9, "MyApp.sleela:5", 14);  // bordered table
    }
}
```

```sh
./impl/build/sleela run MyApp.sleela
```

### Why an explicit character count on every call

The Core value model has no string-length or character indexing, so every
content primitive takes the content's character count explicitly (`"MyApp"` →
`5`). Widths are then measured arithmetically, and cells are **padded, never
truncated**.

## What `PrintRail` renders

| Concern | Methods |
|---|---|
| Professional component line | `printComponent(name, nameChars, oid, dateSeq, message)` / `componentLine(...)` |
| State-labelled status line | `printState(state, stateChars, message, messageChars)` / `stateLineText(...)` |
| 21×21 progress square (441 cells) | `printSquare(filledCells)`, `printSquarePercent(percent)`, `percentToCells(percent)` |
| Single-pixel progress indicator | `printPixel(filledCells)`, `printPixelPercent(percent)` |
| Bordered two-column table | `printTable(...)`, `printTableRow(...)`, `printTableTop/Separator/Bottom(...)` |
| Field/value pair (plain **or** framed) | `printPair(field, fieldChars, value, valueChars, table)` |
| Producer rails | `compilerState(...)`, `nordshriftState(...)`, `debugFrame(symbol, symbolChars, location, locationChars)` |

The builders (`componentLine`, `stateLineText`, `tableRowText`, `tableRule`)
return the rendered **string**, so a caller can `print(...)` it or capture it;
the `print*` methods are the convenience surface on top.

## The four producer surfaces

- **Compiler / Sleelvac** — `printComponent(...)` for the current unit,
  `compilerState("START"/"COMPLETE", ...)` for progress.
- **Nordshrift** — `printComponent(...)` for the sheet, `nordshriftState(...)`
  for lowering/transpile progress.
- **Running program** — `printSquarePercent(...)` / `printPixelPercent(...)`
  for work progress, plus any of the lines/tables above.
- **Debugger** — `debugFrame(symbol, ..., location, ...)` renders a stack
  frame as a bordered table; call `printTable(...)` directly for multi-row
  state dumps.

## Files

| File | Role |
|---|---|
| `PrintRail.sleela` | **The library unit** — the full renderer, auto-resolved by naming `PrintRail`. |
| `common-rails.sleela` | **Runnable demo** (`main`) showing all four producer surfaces; `./impl/build/sleela run lib/common-rails/common-rails.sleela`. |
| `SLPackage.sleela` | Package facade / descriptor for the `common-rails` module. |
| `Print*.sleela` | Per-concern facade units carrying the Metadata-Class metapattern. |

## Relation to the rest of the tree

- The authoritative, free-standing Sleela editions live in the repo-root
  [`common-rails/`](../../common-rails/) (`CommonRails.sleela`, the OO
  `PrinterConfig.sleela`, and the SST sheet).
- A native C and C++ implementation of the **same** contract (identical layout
  constants, glyphs, and states) lives under
  [`common-rails/heritage/printing/`](../../common-rails/heritage/printing/),
  so a C/C++ producer can emit the identical surface.
