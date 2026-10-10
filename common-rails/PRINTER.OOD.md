# CommonRails Printer — Object-Oriented Design (OOD)

This document is the object-oriented design for the CommonRails print
configurations. It describes a small set of **configuration objects** and a
**`Printer`** that consumes them, so a developer can assemble their own print
method in a Sleela source document by constructing and wiring these objects —
rather than calling flat helpers with long positional argument lists.

The working implementation is [`PrinterConfig.sleela`](PrinterConfig.sleela);
it `check`s and `run`s on the toolchain today. The flat, procedural edition
remains in [`CommonRails.sleela`](CommonRails.sleela). This document is the
reference for both the model that exists now and how it is intended to grow.

## 1. Why objects

The procedural API couples every concern into one call site:

```java
// procedural: alignment, width, glyphs, counts all inline on every call
printTable("Component", 9, "State", 5, 18, 10,
           "SearchEngineClient", 18, "WORKING", 7,
           "CommonRails", 11, "COMPLETE", 8);
```

The OO model names each concern as an object a developer builds once and reuses:

```java
PrinterConfig cfg  = p.defaults();
ColumnConfig  comp = p.column("Component", 9, 18, true);
ColumnConfig  stat = p.column("State", 5, 10, true);
p.table2(cfg, comp, stat, /* rows... */);
```

Restyling every table is then a one-object swap:

```java
cfg.table = p.asciiTable();   // all tables now render in ASCII
```

## 2. The object model

Sleela `struct`s are reference types created with `new`, with `.` field access
and reference semantics (see `impl/examples/struct_basic.sleela`). The config
objects are structs; the `Printer` is a class that operates on them.

```
                         ┌──────────────────┐
                         │  PrinterConfig   │   the printer "object" a
                         ├──────────────────┤   developer tunes
                         │ printWidth  :int │
                         │ oidWidth    :int │
                         │ currentWidth:int │
                         │ squareSize  :int │
                         │ full        :Str │
                         │ light       :Str │
                         │ table   :TableStyle ───────┐
                         │ state   :StateStyle ──┐    │
                         └──────────────────┘    │    │
                                                 │    │
             ┌───────────────┐   ┌───────────────▼┐  ┌▼───────────────┐
             │  ColumnConfig │   │   StateStyle   │  │   TableStyle   │
             ├───────────────┤   ├────────────────┤  ├────────────────┤
             │ header    :Str│   │ field     :int │  │ tl tm tr   :Str│
             │ headerChars:int│  └────────────────┘  │ ml mm mr   :Str│
             │ width     :int│                        │ bl bm br   :Str│
             │ leftAlign :boo│   ┌────────────────┐  │ h  v       :Str│
             └───────────────┘   │   PadConfig    │  │ inset     :int │
                                 ├────────────────┤  └────────────────┘
                                 │ width     :int │
                                 │ fill      :Str │
                                 │ leftAlign :boo │
                                 └────────────────┘

                         ┌──────────────────────────┐
                         │          Printer         │  behaviour: factories
                         ├──────────────────────────┤  that build the configs,
                         │ pad/left/right/zeros()   │  and renderers that
                         │ column() stateStyle()    │  consume them.
                         │ lightTable() asciiTable()│
                         │ defaults()               │
                         │ applyPad() padInt()      │
                         │ line() field()           │
                         │ component() current()    │
                         │ stateLine()              │
                         │ tableTop/Sep/Bottom()    │
                         │ tableHeader() tableRow() │
                         │ table2()                 │
                         │ square() pixel()         │
                         │ clampPercent()           │
                         │ percentToCells()         │
                         └──────────────────────────┘
```

## 3. The configuration objects

### `PadConfig` — one padding rule

| Field | Type | Meaning |
|---|---|---|
| `width` | int | field width in characters |
| `fill` | String | fill string (`" "` for spaces, `"0"` for zero-pad) |
| `leftAlign` | boolean | `true` = content then fill; `false` = fill then content |

Built by `pad(width, fill, leftAlign)` and the conveniences `left(width)`,
`right(width)`, `zeros(width)`. Applied with `applyPad(cfg, content, chars)`.

> **Contract.** Padding is clamped at zero; content is never truncated. The
> character count is explicit because the core has no string-length operator.

### `ColumnConfig` — one table column

| Field | Type | Meaning |
|---|---|---|
| `header` / `headerChars` | String / int | header text and its char count |
| `width` | int | column width in characters |
| `leftAlign` | boolean | cell alignment for this column |

