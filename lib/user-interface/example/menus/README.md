# SLeeLa Menubar and Expandable Menu Example

This is the dedicated interactive menu showcase for the SLeeLa GUI toolkit.

Run from the repository root:

```sh
./impl/build/sleela run example/menus/gui-menubar-expandable.sleela
```

The example demonstrates:

- A single-row horizontal menubar with File, Edit, View, and Help.
- One top-level dropdown open at a time; clicking the active heading closes it.
- Nested **Recent Files** and **Panels** groups that expand and collapse in place.
- Menu-item activation updates the status label and dismisses the open menus.
- Native SleelaUI widgets and a small, bounded layout; no web or external GUI dependency.

This is a working example of the custom-widget fallback used by SLeeLa. It is
not a claim that every host has native operating-system menu rendering wired.
