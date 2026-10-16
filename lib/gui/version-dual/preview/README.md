# lib/gui/version-dual — visual preview

`gui-dual-preview.png` is a visual preview of
[`gui-components-demo.sleela`](../gui-components-demo.sleela), the 4.0 (dual)
professional component set: audio recorder, video canvas, HTML-aware HTTP URL
bar, custom click object (orbit + apprentice ladder), exact-position layout,
graphics facility, and the admin authority surface — in the glass aesthetic
(accent `#2E8BFF`, corner radius 14, elevation 55).

## What this is — and is NOT

> **Rendered with Chrome, from the SLeeLa descriptors — NOT the live SLeeLa toolkit.**

- `gui-dual-preview.html` is a hand-authored HTML/CSS mock that mirrors the exact
  component states and style descriptors the `.sleela` model produces (recorder
  stopped @ 72% level / 1.5 s, video playing 0:30/2:00 = 25%, URL bar HTTPS HTML
  200, click object orbit stage 3/3 final-call fired, graphics 4-command canvas,
  admin signed-in/sign-off/signature-for-Law).
- `gui-dual-preview.png` is that HTML screenshotted **headlessly by Chromium**.

It is a faithful picture of **what the model describes**, not pixels emitted by
the SLeeLa GUI host. The production look is produced by the actual host renderer
(Swing / JavaFX / native bridge) painting the real widgets and effects; this
repo's sandbox has no display server, no prebuilt `sleela` runtime, and the
native GUI bridge is ABI-only (no pixels), so the live toolkit could not render
here. The preview exists to show the design intent.
