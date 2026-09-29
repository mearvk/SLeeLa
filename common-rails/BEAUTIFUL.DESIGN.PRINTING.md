# CommonRails Beautiful Printing Pattern

## Purpose

CommonRails defines the reusable printing pattern for SLeeLa's **Beautiful Design**:
clear information hierarchy, stable alignment, restrained decoration, deterministic
progress geometry, and machine-checkable semantics.

The pattern is presentation-neutral at its core. Glyphs, color, and target-specific
terminal features may enhance presentation, but must not change meaning.

## 1. Canonical component line

```text
-- : [Object ID: 0000001234] [Date: 1] [Current: @SearchEngineClient           ] . message .
```

| Field | Contract |
|---|---|
| Prefix | `-- : ` identifies a component/status record. |
| Object ID | Non-negative decimal identifier, zero-padded to width 10. |
| Date | Sequence/date value supplied by the caller. |
| Current | `@ClassName` identity inside a fixed-width field. |
| Message | Human-readable operation/status text surrounded by `. ` decorators. |

The line remains readable when color is unavailable.

## 2. Visual hierarchy

Beautiful Design printing presents information in this order:

1. **Identity** — what component is speaking.
2. **State** — what it is doing now.
3. **Progress** — how far the operation has advanced.
4. **Result** — what completed, failed, or changed.
5. **Detail** — optional diagnostic information.

Decoration must not compete with state or message.

## 3. Fixed-width print contract

Beautiful Design uses fixed-width print regions so separate print statements occupy
the same organized space.

The canonical default is:

| Property | Contract |
|---|---|
| Print width | **80 characters** |
| Content | Begins at the declared content position |
| Short content | Right-padded with spaces |
| Long content | Must be wrapped by the caller/rendering layer; it is never silently truncated |
| Alignment | Repeated print statements use identical field geometry |
| Color | Optional; it does not change width semantics |

The SLeeLa implementation provides:

```text
PRINT_WIDTH = 80
printWidthLine(content, contentChars)
printField(content, contentChars, fieldWidth)
```

Because the current core surface does not provide general string-length or character
indexing operations, the content character count is explicit in the primitive's
contract. This keeps width calculation deterministic.

Example:

```text
[START]   CommonRails printing initialized
[WORKING] Content remains aligned inside the declared width
[COMPLETE] Fixed-width output ready
```

Each logical line is padded to 80 columns. When content is too long for the available
region, the rendering layer wraps it onto continuation lines rather than changing
the declared geometry.

## 4. Canonical progress square

The CommonRails square is **21×21 = 441 cells**.

Fill order:

```text
bottom-right → left across the row → up one row → left → ...
```

A cell is filled when its fill index is less than the requested filled-cell count.

Required boundary states:

- 0 — empty
- 1 — bottom-right corner
- 21 — complete bottom row
- 22 — bottom row plus first cell of the next row
- 440 — one cell remaining
- 441 — complete

## 5. Percentage pattern

Percentages are normalized before rendering:

```text
p < 0    → 0
p > 100  → 100
cells    → floor(p × 441 / 100)
```

The displayed percentage and rendered cell count must use the same normalized
value.

Recommended form:

```text
progress 50% (220/441 cells)
```

## 6. Single-pixel pattern

For compact status displays:

```text
░  = no progress
█  = progress has started
```

This is a binary activity/state indicator, not a replacement for the measured
441-cell progress state.

## 7. Glyph and color rules

Core meaning is carried by structure and glyphs:

- `█` = filled
- `░` = empty

Color is optional presentation metadata. A target may use ANSI or another color
system, but removing color must leave the same interpretation.

## 8. Alignment rules

- Keep component prefixes identical.
- Keep Object ID width stable.
- Keep Current-field width stable when the name fits.
- Never truncate a component name merely to preserve padding.
- Clamp padding at zero when content exceeds the declared width.
- Keep messages concise and semantically complete.

## 9. State vocabulary

Beautiful Design should distinguish state without relying solely on color:

```text
[START]       operation has begun
[WORKING]     operation is active
[PROGRESS]    measurable progress is available
[COMPLETE]    operation completed
[WARN]        operation continues with a non-fatal condition
[ERROR]       operation failed or cannot continue
```

These are conventions, not mandatory text on every line.

## 10. Rendering architecture

Every visual element should have a deterministic semantic source:

```text
Sleela source
    ↓
semantic contract
    ↓
rendering rules
    ↓
target presentation
```

Target-specific color, fonts, terminal features, or graphical treatment must not
redefine the underlying state.

## 11. Reusable operation pattern

```text
-- : [Object ID: 0000001234] [Date: 1] [Current: @ComponentName          ] . START message .
  progress 0% (0/441 cells)
░

-- : [Object ID: 0000001234] [Date: 2] [Current: @ComponentName          ] . WORKING message .
  progress 50% (220/441 cells)
[21x21 square]

-- : [Object ID: 0000001234] [Date: 3] [Current: @ComponentName          ] . COMPLETE message .
  progress 100% (441/441 cells)
[21x21 square]
```

The messages are application-specific. Geometry, alignment, state semantics, and
progress mathematics are reusable.

## 12. Design invariants

A conformant Beautiful Design implementation must preserve:

- stable component identity;
- state understandable without color;
- deterministic progress mathematics;
- unambiguous 0% and 100%;
- deterministic row transitions;
- agreement between labels and rendered progress;
- target-independent semantics;
- identical logical output for identical source state.

## 13. Relationship to SST

The CommonRails SST companion is the semantic authority for the square's
mathematical contract. This document defines the broader presentation pattern
for SLeeLa tools, IDE output, builders, loaders, test suites, debuggers, and
future graphical interfaces.

Target-specific presentation may extend the pattern while preserving these
invariants.
