<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa Website Generator

Build custom, exciting websites from scratch out of **designs** and **signs**.

SLeeLa is authoritative for the generation: it takes a high-level design
intent (a theme, a palette, a layout grammar, and a set of *signs* — the
semantic content blocks a page is made of) and emits a complete, self-contained
static website (HTML + CSS, optional progressive-enhancement JS). A JavaFX GUI
is provided as a thin presentation/authoring surface so users and developers can
compose a site by hand and preview it.

## Architectural rule

> **JavaFX presents; SLeeLa decides.**

The GUI never generates markup itself. It collects a design intent and a list of
signs, hands them to the SLeeLa generator through the common Java connector
(process / RMI / HTTP), and renders back whatever the generator produced.

```text
   Design intent (theme, palette, layout grammar)
   + Signs (hero, feature, gallery, cta, footer, ...)
                     |
                     v
         SLeeLa Website Generator            <-- authoritative
   (website/*.sleela : design model + emitter)
                     |
         +-----------+-----------+
         |                       |
   static site out/          live preview
   (index.html + styles)          |
                              JavaFX GUI
                        (website-generator/gui)
```

## Vocabulary

- **Design** — the global look: a `Theme` (named), a `Palette` (brand/base/
  accent colors), typography scale, spacing rhythm, border radius, motion level,
  and a `LayoutGrammar` (how sections stack and align). "Exciting" is a first
  class dial: `energy` in `0..100` drives gradients, motion, and contrast.
- **Sign** — a semantic content block that *means* something on the page:
  `hero`, `feature`, `gallery`, `pricing`, `testimonial`, `cta`, `nav`,
  `footer`, `richtext`. A page is an ordered list of signs. The generator maps
  each sign to markup + styling derived from the active design.
- **Site** — one or more pages plus the shared design. The emitter renders a
  complete static site.

## Layout

```text
website-generator/
  README.md                 This document.
  DESIGN.md                 The design/sign model in detail.
  Makefile                  Build/preview helpers (dispatch into SLeeLa).
  website/                  SLeeLa generator (authoritative).
    design.sleela           Theme, Palette, LayoutGrammar, energy dial.
    signs.sleela            The Sign catalog and sign -> markup mapping.
    emitter.sleela          Assembles signs under a design into HTML + CSS.
    site.sleela             Top-level: compose a demo site and emit it.
  samples/                  Example design intents and generated output.
    starter.site            A sample site description (design + signs).
  gui/                      JavaFX authoring + preview surface.
    pom.xml
    src/main/java/module-info.java
    src/main/java/com/mearvk/sleela/website/gui/
      WebsiteStudioApp.java         JavaFX application (presentation only).
      WebsiteDesignModel.java       Presentation model + connector bridge.
      Sign.java                     Sign record used by the GUI.
```

## Status

The SLeeLa generator defines the full design/sign/emit pipeline and a runnable
demo (`website/site.sleela`). The JavaFX GUI is a complete, buildable authoring
shell that is wired to call the generator through `SleelaJavaConnector`; the live
connector transport is the next integration point and is clearly marked.

**Max Rupplin — MEARVK LLC — 2026**