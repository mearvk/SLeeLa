# Guia™ Visual Components — BODI/XML

## Purpose

Standard visual vocabulary for SLeeLa XML GUI definitions. Bevels and 2D gradients are first-class declarative components.

## Components

### `bevel`
Dimensional 2D edge, divider, or surface treatment. Attributes include `id`, `orientation`, `style`, `size`, `light`, `shadow`, and `base`.

### `gradient`
2D visual transition. Types: `linear`, `radial`, and `bilinear`.

### `style`
Reusable presentation declaration containing visual components.

### `style-ref`
Reference from a component or container to a reusable style.

## Authority rule

For every Skya GUI setup, the base BODI/XML document is the authoritative declaration of structure, visual components, and style references. JavaFX/native adapter code implements that declaration and should not silently create a conflicting GUI definition.

— Max Rupplin - MEARVK LLC - 2026
