# Lesson 3 — Pixel Coordinates and Bounds

Ballum's documented coordinate convention uses a top-left origin. For a display of width `w` and height `h`, valid coordinates satisfy:

- `0 <= x < w`
- `0 <= y < h`

The last valid pixel is `(w - 1, h - 1)`; `(w, h)` is outside the display. Check the per-pixel capability before issuing a write, and use the native status as the final authority.

## Recommended sequence

1. Enumerate displays and select one.
2. Check the per-pixel capability bit (`1`).
3. Validate coordinates before calling the bridge.
4. Call the pixel API and inspect the returned status.
5. Log failures; never turn a failure into a success message.

Try [02-safe-pixel.sleela](../examples/02-safe-pixel.sleela). It intentionally submits one invalid coordinate to demonstrate that out-of-bounds writes should be rejected.
