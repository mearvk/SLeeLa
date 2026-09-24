# Guia™ Objects Reference

The standard object families are `GuiApplication`, `GuiClient`, `GuiListener`, `Circuit`, `Session`, `Control`, `DataModel`, and `Command`.

Every object has a stable `id`, a `type`, an optional `parent`, and lifecycle state. Active IDs MUST NOT be reused.

Reserved namespaces include `gui.*`, `window.*`, `view.*`, `control.*`, `layout.*`, `menu.*`, `dialog.*`, `data.*`, `event.*`, `command.*`, `client.*`, `listener.*`, `circuit.*`, `monitor.*`, `session.*`, `error.*`, and `security.*`.


## Visual Components

- `bevel` — dimensional 2D edge or divider; styles include `raised`, `sunken`, `etched`, `flat`, and `2d-bevel`.
- `gradient` — 2D surface transition; types include `linear`, `radial`, and `bilinear`.
- `style` — reusable visual declaration.
- `style-ref` — reference to a reusable style.

Visual styling is presentation metadata and does not alter the Guia™ lifecycle or security model.