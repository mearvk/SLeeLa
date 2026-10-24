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
| `SLBallumScreen.sleela` | **The clean, pixel-accurate Screen API** — the high-level surface a developer integrates with (see below). |
| `SLBallumManager.sleela` | The low-level bridge-boundary driver: bind the native handle, enumerate, then live `setPixel`/`setBrightness`/`setWarmth`/`setGamma` through the `uiBallum*` VM bridge (plus `request*` pre-validation helpers). |
| `SLBallumDisplay.sleela` | One connected display: id, geometry, primary flag, and the capability bitmask (`supportsPerPixel`/`supportsBrightness`/…), mirroring `ballum_display_t`. |
| `SLBallumColor.sleela` | An 8-bit RGBA color with clamped channels, mirroring `ballum_color_t`. |
| `SLBallumStatus.sleela` | Result codes mirroring `ballum_status_t`, plus the SLeeLa `BRIDGE_REQUIRED` code and `text(code)`. |
| `SLBallumInput.sleela` | **The touchscreen + mouse input listener** — polls pointer events and synthesizes click/move/drag gestures (see below). |
| `SLBallumPointerEvent.sleela` | One pointer event (type/x/y/button/source/contact/modifiers), mirroring `ballum_pointer_event_t`. |

## Input: touchscreen + mouse (`SLBallumInput`)

Ballum is not just output — `SLBallumInput` is a clean listener for **mouse and
touchscreen** input over the whole screen. The native backend reports raw
pointer events (mouse and touch unified as `DOWN`/`MOVE`/`UP`), and the listener
folds them into the gestures a developer actually wants — **click** and
**drag** — with a small, explicit state machine and a configurable drag
threshold.

The core VM has no callbacks, so input is **polled**: call `poll()` each frame;
it returns a gesture code and latches the event position/button/source for the
accessors. Drain until it returns `NONE`.

| Gesture (`poll()` return) | Meaning |
| --- | --- |
| `NONE` (0) | nothing this poll |
| `DOWN` (1) | a raw press, not yet resolved to click/drag |
| `MOVE` (2) | hover/move with no button held |
| `CLICK` (3) | press + release at the same spot (a tap, on touch) |
| `DRAG_BEGIN` (4) | movement past the threshold while pressed |
| `DRAG_MOVE` (5) | continued drag movement |
| `DRAG_END` (6) | release that ends a drag |

```java
SLBallumInput input = new SLBallumInput();
input.configure();
input.setDragThreshold(4);                 // pixels of slop before tap -> drag

if (input.available()) {                    // a pointer backend is present
    int g = input.poll();                   // call each frame; drain to NONE
    while (g != SLBallumInput.NONE) {
        if (g == SLBallumInput.CLICK) {
            screen.plotRgb(input.x(), input.y(), 0, 255, 0);   // paint where tapped
        } else if (g == SLBallumInput.DRAG_MOVE) {
            screen.plotRgb(input.x(), input.y(), 255, 255, 0); // trail while dragging
        }
        g = input.poll();
    }
}
```

| Method | Purpose |
| --- | --- |
| `configure()` / `setDragThreshold(px)` | prepare; tune the click-vs-drag threshold |
| `available()` | is a pointer-input backend present on this host |
| `poll()` | fold one raw event into a gesture; returns a gesture code |
| `gesture()` / `gestureName(g)` | the last gesture and its readable name |
| `x()` / `y()` / `button()` / `source()` / `contact()` / `modifiers()` | latched event fields |
| `isTouch()` / `isMouse()` / `isPressed()` / `isDragging()` | state queries |
| `pressX()` / `pressY()` | where the current press/drag started |
| `snapshot()` | an `SLBallumPointerEvent` value object of the latched event |

**Platform support:** the Linux (X11) backend provides live pointer input
(`XSelectInput` + button/motion events). The macOS and Windows backends report
`available() == false` for now — whole-screen global capture there needs a
CGEventTap (macOS, with Accessibility permission) or a `WH_MOUSE_LL` hook
(Windows), each with its own lifecycle; until wired, they fail cleanly rather
than fake an input stream. Runnable demo: `input-demo.sleela`.

## The Screen API (`SLBallumScreen`) — pixel accuracy, cleanly

`SLBallumScreen` is the surface most developers should use. It wraps the
low-level `SLBallumManager` for one bound display and offers a small, polite,
well-named API with **pixel accuracy as its contract**: every placement is
validated against the live display geometry *and* the display's advertised
capability **before** it reaches the device, so a write either lands exactly
where asked or returns a precise, readable status — it never silently clips,
wraps, or no-ops. Coordinates use a top-left origin: `x` in `[0, width)`,
`y` in `[0, height)`.

