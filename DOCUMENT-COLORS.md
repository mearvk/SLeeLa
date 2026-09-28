# SLeeLa GUI and Document Color System

SLeeLa uses a consistent six-color vocabulary for graphical interfaces and document treatments.

## Base palette

**Yellow, Gray, and Black** are the SLeeLa base colors.

- **Yellow** — identity, attention, active controls, focus, selection, and highlights.
- **Gray** — structure, neutral surfaces, borders, separators, metadata, and secondary controls.
- **Black** — foundation, terminal surfaces, strong text, dark panels, and formal framing.

**White, Red, and Blue** are support colors used when their semantic distinction is needed.

- **White** — readable surfaces, document pages, light controls, and contrast.
- **Red** — error, danger, rejection, invalid state, destructive action, or security warning.
- **Blue** — information, navigation, links, and network/service context.

## Semantic roles

| Color | GUI role | Document role |
|---|---|---|
| Yellow | active, focused, selected, emphasized | headings, definitions, key callouts |
| Gray | structure, panel, border, secondary state | rules, tables, metadata |
| Black | foundation, terminal, primary text | titles, code, formal text |
| White | readable surface, light control | page/background surface |
| Red | error, danger, destructive action | warnings and failures |
| Blue | information, navigation, links | references and informational notes |

## Default GUI treatment

The default SLeeLa composition is **Black / Gray / Yellow**:

1. Black supplies the foundation.
2. Gray supplies structural interface material.
3. Yellow identifies the current SLeeLa action or attention state.
4. White is introduced for readable content surfaces and contrast.
5. Red is reserved for actual error or danger states.
6. Blue is reserved for information, navigation, and service/network context.

Not every interface needs all six colors.

## Document treatment

Technical and formal documents should generally use Black for primary text and code, Gray for structure and metadata, Yellow for controlled emphasis, and White for the reading surface.

Operational documents may add Red for faults and Blue for informational or network material.

The same semantic vocabulary applies to Markdown, HTML, generated reports, API documentation, and future GUI reference material.

## Accessibility

Color must not be the only carrier of meaning. Error, selection, focus, and informational states should also have text, icons, outlines, labels, or other non-color indicators.

The named colors are **semantic tokens**, not a demand for one fixed RGB value on every platform. Implementations may choose accessible shades appropriate to the display, toolkit, light/dark context, and contrast requirements while preserving the token's meaning.

## Platform themes

Operating-system and toolkit themes may change the rendered shade while preserving the SLeeLa role:

```text
Yellow -> identity / attention
Gray   -> structure / neutral
Black  -> foundation
White  -> readable surface
Red    -> error / danger
Blue   -> information / navigation
```

## Scope

This system applies to SLeeLa GUI applications, terminal-adjacent interfaces, API and developer documentation, HTML/Markdown documentation, generated reports, and future desktop components.

Plain-text source and machine-readable protocol data do not need to contain color information.

---

*Copyright (C) 2026 MEARVK LLC*
