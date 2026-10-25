# The four standard UI themes (`lib/ui`)

Four basic, excellent, powerful flavors — **Blue, Gray, Black, White** — over one
unified token model. Pick a flavor (or let a source hint pick it) and the
renderer draws the whole UI from its role tokens. Pure SLeeLa; every class checks
and runs.

## The flavors and their psychology

The four are a **family**: one accent family, one shared status set, one
contrast discipline. Each exists for a reason, not a whim.

| Flavor | Intent | Why | bg | surface | text | accent |
|--------|--------|-----|----|---------|------|--------|
| **Blue** | trust | deep navy lowers arousal; steel-blue reads dependable, not loud | `#0B1A2F` | `#12263F` | `#EAF2FB` | `#4F8CFF` |
| **Gray** | neutral | graphite recedes so content leads; low eye-fatigue | `#17191C` | `#212429` | `#E7E9EC` | `#6EA8FE` |
| **Black** | focus | matte near-black floor + crisp white text; premium restraint | `#0A0A0C` | `#121214` | `#F4F6F8` | `#5E9CFF` |
| **White** | clarity | soft off-white (never `#FFFFFF` glare) + dark ink; daylight reading | `#F7F8FA` | `#FFFFFF` | `#16202C` | `#1F6FEB` |

Shared across all four (so they are learnable): `danger #D9534F`, `success
#3FA776`, `warning #E0A33E`. Every text/surface and accentText/accent pairing is
tuned to clear **WCAG 2.1 AA** contrast — that is the quality bar.

## The classes

| Class | Role |
|---|---|
| `SLStandardTheme` | the **unified token model** every flavor fills (bg/surface/surfaceHi/chrome/border/text/textDim/accent/accentText/focus + status) |
| `SLThemeFlavor` | the **four presets** — `blue()`, `gray()`, `black()`, `white()`, plus `byId()` / `byName()` |
| `SLThemeHint` | **source-hint → renderer**: resolves the flavor a document intends into a ready-to-render theme |

## Pick a flavor directly

```sleela
SLThemeFlavor flavors = new SLThemeFlavor(); flavors.create();
SLStandardTheme t = flavors.blue();        < or .gray() / .black() / .white() >
String style = t.toStyle();                < what the renderer consumes >
```

## Let the source hint the flavor (the renderer obeys)

A document states the flavor it intends with a single word — a `#theme blue`
convention your app reads, a document-design keyword, or a user setting — and
`SLThemeHint` resolves it:

```sleela
SLThemeHint hint = new SLThemeHint(); hint.create();
hint.setFallback("Black");                 < used if the hint is unknown/empty >

SLStandardTheme theme = hint.resolve("white");   < from the source hint >
print(hint.report());                      < "theme hint 'white' -> White" >
< ... hand `theme` (its tokens) to the renderer; it produces the design ... >
```

An unknown hint falls back **deliberately** (never a guess) and says so:
`theme hint 'chartreuse' not recognized -> fell back to Black`. The renderer
never parses the hint itself — `SLThemeHint` does, and returns a complete theme.

## Relationship to the organizing Frame and the native theme

- Pair a theme with an `SLLayoutFrame` (the organizing root): the Frame arranges
  the components, the theme colors them — layout and flavor are orthogonal.
- The native engine (`user-interface/slui_theme.*`, `gui/native/sleela_gui_theme.*`)
  carries its own presets for the C/C++ renderer. These four standard flavors are
  the pure-SLeeLa, source-selectable layer; their token names line up with the
  native role model so a host can map one to the other.

Runnable: [`theme-demo.sleela`](theme-demo.sleela).
