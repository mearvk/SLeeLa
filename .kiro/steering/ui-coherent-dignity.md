---
inclusion: always
---

# UI/UX Architectural Specification: Coherent Dignity

The governing design standard for every SleelaUI interface (`user-interface/`,
the `ui*` SLVM bridge, and the `example/` GUI demos). It applies to software
backed by the procedural C/C++ Sleela core and rendered through the toolkit's
software rasterizer (a custom `uint32_t` pixel frame-buffer blitted to
X11/Cocoa/Win32 — not ImGui/Qt/Raylib). Synthesises UI principles 1977–2026 into
a rigid, deterministic rendering system.

Core objective: **dignity of method** — a UI that behaves with predictable,
mathematically sound composure, respecting user autonomy ("freedom") and visual
harmony ("admission").

## 1. Architectural philosophy

**Dignity of Method.** Software must feel like a well-crafted physical
instrument. No jumping, flashing, or layout recalculation from unpredictable
payloads. Favour deterministic predictability over high-motion micro-interaction.
View-layer state is explicit and immutable, cleanly separated from the C/C++
execution loops.

**Freedom (ergonomics of long-term occupancy).** Users sit with the software for
years. Low fatigue: dark/light modes target strictly bounded contrast. Spatial
memory: structural landmarks (navigation, action grids) stay statically
anchored; components never drift or auto-rearrange. Zero hostility: no aggressive
pop-ups, no layout shift (CLS), no predictive path assumptions.

**Admission (colour coexistence).** Colours must admit one another into a shared
spectrum under programmatic harmony, not arbitrary brand palettes. Adjacent
colours must never vibrate, strain, or compete for dominance.

## 2. Layout, positioning, spatial scale

Deterministic, integer-based grid. Absolute alignment under direct pixel
manipulation / software blitting.

- **Explicit hierarchy** from the top-left origin (0,0). No browser-style
  flexible flow that breaks predictability.
- **Frame isolation.** Every module lives in a strictly bounded BBox; child
  coordinates are relative to the immediate parent. (SleelaUI: `Box`, `Card`,
  `Frame`, `ScrolledView`, `HeaderBar` all clip children to their content box.)
- **Collision avoidance.** Elements never overlap dynamically. Overflowing text
  is truncated with an ellipsis (…) or bounded in an internal scroll-clip.

**Baseline grid = 8px.** All margins, padding, and dimensions resolve to
multiples of 8 (8, 16, 24, 32, 48, 64). In the toolkit the theme spacing unit is
`unit = 4` (a sub-module); structural steps use even multiples so the effective
module is 8px — audit new layout for off-grid values (avoid lone `unit`=4 and
`unit*3`=12 for structural spacing; prefer `unit*2`=8, `unit*4`=16, `unit*6`=24).

**Metric weight ratios (1X = 8px):**

| Component | Ratio | Px | Allocation |
|---|---|---|---|
| Global menu / status bar | 1X | 8 (height module) | top bar |
| Micro spacing | 1X | 8 | internal padding, label gutters |
| Element structural height | 4X | 32 | buttons, text-entry lines (`control_height`) |
| Header banner height | 6X | 48 | module control bars, tool strips |
| Minimum sidebar width | 24X | 192 | primary system trees |
| Context footer | 0.5X | — | system status footer |

## 3. Colour architecture & "Admission"

Colours are mathematical relationships, not decorative assets.

**Contrast & luminance matrix.** Background-to-text contrast is strictly bounded
**4.5:1 (min visibility) … 7:1 (max comfort)**. Pure `#FFFFFF` on pure `#000000`
is **banned** (ocular ghosting). Light surfaces use a near-white neutral (warm
parchment); dark surfaces use deep obsidian, never pure black.

- Background: low-stimulus neutral.
- Surface layer: +10% luminance step off the background.
- Component bounds: a muted border line.
- Text: high-readability mid-chroma counterpart within the 4.5–7:1 band.

**Functional allocation.**
- Neutral backgrounds: restrained low-chroma tones mimicking industrial
  materials (slate gray, deep obsidian, warm parchment).
- Interactive elements step luminance up by ~15% off the baseline background
  hue rather than introducing an unrelated accent.
- Semantic signalling: Red (error) / Yellow (warning) / Green (success) must be
  heavily desaturated, cover **< 2%** of screen real-estate, and always pair
  with a structural glyph (`[!]`, `[✓]`) for accessibility.

## 4. Typography, alignment, hierarchy

Crisp pixel clarity via integer-snapped font metrics. Line-heights land on the
8px baseline grid so paragraph wraps align with layout boxes.

| Role | Size / line-height | Use |
|---|---|---|
| System / metric labels | 11 / 16 | dense data grids, technical readouts |
| Standard interface body | 14 / 24 | inputs, body text, button labels |
| Sub-section headers | 18 / 24 | module cards, inner pane titles |
| Primary window title | 24 / 32 | top-level structural anchors |

- **Numerical columns:** right-aligned, tabular monospaced digits (audit by
  decimal place).
- **Textual labels:** left-aligned (consistent vertical scanning axis).

## 5. Procedural C/C++ backend interface blueprint

The UI layer is an immutable, stateless projection of the C/C++ core state. It
reads state and passes interactions through an asynchronous, non-blocking ring
buffer. (SleelaUI: the `ui*` bridge marshals packed state over `OP_UI_CALL`; the
event loop pumps via `uiAppPump` without blocking the core.)

Packed, byte-aligned descriptor the UI polls/receives:

```c
typedef struct {
    uint32_t component_id;
    uint8_t  is_interactive;
    uint8_t  render_state_flags; // Active, Hovered, Disabled
    int16_t  screen_x;
    int16_t  screen_y;
    int16_t  width;
    int16_t  height;
    char     display_text[128];
} UI_Element_Render_Descriptor;
```

**Execution-matrix constraints.**
1. **Separation of loops.** The interface thread never blocks on engine
   calculation, disk, or network. If the core stalls, the UI keeps rendering a
   static cached frame with a muted "Core Busy" flag.
2. **Input coordinate clamping.** Pointer (X,Y) events are validated and clamped
   against the active component BBox before entering the event ring.
3. **No hidden state side-effects.** Changing a component's scale or visibility
   must be explicitly declared in the central layout state; no arbitrary
   on-the-fly visual alterations.

## Conformance when writing SleelaUI code

- Clip children to the frame (hard BBox); never let content spill margins.
- Keep structural landmarks statically anchored; no auto-rearrange.
- Use the metric ratios above; keep structural spacing on the 8px module.
- Use the bounded-contrast presets (no pure black/white); desaturate semantics
  and pair them with a glyph.
- Overflowing text → ellipsis or scroll-clip, never overlap.
- Treat the UI as a stateless projection of the core; never block the UI thread
  on core work — show the cached frame + "Core Busy".
