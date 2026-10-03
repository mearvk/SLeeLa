# Website Generator — Design & Sign Model

This document is the contract between the SLeeLa generator and any front end
(the JavaFX GUI, a CLI, or an HTTP caller). It describes the two inputs — a
**design** and an ordered list of **signs** — and the output.

## 1. Design

A design is the global look applied to every page.

| Field          | Type        | Meaning                                                        |
|----------------|-------------|----------------------------------------------------------------|
| `theme`        | string      | Named theme, e.g. `aurora`, `brutalist`, `editorial`, `neon`.  |
| `brand`        | color (hex) | Primary brand color.                                           |
| `base`         | color (hex) | Page background base color.                                   |
| `accent`       | color (hex) | Accent / call-to-action color.                                |
| `font`         | string      | Display font family stack.                                    |
| `radius`       | int (px)    | Corner radius rhythm (0 = sharp, 24 = soft).                  |
| `energy`       | int 0..100  | The "exciting" dial — drives gradients, motion, contrast.     |
| `grammar`      | enum        | Layout grammar: `stack`, `split`, `grid`, `canvas`.           |

`energy` is deliberately a single, legible dial so a user can go from calm
(`editorial`, energy 10) to loud (`neon`, energy 95) without touching markup.

## 2. Signs

A sign is a semantic block. The generator owns the mapping from a sign to the
markup/style it produces under the active design. Supported signs:

| Sign          | Primary fields                              | Renders as                         |
|---------------|---------------------------------------------|------------------------------------|
| `nav`         | `brand`, `links[]`                          | Sticky top navigation.             |
| `hero`        | `headline`, `subhead`, `cta`                | Full-bleed hero with energy gradient.|
| `feature`     | `title`, `items[] {icon,title,body}`        | Feature grid.                      |
| `gallery`     | `title`, `images[]`                         | Responsive image grid.             |
| `pricing`     | `title`, `tiers[] {name,price,points[]}`    | Pricing cards.                     |
| `testimonial` | `quote`, `author`                           | Pull-quote band.                   |
| `cta`         | `headline`, `button`                        | Conversion band in accent color.   |
| `richtext`    | `html`                                      | Prose section.                     |
| `footer`      | `brand`, `columns[]`                        | Multi-column footer.               |

A sign carries a small set of string fields (keyed values). The generator fills
in everything else — layout, spacing, color, motion — from the design, so the
same sign list produces a radically different site under a different design.

## 3. Output

The emitter produces a single self-contained document per page:

- One `<style>` block derived from the design (CSS custom properties for the
  palette, a spacing/typographic scale, and energy-driven gradients/animation).
- The ordered signs rendered to semantic HTML.

The result is dependency-free: no external CSS/JS is required to view it.

## 4. Front-end contract

A front end submits a *site description* — a design plus an ordered sign list —
and receives the generated HTML. In SLeeLa terms the generator exposes:

- `designNew(theme, brand, base, accent, font, radius, energy, grammar)`
- `siteNew(design)`
- `signHero(site, headline, subhead, cta)` and one builder per sign kind
- `siteEmit(site)` → the full HTML string

The JavaFX GUI mirrors these calls through `SleelaJavaConnector`; it never emits
markup itself. See `gui/`.

**Max Rupplin — MEARVK LLC — 2026**
