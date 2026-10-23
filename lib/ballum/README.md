<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa Ballum™ — pixel-level display control

**Status: SLeeLa-side facade + per-OS profiles over the native Ballum core.**
This directory is the language-facing layer for Ballum, the SLeeLa feature that
gives a program pixel-level control of the native OS screen/monitor — set a
pixel's color, blit a region, and adjust **brightness**, **warmth** (color
temperature), and **gamma** — on **macOS, Linux, and Windows 10+**.

The actual device I/O lives in the native core at `ballum/`
(`ballum/include/sleela_ballum/ballum_manager.h`). These `.sleela` classes own
request/state/validation semantics and honest status reporting. Like
`lib/wifi/`, the facade is a **bridge boundary**: it never drives a display on
its own; a host SLVM binding connects its methods to the native
`ballum_manager_*` API. Until that binding is installed, operations return
`BRIDGE_REQUIRED` — but every bounds, range, and capability check still runs
locally, so a program fails cleanly and never asks the native layer to write an
out-of-range pixel or an impossible control.

## Classes

| File | Role |
| --- | --- |
| `SLPackage.sleela` | Package facade (`configure`/`name`/`available`). |
| `SLBallumManager.sleela` | The bridge-boundary facade a program drives: bind the native handle, enumerate, then `request*` a pixel/blit/brightness/warmth/gamma op (validated before dispatch). |
| `SLBallumDisplay.sleela` | One connected display: id, geometry, primary flag, and the capability bitmask (`supportsPerPixel`/`supportsBrightness`/…), mirroring `ballum_display_t`. |
| `SLBallumColor.sleela` | An 8-bit RGBA color with clamped channels, mirroring `ballum_color_t`. |
| `SLBallumStatus.sleela` | Result codes mirroring `ballum_status_t`, plus the SLeeLa `BRIDGE_REQUIRED` code and `text(code)`. |

Per-OS profile classes (the `lib/antivirus` profile pattern) under
`platforms/<Brand>/<OS>/`:

| File | OS | Backend |
| --- | --- | --- |
| `platforms/GNU/Linux/SLLinuxBallumProfile.sleela` | Linux | `x11-xrandr` |
| `platforms/Apple/macOS/SLMacOSBallumProfile.sleela` | macOS | `macos-coregraphics` |
| `platforms/Microsoft/Windows/SLWindowsBallumProfile.sleela` | Windows 10+ | `windows-gdi` |

Each profile exposes the backend name and per-OS capability facts
(`supportsColorTemperature()`, `supportsDdcBacklight()`, …), so the SLeeLa layer
can reason about OS differences without touching the device.

## The flow

```
bindNativeHandle(h)                       → BRIDGE_READY (or BACKEND_UNAVAILABLE)
acceptEnumerateResult(status, count)      → ENUMERATED; displays() known
display.supportsBrightness()              → gate the control in the UI
mgr.requestBrightness(display, 70)        → OK (bridge dispatches) or UNSUPPORTED
mgr.statusText()                          → human-readable status
```

## Result codes

Mirror `ballum_status_t` (see `SLBallumStatus`):

| Code | Meaning |
| --- | --- |
| `0` OK | ok |
| `1` INVALID_ARGUMENT | bad arguments |
| `2` BACKEND_UNAVAILABLE | no backend on this host |
| `3` BACKEND_ERROR | the OS display call failed |
| `4` DISPLAY_NOT_FOUND | unknown display id |
| `5` OUT_OF_BOUNDS | pixel/region outside the display |
| `6` BUFFER_TOO_SMALL | enumerate buffer too small |
| `7` UNSUPPORTED | display/backend lacks this capability |
| `8` PERMISSION_DENIED | OS refused the operation |
| `9` NOT_INITIALIZED | manager not initialized |
| `100` BRIDGE_REQUIRED | native bridge not installed (SLeeLa-side) |

## Ranges (mirror `ballum_manager.h`)

- **Brightness**: `0..100` percent.
- **Warmth**: `1000..10000` kelvin (6500 = neutral D65).
- **Gamma**: encoded ×100, `10..1000` (i.e. `0.10..10.00`).
- **Color**: RGBA, each channel `0..255`.

`SLBallumManager` exposes `clampBrightness`/`clampWarmth`/`clampGamma` mirroring
the native `ballum_clamp_*` helpers.

## Relationship to the native tree

- Native manager + per-OS backends: `ballum/`
  (`ballum/include/sleela_ballum/ballum_manager.h`, `ballum/src/*`).
- Bridge convention mirrors `lib/wifi/` (`SLWifiManager` + the VM bridge). A live
  `impl/core/ballum_vm_bridge.*` is the planned next step to connect this facade
  to the native manager at runtime.

## Implementation boundary

These classes manage, classify, validate, and report. Real pixel/brightness/
warmth/gamma control requires the native `ballum/` backend plus a host VM
bridge. Without the bridge, operations report `BRIDGE_REQUIRED`; with a backend
that lacks a capability, they report `UNSUPPORTED` — never a false success.