Built by `column(header, headerChars, width, leftAlign)`.

### `TableStyle` — the frame

The eleven box-drawing glyphs (`tl tm tr ml mm mr bl bm br h v`) plus `inset`
(spaces inside each cell). Swapping a `TableStyle` restyles every table.
`lightTable()` is the Unicode default; `asciiTable()` is a `+ - |` fallback.
New styles (heavy `┏━┓`, double `╔═╗`) are added by writing another factory.

### `StateStyle` — state-label field

| Field | Type | Meaning |
|---|---|---|
| `field` | int | fixed width the `[STATE]` label is laid into |

`stateLine` guarantees at least one space after the label even if a label is
wider than `field`.

### `PrinterConfig` — the composed printer

Aggregates the scalar settings (`printWidth`, `oidWidth`, `currentWidth`,
`squareSize`, `full`, `light`) and *has-a* `TableStyle` and `StateStyle`.
`defaults()` returns the canonical CommonRails printer (80 columns, 10-digit
IDs, 39-wide Current field, 21×21 square, `█`/`░`, light table).

## 4. Composing a print method (developer recipe)

```java
Printer p = new Printer();          // the renderer

// 1. a printer, tuned from defaults
PrinterConfig cfg = p.defaults();
cfg.printWidth = 100;               // widen lines if desired

// 2. reusable column objects
ColumnConfig name = p.column("Name", 4, 20, true);    // left-aligned
ColumnConfig size = p.column("Size", 4, 8, false);    // right-aligned

// 3. render
p.component(cfg, "MyService", 9, 7, 1, "starting");
p.stateLine(cfg, "WORKING", 7, "indexing documents", 18);
p.tableTop(cfg.table, name, size);
p.tableHeader(cfg.table, name, size);
p.tableSep(cfg.table, name, size);
p.tableRow(cfg.table, name, "report.pdf", 10, size, "2.4MB", 5);
p.tableBottom(cfg.table, name, size);
p.square(cfg, p.percentToCells(cfg, 50));
```

A developer who wants a different frame writes one factory and assigns it:

```java
cfg.table = p.asciiTable();         // or a custom heavy/double style
```

## 5. Design rules carried from Beautiful Design

The object model preserves every invariant in
[`BEAUTIFUL.DESIGN.PRINTING.md`](BEAUTIFUL.DESIGN.PRINTING.md):

- **Meaning survives without color** — structure and glyphs carry state; a
  `TableStyle`/glyph swap changes presentation, never meaning.
- **Deterministic widths** — all width arithmetic is explicit and clamped at
  zero; content is never silently truncated.
- **Stable identity/geometry** — component prefix, Object-ID width, and square
  geometry are config fields, held stable by `defaults()`.
- **Agreement of labels and rendering** — percentages are normalized once
  (`clampPercent` → `percentToCells`) and both the label and the square use the
  same value.

## 6. Growth plan (the forward-looking part)

The model is intentionally small. Natural extensions, each additive:

1. **N-column tables.** When the core gains arrays (a tracked candidate in
   `impl/README.md`), add `TableConfig { ColumnConfig[] columns }` and a
   variadic `row(...)`; until then, two-column `table2` plus the per-piece
   `tableRow` calls cover arbitrary rows.
2. **More styles.** `heavyTable()` (`┏━┓`), `doubleTable()` (`╔═╗`), and a
   color-aware style once the target exposes ANSI — added as factories, no
   renderer change.
3. **Per-cell alignment** independent of the column default — an optional
   alignment argument on `tableRow`.
4. **`PrintState` enum object** carrying both label and glyph, so
   `stateLine(cfg, state)` takes a `StateConfig` instead of raw text + count.
5. **Output targets.** A `Writer` object (stdout/string/file) so `Printer`
   emits through a target rather than calling `print` directly — mirroring the
   Heritage `PrintWriter` responsibility.

## 7. Relationship to the other editions

| Edition | File | Role |
|---|---|---|
| Procedural | [`CommonRails.sleela`](CommonRails.sleela) | flat helpers; the smallest surface |
| **Object-oriented** | [`PrinterConfig.sleela`](PrinterConfig.sleela) | configurable objects + `Printer` |
| Semantic | [`common-rails.sst`](common-rails.sst) | the math/formatting contract |
| Heritage | [`heritage/`](heritage/) | C / C++ / Java implementations of the contract |

All editions render the same observable output for the same state; this
document is the authority for the object model that the Sleela OO edition
realises.
