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
mirroring the Wi-Fi bridge, is the planned next step to wire the `lib/ballum`
facade to the native manager at runtime.
