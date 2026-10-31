# Ballum example — white screen, "ESC to exit..." that fades

A single runnable SLeeLa program that shows exactly this:

- the whole screen **white**,
- the text **"ESC to exit..."** in **black**, about **14pt**, centered,
- the text then **fades out** (dissolving into the white) over ~48 frames,
- the **screen stays white**,
- **ESC** exits.

## File

- `white-screen-esc.sleela` — self-contained over the `uiBallum*` VM bridge, so
  it drives the real native Ballum manager (the same bridge the `lib/ballum`
  facade uses). It claims an **owned fullscreen surface** (override-redirect, so
  a compositor cannot paint over it), draws into the framebuffer, and
  **presents** each frame in one native image transfer.

## How the text is drawn

Ballum is a **pixel/blit** surface — it has no font engine. The text is drawn
from a tiny embedded **5×7 bitmap font** (only the glyphs of "ESC to exit..."),
scaled ×2 so a 5×7 cell is about 14pt on a typical display, plotted as black
pixels. The fade raises the text pixels' gray level 0 → 255 (black → white)
while the surrounding screen is already white, so the letters dissolve into the
white and nothing else changes.

## Run it

```sh
# needs the source-built compiler (supports #sleela 2.9):
impl/build/sleela run ballum/example/white-screen-esc.sleela
```

On a **Linux X11** host with the Ballum X11 backend compiled in (the bridge
built with `BALLUM_BACKEND_X11`), this paints a real white fullscreen surface
with the fading black text. On a **headless** host the native backend is the
unavailable fallback, so there is no display; the program prints an honest
status and exits without drawing a false frame.

> This example lives outside `/lib`, so it is not part of the library symbol
> inventory or the SHA-256 source manifest — it is a standalone demo.
