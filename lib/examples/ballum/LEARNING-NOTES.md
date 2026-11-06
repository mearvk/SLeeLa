# Ballum examples — what to learn here

Ballum is SLeeLa's display/frame layer. These examples teach the **arithmetic of
a frame**: how a rectangle of pixels is described, how content is fitted inside
it, and how a renderer decides which parts to repaint. They are
*calculation-only* — no native display backend is opened, so they run anywhere
and always print the same numbers.

Read them in order; each adds one idea.

## The three cases

| Level | File | The one new idea |
|---|---|---|
| Basic | [`BASIC-frame-metrics.sleela`](BASIC-frame-metrics.sleela) | A frame is width × height: pixel count and aspect ratio. |
| Moderate | [`MODERATE-fit-and-letterbox.sleela`](MODERATE-fit-and-letterbox.sleela) | Fit a source inside a frame keeping aspect ratio, then center it (letterbox / pillarbox). |
| Advanced | [`ADVANCED-dirty-rects.sleela`](ADVANCED-dirty-rects.sleela) | Track changed regions ("dirty rectangles"), union them, and measure frame coverage. |

The original [`CLASSIC-display-frame.sleela`](CLASSIC-display-frame.sleela) is the
shortest starter.

## What we want you to take away

1. **A display is numbers before it is pixels.** Every decision a renderer makes
   — how big, where, how much to repaint — is arithmetic you can do and verify
   without a screen. That is what these examples show.
2. **Aspect-ratio fitting is a "smaller of two ratios" rule.** The moderate
   example scales by `min(frameW/srcW, frameH/srcH)` so the whole source fits,
   then splits the leftover space into equal margins. Whichever axis has margin
   tells you letterbox vs pillarbox.
3. **Real renderers repaint only what changed.** The advanced example models
   dirty rectangles and unions them into one bounding box, then compares its
   area to the whole frame. This is why a well-written UI does not redraw the
   entire screen on every keystroke.

## Practical notes for SLeeLa 1.7

- **No math intrinsics.** There is no `sqrt`/`abs`/rounding function. Use the
  operators `+ - * / %`, and promote to `double` (e.g. `width * 1.0`) when you
  want a fractional result instead of integer division.
- **`(int)` does not truncate here.** At this language level an `(int)` cast on a
  double does not drop the fractional part, so the moderate example keeps fitted
  sizes as doubles and leaves pixel rounding to the backend. When you want an
  integer-ish percentage, scale first (the advanced example reports coverage in
  *per-mille* using integer math: `unionArea * 1000 / frameArea`).
- **Object arrays** follow the standard idiom: a static-field array, elements
  built in typed locals and stored back, and read via typed locals. See
  `ADVANCED-dirty-rects.sleela`.

## Run them

```sh
sleela check lib/examples/ballum/BASIC-frame-metrics.sleela
sleela run   lib/examples/ballum/MODERATE-fit-and-letterbox.sleela
```

Output is deterministic. The moderate example reports a 1280×1024 source
pillarboxed into a 1920×1080 frame with 285-pixel side margins; the advanced
example reports a union rectangle covering 375 per-mille of the frame.
