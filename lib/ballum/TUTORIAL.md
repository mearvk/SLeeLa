# Ballum Full-Screen: Quick Start and Careful Use

Ballum is SLeeLa's pixel-accurate native-display API. These examples target the current `master` tree and use the logo assets already in `images/`.

## 1. Run the full-screen white splash

From the repository root:

```sh
make -C impl
./impl/build/sleela run lib/ballum/session-demo.sleela
```

The demo requests the primary display, paints the full surface white, loads
`images/national-bitcoin-icc-logo-007.jpg`, and fades the logo in. When the
session ends, it fades back to white. The first Escape enters frame mode; the
second Escape exits. Keep calling `step()` while in frame mode so the second
press is handled.

## 2. Choose the alternate logo

```sh
LOGO_FILE=images/national-bitcoin-icc-logo-006.jpg FX=fade ./impl/build/sleela run lib/ballum/session-demo.sleela
```

Other available effect names are printed by the runnable. Try `FX=iris-in`,
`FX=wipe-left`, or `FX=dissolve`. Use the repo-root working directory so
relative image paths resolve.

## 3. Tune the reveal

```sh
SESSION_FRAMES=48 FX=fade LOGO_FILE=images/national-bitcoin-icc-logo-007.jpg ./impl/build/sleela run lib/ballum/session-demo.sleela
```

A larger `SESSION_FRAMES` value gives a longer fade. Use the default modest
value for quick testing; the demo frame loop uses a 16 ms busy-wait cadence
because the current SLeeLa surface does not expose a sleep/yield primitive.
That cadence is simple but can consume CPU while the demo is running.

## 4. Minimal session API pattern

The standard library session API is in `lib/ballum/SLBallumSession.sleela`.
Its intended usage pattern is: call `finish()` after the exit loop to fade the
logo back out and restore the configured background (white by default).

```text
SLBallumSession s = new SLBallumSession();
s.withLogoFile("images/national-bitcoin-icc-logo-007.jpg");
s.withEffect(SLBallumEffect.FADE);
s.withFadeFrames(48);
s.start();
while (s.step() != SLBallumSession.EXIT) {
  // Keep polling; first Escape enters FRAME, second Escape exits.
}
s.finish(); // fade the logo back to white, then restore a clean white screen
```

This is an API illustration; use the runnable demo above as the verified
entry point for the current single-file `sleela run` workflow.

## Careful-use checklist

- Run from the repository root when using relative paths.
- Save work before launching a full-screen display-control demo.
- Confirm the selected monitor and resolution before using pixel-level APIs.
- Test on a normal user desktop first; do not run as root.
- On a headless machine, expect the backend to fail closed rather than claim a
  display was opened.
- Check `uiBallumLogoReady()` before assuming image decoding succeeded.
- Keep keyboard polling in the loop and continue until `EXIT`.
- Avoid extremely large frame counts and avoid running multiple full-screen
  sessions simultaneously.
- This demo is a display/rendering example, not a cryptocurrency wallet or
  proof of any seizure or crime.

## Assets

- [Logo 006](../../images/national-bitcoin-icc-logo-006.jpg)
- [Logo 007](../../images/national-bitcoin-icc-logo-007.jpg)
- [Ballum standard](../../ballum/BALLUM.md)

## Validation notes

The source and paths are updated in the repository. A successful remote commit
does not itself prove the current host can open a display or decode a JPEG.
Run `make -C ballum test` and `make -C impl all`, then try the demo on a
machine with a supported graphical display. These commits have not been
validated by a live full-screen run in this session.