```java
SLBallumScreen screen = new SLBallumScreen();
screen.create();
screen.attach(nativeHandle);     // host VM provides the backend handle
screen.open();                   // enumerate + bind the primary display

if (screen.isReady() && screen.canPlotPixels()) {
    screen.plotRgb(10, 20, 255, 0, 0);          // one exact red pixel
    screen.fillRect(0, 0, 32, 32, screen.rgb(0, 128, 255));
    int packed = screen.sample(10, 20);         // read back: 0xAARRGGBB
}
if (screen.canSetBrightness()) { screen.brightness(70); }   // 0..100 %
if (screen.canSetWarmth())     { screen.warmth(3400); }     // kelvin (warm)
if (screen.canSetGamma())      { screen.gammaPoint(2, 20); }// gamma 2.20
print(screen.lastMessage());                                // readable status
```

| Method | Purpose |
| --- | --- |
| `create()` / `attach(handle)` / `open()` / `openIndex(i)` | prepare, bind the native handle, enumerate + bind a display |
| `isReady()` / `width()` / `height()` / `name()` / `displayId()` / `isPrimary()` | readiness + geometry/identity of the bound display |
| `screenCount()` / `primaryIndex()` | discovery across displays |
| `canPlotPixels()` / `canSetBrightness()` / `canSetWarmth()` / `canSetGamma()` | capability gates to check before enabling a control |
| `inBounds(x, y)` | the accuracy gate every placement passes |
| `rgb(r,g,b)` / `rgba(r,g,b,a)` | build a color at the call site |
| `plot(x,y,color)` / `plotRgb(x,y,r,g,b)` | set exactly one pixel |
| `sample(x,y)` | read a pixel back as packed `0xAARRGGBB` (verify accuracy) |
| `fillRect(x,y,w,h,color)` | fill an exact rectangle, pixel by pixel |
| `brightness(%)` / `warmth(kelvin)` / `gamma(scaled)` / `gammaPoint(whole,hundredths)` | whole-display controls |
| `statusCode()` / `ok()` / `lastMessage()` | precise, readable result of the last op |

Runnable demo: `screen-demo.sleela`.

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
- VM bridge: `impl/core/ballum_vm_bridge.c` and `impl/core/ballum_vm_bridge.h` expose the `uiBallum*` boundary used by the live demos. Verify that the bridge is included in the particular runtime build before expecting device I/O.

## Implementation boundary

These classes manage, classify, validate, and report. Real pixel/brightness/
warmth/gamma control requires the native `ballum/` backend plus a host VM
bridge. Without the bridge, operations report `BRIDGE_REQUIRED`; with a backend
that lacks a capability, they report `UNSUPPORTED` — never a false success.


## Examples and Tutorial Series

The package now includes a progressive learning path:

- **[Examples](examples/README.md)** — five focused SLeeLa programs covering display inspection, safe pixel writes, capability-gated display controls, bounded pointer polling, and headless-safe behavior.
- **[Tutorials](tutorials/README.md)** — seven lessons covering architecture, display enumeration, coordinate safety, display controls, pointer input, status/testing, and the native bridge.

Start with `examples/05-headless-safe.sleela` and `examples/01-screen-inspection.sleela` before trying operations that visibly change a display. The examples are source additions; compilation and runtime behavior still need to be verified with the project's actual SLeeLa toolchain and target OS.


## Full-Screen White Splash and Logo Fade

For a ready-to-run, full-screen white display with the repository logo fading in,
see **[Ballum Full-Screen Tutorial](TUTORIAL.md)**. From the repository root:

```sh
make -C impl
./impl/build/sleela run lib/ballum/session-demo.sleela
```

The default splash uses [logo 007](../../images/national-bitcoin-icc-logo-007.jpg);
select [logo 006](../../images/national-bitcoin-icc-logo-006.jpg) with:

```sh
LOGO_FILE=images/national-bitcoin-icc-logo-006.jpg FX=fade ./impl/build/sleela run lib/ballum/session-demo.sleela
```

The runnable paints a white background, fades the logo in, handles Escape, then
fades the logo out and restores white. The reusable `SLBallumSession` API now
provides `finish()` for the same graceful fade-out; call it after the `step()`
loop. Relative asset paths assume the repository root is the working directory.
These are source-level examples; validate the build and display backend on your
machine before relying on live display control.
