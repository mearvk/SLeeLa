# CSS Snapshot — "what counts as CSS" at a point in time

| | |
|---|---|
| **Formal title** | CSS Snapshot 2023 |
| **Publisher** | W3C (CSS Working Group) |
| **Status** | A stable, dated collection of the modules that together define CSS today. |
| **Canonical reference** | <https://www.w3.org/TR/css-2023/> |
| **Source in this archive** | [`CSS-Snapshot-2023.bs`](./CSS-Snapshot-2023.bs) — the W3C Bikeshed source. See [`../SIGNATURES.md`](../SIGNATURES.md). |

Because post-CSS2 development is modular, the W3C publishes a periodic **Snapshot** that lists the
modules considered part of "CSS" at that date. This is the closest thing to a modern "CSS version
number." The modules below are exactly those named in the archived 2023 Snapshot.

## Modules in the CSS 2023 Snapshot

### Foundations
- CSS Syntax Level 3
- CSS Values and Units Level 3
- CSS Cascading and Inheritance Level 4 & 5 (`@layer`)
- CSS Custom Properties for Cascading Variables Level 1 (`--var`, `var()`)
- CSS Conditional Rules Level 3 & 4 (`@supports`, `@media`, `@container`)
- CSS Box Model Level 3 · Box Sizing Level 3 · Display Level 3

### Selectors & logic
- Selectors (the Snapshot references the current Selectors level)
- CSS Containment Level 1 (`contain`, container queries groundwork)

### Color & images
- CSS Color Level 4 (`oklch()`, `lab()`, `color()`, relative color)
- CSS Color Adjustment Level 1 (`color-scheme`, `prefers-color-scheme`)
- CSS Images Level 3 (`gradient()`s, `image-set()`, `object-fit`/`object-position`)
- CSS Compositing and Blending Level 1 (`mix-blend-mode`, `background-blend-mode`)
- CSS Masking Level 1 · CSS Shapes Level 1

### Text & fonts
- CSS Text Level 3 · CSS Text Decoration Level 3 · CSS Line Level 3
- CSS Fonts Level 3 & 4 · CSS Font Loading Level 3
- CSS Writing Modes Level 3 & 4 · CSS Logical Properties and Values Level 1

### Layout
- CSS Flexible Box (Flexbox) Level 1
- CSS Grid Layout Level 1 & 2
- CSS Multi-column Layout Level 1
- CSS Positioned Layout Level 3
- CSS Box Alignment Level 3
- CSS Fragmentation Level 3
- CSS Scroll Snap Level 1 · CSS Scrollbars Styling Level 1

### Interaction, motion & effects
- CSS Transforms Level 1 & 2 (`transform`, `translate`/`rotate`/`scale`, 3D)
- CSS Transitions Level 1
- CSS Animations Level 1
- CSS Easing Functions Level 1 (`cubic-bezier()`, `steps()`, `linear()`)
- CSS Will Change Level 1 (`will-change`)
- CSS View Transitions Level 1

### Lists, counters, UI, speech
- CSS Lists and Counters Level 3 · CSS Counter Styles Level 3 (`@counter-style`)
- CSS Basic User Interface Level 3 (`cursor`, `outline`, `appearance`, `caret-color`, `resize`)
- CSS Speech Level 1

## How the versions relate

```
CSS1 (1996) ─ one document
      │
CSS2 (1998) / CSS 2.1 (2011) ─ one document (core model)
      │
      ▼
CSS3+ ─ many independent MODULES, each at its own Level
      │
      ▼
CSS Snapshot (dated) ─ the list of modules that = "CSS" that year
```

See [`../css1-1996/FEATURES.md`](../css1-1996/FEATURES.md),
[`../css2.1-2011/FEATURES.md`](../css2.1-2011/FEATURES.md), and
[`../css3-modules/FEATURES.md`](../css3-modules/FEATURES.md) for the per-level catalogues.
