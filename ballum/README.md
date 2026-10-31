<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-logo-004.jpg" alt="SLeeLa">

# Ballum™ — pixel-level display control

**Package:** `ballum/`
**Date:** 2026-10-23

Ballum gives a SLeeLa program direct, bounded control over the screen/monitor of
the native OS: set an individual pixel's color, read it back, blit a rectangle
of pixels, and adjust whole-display **brightness**, **warmth** (color
temperature), and **gamma**. It targets **macOS, Linux, and Windows 10+** through
one portable manager over a per-OS backend — the same native-core + per-OS
backend shape that `wifi-common/` uses for the radio.

## Architecture

```
   SLeeLa program
        │  (via the lib/ballum facade + a host VM bridge)
        ▼
   ballum_manager_t  ──────────────►  ballum_backend_t  (one per OS)
   portable validation                 device I/O only
   (bounds, caps, ranges)              x11 · macOS · windows · unavailable
```

- **Portable manager** (`src/ballum_manager.c`) — all validation lives here:
  display-bounds checks, capability gating, and value clamping, so each backend
  stays thin and only performs device I/O. It caches the enumerated display list
  and copies results into caller/VM-owned memory (never exposes internal
  pointers).
- **Backend vtable** (`include/sleela_ballum/ballum_manager.h`) — a C
  struct-of-function-pointers: `enumerate`, `set_pixel`, `get_pixel`,
  `blit_region`, `set_brightness`, `set_warmth`, `set_gamma`, `poll_event`,
  `poll_key`, and the fullscreen-surface trio `claim_surface` / `present` /
  `release_surface`. Exactly one backend is compiled in per build;
  `ballum_backend_default()` returns it and is never NULL.

## Total per-pixel control: the fullscreen surface (World-Class path)

Drawing on the live desktop (`set_pixel`/`blit` with no surface claimed) is
**transient** — a compositor can repaint over it — and per-pixel device calls
are slow. For **total, owned, fast** per-pixel control, claim a fullscreen
surface:

```
claim_surface(id)  ->  set_pixel / blit (into an in-memory RGBA framebuffer)
                   ->  present(id)         (one XPutImage; MIT-SHM when available)
                   ->  release_surface(id)
```

- `claim_surface` creates an **override-redirect, always-on-top** fullscreen
  window sized to the display's CRTC, so the window manager/compositor leaves
  every pixel to the program.
- While claimed, `set_pixel`/`get_pixel`/`blit_region` operate on the
  framebuffer in memory — O(1) / row-wise `memcpy`, **no X round-trips** — and
  `present` pushes the whole frame to the screen in **one** `XPutImage` (a
  shared-memory `XShmPutImage` when the server has MIT-SHM). A 1920×1080 frame
  is one image transfer, not ~2 million `XDrawPoint` calls.
- The capability is advertised as `BALLUM_CAP_FULLSCREEN_SURFACE`; a backend
  that cannot provide it returns `UNSUPPORTED`, and the manager reports
  `ballum_manager_has_surface(id) == 0`.

On Linux this targets **X11** (including XWayland). Native Wayland does not grant
a client root/screen ownership through X11; true Wayland pixel control needs a
different protocol (wlr-layer-shell or DRM-KMS) and is a documented follow-up.

### Per-OS backends (open standards only)

| OS | File | Technology |
|---|---|---|
| Linux | `src/ballum_backend_x11.c` | X11 + XRANDR CRTC gamma ramp; **fullscreen framebuffer surface** via an override-redirect window + one `XPutImage` (MIT-SHM when available); DDC/CI backlight where available |
| macOS | `src/ballum_backend_macos.mm` | Core Graphics / Quartz (`CGDisplay*`, `CGSetDisplayTransferByFormula`) |
| Windows 10+ | `src/ballum_backend_windows.c` | Win32 GDI + `SetDeviceGammaRamp`, `EnumDisplayMonitors` |
| (fallback) | `src/ballum_backend_unavailable.c` | always present; fails closed with `BACKEND_UNAVAILABLE` |

`src/ballum_backend_select.c` maps the compile-time `BALLUM_BACKEND_*` define to
`ballum_backend_default()`; with none defined (unknown OS / headless) it selects
the unavailable backend, so calls fail cleanly and the program never crashes.

## Controls and ranges

| Control | Unit | Range |
|---|---|---|
| Brightness | percent | `0..100` |
| Warmth (color temperature) | kelvin | `1000..10000` (6500 = neutral D65) |
| Gamma | scaled ×100 | `10..1000` (i.e. `0.10..10.00`) |
| Pixel color | RGBA 8-bit | per channel `0..255` |

Every setter is clamped to its range and gated on the display's advertised
capability (`ballum_capability_t`) and bounds **before** the backend is called.

## Status codes (`ballum_status_t`)

`OK`, `INVALID_ARGUMENT`, `BACKEND_UNAVAILABLE`, `BACKEND_ERROR`,
`DISPLAY_NOT_FOUND`, `OUT_OF_BOUNDS`, `BUFFER_TOO_SMALL`, `UNSUPPORTED`,
`PERMISSION_DENIED`, `NOT_INITIALIZED`. A control the backend or display cannot
perform returns `UNSUPPORTED` — never a silent/false success.

## Build and test

```sh
# from ballum/ -- the real per-OS backend (auto-detects the OS):
make                 # -> build/libsleela_ballum.a
make PLATFORM=linux  # force a specific backend

# the portable test (links the 'unavailable' backend + an in-test mock
# backend, so it runs anywhere with no display libraries):
make test
```

The real OS backends require that platform's development libraries
(`libX11`/`libXrandr` on Linux; the CoreGraphics frameworks on macOS; `gdi32`/
`user32` on Windows) — the same per-OS SDK requirement as the existing
`wifi-common/` and `idea/realtime/` backends. The portable `make test` has no
such dependency.

## Security posture

- Validate bounds and ranges before any device write; fail closed.
- Copy native results into caller/VM memory; never hand out internal pointers.
- Open standards per OS (X11/XRANDR, Core Graphics, Win32 GDI) — no
  proprietary-only paths; an always-present unavailable fallback.

## SLeeLa-facing surface

The developer-facing classes live in `lib/ballum/` (`SLBallumManager`,
`SLBallumDisplay`, `SLBallumColor`, `SLBallumStatus`, and per-OS profile
classes under `lib/ballum/platforms/`). See `lib/ballum/README.md`.

> **SLeeLa-facing surface.** The fullscreen-surface trio is now wired all the
> way to SLeeLa. The host VM bridge (`impl/core/ballum_vm_bridge.c`) exposes
> `uiBallumHasSurface` / `uiBallumClaimSurface` / `uiBallumPresent` /
> `uiBallumReleaseSurface`, and the facade adds ergonomic methods:
> `SLBallumScreen.canFullScreen()` / `claimFullScreen()` / `present()` /
> `releaseFullScreen()` (over `SLBallumManager.hasSurface/claimSurface/present/
> releaseSurface`). See `lib/ballum/surface-demo.sleela` for a runnable
> walk-through. Each call is validated by the native manager and fails closed on
> a backend without the capability.

---

# SLeeLa
<p align="center"><img src="images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>
