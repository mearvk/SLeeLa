# Lesson 2 — Discover Displays

Before drawing or changing settings, enumerate displays and read their geometry and capabilities.

The existing [screen demo](../screen-demo.sleela) shows the primary-display selection pattern. The [inspection example](../examples/01-screen-inspection.sleela) prints each display's identifier, dimensions, primary flag, and capability mask.

## Workflow

1. Call `uiBallumEnumerate()`.
2. Check its status before reading the display count.
3. Iterate from index `0` to `count - 1`.
4. Read the ID, width, height, primary flag, and capability mask.
5. Select a display only after confirming it exists.

Display indices are enumeration positions; display IDs identify native displays. Do not assume the primary display is always index zero. Treat a zero-display result as a normal unavailable condition, especially in CI or headless environments.
