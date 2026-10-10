# SLeeLa `boolean` package — Boolean/Binary syllable words

**Revision:** 0.1
**Family:** `boolean`

## Idea

Some Return Values plainly imply a **TRUE** or a **FALSE**. Others imply a
*further relation*. The `boolean` package makes that relation first-class.

A Boolean **word** is an ordered sequence of **syllables**, each either
`BOOLEAN.TRUE` or `BOOLEAN.FALSE`:

```
BOOLEAN.TRUE
BOOLEAN.FALSE
BOOLEAN.TRUE.FALSE
BOOLEAN.TRUE.FALSE.FALSE
BOOLEAN.X.X.X.X.Y ...
```

- The **HEAD** syllable is the **result**.
- Every following syllable is a **carry** — a *caveat structure* that must be
  observed, or agree.

So `BOOLEAN.TRUE.FALSE` reads: *the result is TRUE, but there is a concern of
carry — the second syllable is FALSE. Having expected mainly the structure of a
single return syllable, TRUE is the result, yet FALSE is carried as a caveat.*

## Layout

```
lib/boolean/
  SLPackage.sleela         package facade (names the package)
  BooleanTrue.sleela       primitive syllable  BOOLEAN.TRUE
  BooleanFalse.sleela      primitive syllable  BOOLEAN.FALSE
  boolean-demo.sleela      runnable demonstrator
  ARCHITECTURE.md          this document
  generate-boolean.py      deterministic generator (filesystem is the truth)
  8/                       FULL enumeration — all 2^8 = 256 depth-8 words
      Bool_TTTTTTTT.sleela ... Bool_FFFFFFFF.sleela
  16/                      depth-16 word factory + canonical boundary words
      BooleanWord16.sleela   constructs/validates/reads any 2^16 word on demand
      Bool_<16 glyphs>.sleela  all-TRUE, all-FALSE, TRUE+carry-FALSE, FALSE+carry-TRUE
  32/                      depth-32 word factory + canonical boundary words
      BooleanWord32.sleela   constructs/validates/reads any 2^32 word on demand
      Bool_<32 glyphs>.sleela  the four boundary words
```

### File naming

Names are **coherent to the value**: one glyph per syllable, `T` = `TRUE`,
`F` = `FALSE`, head syllable first. Thus `BOOLEAN.TRUE.FALSE.FALSE` →
`Bool_TFF` (at depth 8, `Bool_TFFFFFFF`).

## Why depth 8 is enumerated but 16 and 32 are generative

| Depth | Combinations | Form |
|------:|-------------:|------|
| 8  | 256 | **Fully enumerated** — one `Bool_<glyphs>.sleela` per word. |
| 16 | 65,536 | **Factory** `BooleanWord16` + 4 canonical boundary files. |
| 32 | 4,294,967,296 | **Factory** `BooleanWord32` + 4 canonical boundary files. |

Materializing 65,536 files bloats the tree, and 4.29 **billion** files for
depth 32 is physically impossible. Every depth-16 and depth-32 word is still a
usable value — `BooleanWord16` / `BooleanWord32` construct, validate, and read
any combination on demand with the exact same HEAD/result + CARRY/caveat
semantics as the enumerated depth-8 classes.

## Reading a word

Each word class exposes a uniform surface:

| Member | Meaning |
|---|---|
| `word()` | the dotted `BOOLEAN.X.X...` rendering |
| `glyphs()` | the compact `T`/`F` form |
| `depth()` | number of syllables |
| `head()` / `headSyllable()` | the **result** (TRUE/FALSE) |
| `carry()` / `carryDepth()` | the trailing **caveat structure** |
| `caveat()` | true when a `FALSE` is carried behind the head |
| `agrees()` | true when the word is uniform (all syllables equal the head) |
| `trueCount()` / `falseCount()` | syllable tallies |
| `bits()` / `value()` | binary reading, TRUE=1 FALSE=0, head most significant |

The factory classes (`BooleanWord16`, `BooleanWord32`) add `configure(glyphs)`
and `valid()`.

## Using it in SLeeLa

See `boolean-demo.sleela`. In short:

```
Bool_TFFFFFFF r = new Bool_TFFFFFFF();   // BOOLEAN.TRUE.FALSE.FALSE...
r.headSyllable();  // "TRUE"  — the result
r.caveat();        // true    — a carry concern must be observed

BooleanWord16 w = new BooleanWord16();
w.configure("TFFFFFFFFFFFFFFF");
w.valid();         // true
w.head();          // true    — result TRUE
w.caveat();        // true    — FALSE carried as caveat
```

**SLeeLa — MEARVK LLC — 2026**
