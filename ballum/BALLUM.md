# Ballum™ Display-Control Standard

**Date:** 2026-10-23

## 1. Purpose

Ballum is SLeeLa's standard for pixel-level control of the native OS
screen/monitor. It separates *what a display can do* (capabilities) from *how a
given OS does it* (per-OS backend), so a SLeeLa program writes one API and runs
on macOS, Linux, and Windows 10+.

## 2. Operations

| Operation | Meaning |
|---|---|
| `enumerate` | List connected displays with geometry + capabilities. |
| `set_pixel` / `get_pixel` | Write/read a single RGBA pixel. |
| `blit_region` | Write a tightly-packed RGBA rectangle. |
| `set_brightness` | Whole-display brightness, `0..100`. |
| `set_warmth` | Color temperature in kelvin, `1000..10000` (6500 neutral). |
| `set_gamma` | Gamma exponent, encoded ×100 (`10..1000`). |

## 3. Capability semantics (`ballum_capability_t`)

A backend advertises, per display, which operations it supports:
`PER_PIXEL`, `REGION_BLIT`, `BRIGHTNESS`, `COLOR_TEMPERATURE`, `GAMMA`. A
program checks the capability before enabling a control; a missing capability
yields `UNSUPPORTED` rather than a silent no-op.

## 4. Backend matrix

| OS | Backend name | Open-standard technology |
|---|---|---|
| Linux | `x11-xrandr` | X11, XRANDR CRTC gamma (DDC/CI where available) |
| macOS | `macos-coregraphics` | Core Graphics / Quartz transfer formula |
| Windows 10+ | `windows-gdi` | Win32 GDI device gamma ramp, EnumDisplayMonitors |
| (none) | `unavailable` | fail-closed fallback |

## 5. Security and correctness

The posture is the same as the rest of the native tree: validate bounds before
any device write, clamp every control to its documented range, reject
out-of-bounds pixels and mismatched blit buffers, copy results into
caller-owned memory, and fail closed. No proprietary-only display paths; open
standards per OS with an always-present fallback.

## 6. Status

The portable manager and all four backends are implemented. The portable
manager is covered by `tests/test_ballum_manager.c` (enumeration, capability
gating, bounds, clamping, fallback). The per-OS backends compile against their
platform SDKs. The SLeeLa developer surface is in `lib/ballum/`.

A live VM bridge (`impl/core/ballum_vm_bridge.*` + `SLUI_FN_BALLUM_*` dispatch),
mirroring the Wi-Fi bridge, wires the `lib/ballum` facade to the native manager
at runtime.

## 7. Keyboard input

Alongside pointer input, the bridge exposes a non-blocking key poll so a
full-screen session can exit by key. The backend vtable has an optional
`poll_key(context, int *out)`; the manager passes it through
(`ballum_manager_poll_key` / `ballum_manager_has_keyboard`), and the VM bridge
surfaces `uiBallumPollKey()` / `uiBallumHasKeyboard()`. Key codes are the
backend-neutral `ballum_key_t` set: Escape (27), Enter (13), and Space (32).
The Linux/X11 backend uses `XQueryKeymap`, Windows uses `GetAsyncKeyState`,
and macOS uses `CGEventSourceKeyState`. Each backend emits key-down edges
rather than repeating a key while it is held, so one long Escape press cannot
skip both stages of the session exit flow. A backend that is unavailable or
cannot poll keys still fails closed and reports no key; pointer capture remains
a separate capability with its own platform limitations.

## 8. Full-screen session (white splash + logo fade + ESC)

`lib/ballum/SLBallumSession` is the user-facing, easy-startup session built on
`SLBallumScreen` (pixels/geometry) and `SLBallumInput` (pointer + the key poll).
A session is **full screen by nature** — Ballum owns the whole display, not a
window. The session:

1. paints the display **pure white**,
2. **fades a logo in from the white background** across a configurable number of
   frames (opacity 0→100 %, blended toward the background so it emerges rather
   than pops in), and
3. runs until **ESC**: the first ESC yields full screen to **frame mode**, a
   second ESC **exits** the program. `requestExit()` and a pointer long-press
   are always-available fallbacks for hosts without a keyboard.

Easy startup is three lines — `configure()`, `start()`, then a `step()` loop —
with sensible defaults (primary display, the national bitcoin / ICC logo, a
~48-frame fade) overridable by the fluent `withLogo` / `withFadeFrames` /
`withDisplayIndex` / `withBackground` setters.

**Logo bytes.** The session loads its logo through the **image handler**
(`ballum/src/ballum_image.c`, `uiBallumLogoLoad(path)`), which decodes a JPEG
with the bundled nanojpeg (MIT) into an RGBA buffer and samples it scaled and
centered onto the display. Fail closed: a missing/undecodable file leaves
`uiBallumLogoReady()==0`, and the white fill + fade timeline + ESC still run —
never a false "ready", never a blit the core cannot perform.

## 9. Transition effects

The logo reveal runs through a catalog of **transition effects**
(`ballum/src/ballum_effects.c`, `ballum_fx_t`): a pure, deterministic,
integer-only per-pixel compositor. A session picks one with
`uiBallumLogoSetEffect(fx)` (or `SLBallumSession.withEffect(...)`); each frame
`uiBallumLogoBlend(...)` composites the fitted logo over the background at the
frame's progress using the selected effect. The 20 effects:

`fade`, `wipe-left/right/up/down`, `iris-in`, `iris-out`, `bars`, `blinds`,
`dissolve`, `pixelate`, `slide-left`, `slide-up`, `zoom`, `spotlight`,
`ripple`, `checker`, `diamond`, `clock`, `sparkle`.

Every effect frames the reveal as "foreground over background as t goes
0 → 100", so at full progress the logo is shown wherever it is opaque; a
transparent logo pixel always stays background. The effect catalog is surfaced
to SLeeLa by `SLBallumEffect` (ids + `uiBallumFxName`/`uiBallumFxByName`/
`uiBallumFxCount`). Covered by `ballum/tests/test_ballum_effects.c`.

Runnable demonstrator: `lib/ballum/session-demo.sleela` (lists every effect and
runs one; `FX=iris-in LOGO_FILE=/path/logo.jpg` to pick an effect and a logo).
