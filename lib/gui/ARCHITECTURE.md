# SLeeLa `lib/gui` — the SLeeLa-facing GUI vocabulary

**Revision:** 0.1
**Family:** `gui`

`lib/gui` is the **SLeeLa-facing consequence** of the top-level [`gui/`](../../gui/)
work. The `gui/` directory holds the Java host, the native C ABI bridge
(`gui/native/sleela_gui_bridge.{h,cpp}`), and the 1..14 document-change
listener; `lib/gui` is the `.sleela` contract a SLeeLa program uses to express
that same model. The two are kept in correspondence: every class here mirrors a
concept documented in [`gui/INTEGRATION.md`](../../gui/INTEGRATION.md) and
[`gui/DOCUMENT_LISTENER.md`](../../gui/DOCUMENT_LISTENER.md).

## The division of authority

The architecture is intentionally asymmetric (see `gui/INTEGRATION.md`):

- **SLeeLa** owns business logic, state transitions, and the *meaning* of events.
- **Java** owns Swing/JavaFX presentation and desktop lifecycle.
- **The native bridge** is the stable low-level boundary to the C/C++ core.

Java is a native desktop *host* for SLeeLa, not a second business-logic
implementation.

## Classes

| Class | Mirrors (in `gui/`) | Role |
|---|---|---|
| `SLGuiBackend` | `SleelaGui.create(backend)` | toolkit-neutral backend selector: `swing` / `javafx` (alias `fx`) / `native` |
| `SLGuiWindow` | `SleelaGui` | window intent: `show`, `setText`, and the `refresh(id, revision)` document sink |
| `SLGuiAction` | `SleelaGuiRuntime.action` / `SleelaGui.onAction` | a named action bound to a SLeeLa operation; take-the-latch activation |
| `SLGuiDocument` | `SleelaDocument` | immutable watched-document snapshot: id, path, OS revision (`lastModified:size` / `absent`) |
| `SLGuiDocumentListener` | `DocumentListener` | the 1..14 document watch; OS-driven refresh; revision coalescing |
| `SLGuiRuntime` | `SleelaGuiRuntime` | **Path 2** — SLeeLa intent drives the window directly |
| `SLGuiHost` | `SleelaGuiHost` | **Path 1** — a Java host ties actions/changes to SLeeLa operations |
| `SLGuiBridge` | `sleela_gui_bridge.h` | **Path 3** — the shared native ABI (create/call/refresh/destroy) |
| `SLGuiIntegration` | `gui/INTEGRATION.md` | the recommended three-path posture |

A runnable, self-contained walkthrough is `lib/gui/version-mual/gui-demo.sleela` (the
GUI `.sleela` impl now lives in `lib/gui/version-mual/`, with an identical copy in
`lib/gui/version-gual/`; `lib/gui/` keeps these docs).

## Versioned lines: `version-mual` (1.0) and `version-gual` (2.0)

The GUI `.sleela` impl is carried in two parallel lines under `lib/gui/`:

- **`version-mual` — 1.0 (baseline).** The original 25-class surface. `SLPackage`
  reports `version() == "1.0"` / `featureLevel() == 1`.
- **`version-gual` — 2.0.** The 1.0 surface plus a hardened, settings-first
  posture. `SLPackage` reports `version() == "2.0"` / `featureLevel() == 2`, and
  the line adds:
  - **`SLGuiSettings`** — one validated, nameful settings object gathering the
    operating knobs: theme (`system`/`light`/`dark`), DPI scale, reduced-motion
    / accessibility, vsync, window min/max size + resizable/decorations, and the
    document-listener poll / debounce / coalesce-queue cap / backpressure policy.
    Every setter **clamps to a safe window**; `honored()` / `report()` state
    whether the configuration is internally consistent (the clamp-to-window
    discipline of the `lib/cpu` timing/voltage layer).
  - **Hardened `SLGuiBackend`** — `configureStrict()` refuses an unknown backend
    (fail-closed) and capability flags (`supportsHiDpi/Themes/Vsync/Accessibility/NativeMenus`).
  - **Hardened `SLGuiWindow`** — min/max size, `resizable`/`decorated`, a clamped
    `resize()`, and `applySettings(SLGuiSettings)`.
  - **Hardened `SLGuiDocumentListener`** — `configure()` clamps the attached
    count into the 1..14 contract, plus debounce, a coalesce-queue cap, a
    backpressure policy (coalesce/drop-oldest/block), and `applySettings(...)`.

- **`version-thual` — 3.0.** The 2.0 surface plus a clean, professional
  **visual-effects** layer. `SLPackage` reports `version() == "3.0"` /
  `featureLevel() == 3` (`isThreePointZero()`), and the line adds:
  - **`SLGuiEffects`** — a per-component effect bundle attached to an `SLGuiNode`:
    lighting + **glow**, **3D shine** highlight, drop **shadow**, a **metal**
    look-and-feel, **rotation** (fixed or scheduled), **fade** in/out, and an
    **activation audio** cue. Every knob is clamped; `toStyle()` flattens it into
    the node's style for the `SLGuiMount` reconciler; `honored()` checks ranges.
  - **`SLGuiRotationSchedule` (a "schene")** — scheduled rotation on a cadence.
    Its second role is a **cryptographic rotation cadence for an internal
    memorandum**: the same tick that turns the visible angle advances a key
    `epoch()`, so visible rotation is the honest external signal of an internal
    key rotation. Carries cadence/accounting only (no cipher).
  - **`SLGuiBranding`** — logo + brand identity (asset, placement, scale,
    wordmark, accent/secondary colors, tagline) the host paints into the chrome.
  - **`SLGuiAudioCues`** — the activation-audio registry (named cues → host
    assets, master enable/mute/gain) a component's effect references by name.
  - `SLGuiNode` gains `effects()` / `applyEffects()` / `tickEffects()`, and
    `SLGuiApp` gains `brand()`, `audioCues()`, and an `advanceEffects()` tick in
    its loop that drives scheduled rotations (and their memo key epochs) + fades.
    A runnable walkthrough is `version-thual/gui-effects-demo.sleela`.

- **`version-dual` — 4.0.** The 3.0 surface plus a rich, professional
  **component set** and a substantially upgraded aesthetic. `SLPackage` reports
  `version() == "4.0"` / `featureLevel() == 4` (`isFourPointZero()`), and the
  line adds:
  - **`SLGuiAudioRecorder`** — a standard audio-input record control (idle →
    armed → recording ↔ paused → stopped, level/duration/clip, device + format).
  - **`SLGuiVideoCanvas`** — a movie surface with full transport (play/pause/
    stop/seek, position+duration, volume/mute, loop, rate, fullscreen, buffer).
  - **`SLGuiUrlBar`** — an HTML-aware, HTTP-driven URL navigation bar (go/back/
    forward/reload/stop, bounded history, HTTP status/content-type, HTML-aware).
  - **`SLGuiClickObject`** — a custom click object: a small circle with
    architecturally-driven motion (static/drift/orbit/pulse) and a staged
    "System Apprentice" timer that escalates to a bound **final call**.
  - **`SLGuiLayout`** — exact positioning (absolute x/y/w/h) + layout modes
    (flow/grid/anchor/dock) with alignment, padding, gap, anchors.
  - **`SLGuiGraphics`** — a graphics facility (bounded draw-command list) **and**
    its control layer (quality, antialiasing, layer, dirty region).
  - **`SLGuiAdminAuthority`** — admin **sign-in** to start a process, **sign-off**
    on a component operation, and a **signature for Law**, with a fail-closed,
    audited authority trail.
  - **`SLGuiHtmlRenderer`** — a quick-responsive HTML renderer: a parse → style
    (CSS) → layout → script (JavaScript) → paint pipeline with a configurable
    time-to-first-paint budget (`quickResponse` / `quickPaintMs`), incremental
    parsing, and a JS timeout. It is **codec-aware**: `<video>`/`<audio>`/
    `<source>` elements are gated through `SLGuiCodecs` so the renderer never
    offers media the system cannot play.
  - **`SLGuiCodecs`** — the GUI-facing mirror of the [`/codecs`](../../codecs)
    registry (all 20 audio codecs in `sleela_codecs.h` order, with each codec's
    capability state — native/backend/recognized/container/event — MIME, and
    extensions). GUI media components (`SLGuiVideoCanvas`, `SLGuiAudioRecorder`,
    `SLGuiHtmlRenderer`) consult it with `playable()` / `recordable()` /
    `canPlayType()` / `playableExtension()` before offering a control, so the UI
    can **see what the system supports**.
  - **`SLGuiSettings`** gains an aesthetic surface (preset flat/soft/glass/metal/
    neon, corner radius, elevation, density, type scale, accent/surface, high
    contrast); `SLGuiWidgetFactory` registers the new component tags (incl.
    `html`/`webview`); `SLGuiApp` exposes `codecRegistry()` / `htmlRenderer()`
    and `canPlayCodec()` / `canPlayExtension()`.
  Runnable walkthroughs: `version-dual/gui-components-demo.sleela` and
  `version-dual/gui-html-codecs-demo.sleela`.

- **`version-military` — 5.0.** The 4.0 surface hardened for a neutral,
  professional military build. `SLPackage` reports `version() == "5.0-military"`
  / `featureLevel() == 5` (`isMilitary()`), and the line adds:
  - **Metal/blue aesthetic** — `SLGuiSettings` gains `AESTHETIC_MILITARY` with a
    metal finish (neutral / metal-silver / metal-blue), a blue-hue strength, a
    **3D-lit base frame** (bevel + key-light angle), and careful **quality
    component shadows** (shadow quality % + softness).
  - **Explicit Component vs Container taxonomy** — `SLGuiKind` classifies every
    tag as a container or a component, and says which are clickable and
    icon-eligible.
  - **Icons** — `SLGuiIcon`: matched size tiers, leading/trailing/top/icon-only
    placement, gap, tint, and an optical-centering nudge; `SLGuiApp.setIcon()`
    attaches one only to icon-eligible nodes (buttons, menu items, clickables).
  - **`SLGuiCryptoSelector`** — choose the program's cryptography suite from a
    careful catalog (classical + post-quantum) with a required minimum strength;
    fail-closed (unavailable / below-minimum suites can't be selected).
  - **`SLGuiCertificateStore`** — an X.509-style public-key store: import,
    trust/untrust, revoke/expire, select; a cert is valid only when explicitly
    trusted (fail-closed).
  - **`SLGuiWindowState`** — window key-listeners: resize, minimize, fullscreen,
    maximize, and **fold to a desktop side** ("minimize right"), remembering the
    previous norm so a **double-click restores** it exactly.
  - **`SLGuiScreenFit`** — sizes a window from the **actual screen size /
    resolution** at three support tiers: **basic** (a clamped fraction of the
    screen), **moderate** (fraction of the work area, with a margin and a
    centered position), and **advanced** (resolution-CLASS buckets — ≤720p /
    1080p / 1440p / 4K+ — each with its own target fraction and cap). The host
    feeds the real screen via `setScreen(w,h)` from the new `uiScreenWidth(app)`
    / `uiScreenHeight(app)` built-ins, and reads `width()/height()/x()/y()`
    back; an unknown (headless) screen falls back to a modeled 1920×1080.
  - **`SLGuiScrollbar`** — an **implied scrollbar** for a scrolled surface in
    **both axes** (vertical and horizontal). Given a viewport and content extent
    it decides whether scrolling is implied (`overflow()`) under an auto /
    always / never policy, and computes the normalized `value` (thumb position)
    and `page` (thumb size) the drawn `uiScrollBar(parent, orient, value, page)`
    needs; the host paints the bar and feeds gestures back via
    `scrollBy`/`scrollTo`.
  - Strong facile branding is on by default; `SLGuiWidgetFactory` registers the
    new tags (`cryptoselector`, `certstore`, `menuitem`, `menu`, `toolbar`,
    `panel`, `grid`, `icon`, `scrollbar`/`vscroll`, `hscroll`). Runnable
    walkthroughs are `version-military/gui-military-demo.sleela` and
    `version-military/gui-scroll-screenfit-demo.sleela`; a visual preview is in
    `version-military/preview/`.

The lines are source-compatible: each higher line keeps every entry point of the
one below and adds its surface on top (1.0 → +settings → +visual effects →
+professional components → +military hardening).

### Scrolling + screen-aware sizing (all lines)

Two line-agnostic helpers ship in **every** version line (1.0 → 5.0):

- **`SLGuiScrollbar`** — an **implied scrollbar** for a scrolled surface in both
  axes (vertical + horizontal). From a viewport vs. content extent it decides
  whether scrolling is implied (`overflow()`) under an auto / always / never
  policy and computes the normalized `value` (thumb position) + `page` (thumb
  size) the drawn `uiScrollBar(parent, orient, value, page)` needs; it tracks a
  clamped scroll offset (`scrollBy`/`scrollTo`/…).
- **`SLGuiScreenFit`** — sizes a window from the **actual screen size /
  resolution** at three tiers: **basic** (clamped fraction of the screen),
  **moderate** (fraction of the work area, margin + centered), and **advanced**
  (resolution-CLASS buckets ≤720p / 1080p / 1440p / 4K+, each with its own
  fraction and cap).

`SLGuiApp` wires both in: `boot()` primes a `SLGuiScreenFit` from the live
display and feeds the desktop size to `SLGuiWindowState` (military); `bootFitted(appId,
title, tier)` opens the app, queries the real screen, and sizes the window to it;
`screenFit()` and `scrollbarFor(viewport, content, horizontal)` expose the two
helpers. The real screen comes from the new `uiScreenWidth(app)` /
`uiScreenHeight(app)` built-ins (C ABI `slui_screen_size`), surfaced on the
shared `SLUserInterface` facade as `screenWidth()` / `screenHeight()`; a headless
host reports 0 and `SLGuiScreenFit` falls back to a modeled 1920×1080.
`SLGuiWidgetFactory` registers the `scrollbar`/`vscroll` (vertical) and `hscroll`
(horizontal) tags. A runnable walkthrough is each line's
`gui-scroll-screenfit-demo.sleela`.

### Automatic scrollbars on overflow — the `ScrolledView` (all lines)

`SLGuiScrollbar` is the *geometry brain*; the **native `slui::ScrolledView`**
(in `user-interface/`) is the *muscle* that makes scrolling automatic. It is a
real container widget that **measures its content, clips it to the viewport, and
reveals a scrollbar on its own when the content overflows** — so a component
dimensionally larger than its frame scrolls instead of spilling off-screen.

- **Native widget (`user-interface/src/slui_widget.{hpp,cpp}`).** `ScrolledView`
  treats its first child as the scroll content and owns a vertical + horizontal
  `ScrollBar`. On each `arrange()` it compares the content's natural size with
  the viewport and, per a per-axis policy — **auto** (bar only on overflow),
  **always** (reserve the gutter), **never** (clip, no bar) — shows or hides each
  bar, reserving a gutter and re-checking the other axis. `paint()` pushes a clip
  rect (the Canvas clip stack) so the content never draws past the frame, and
  arranges the content at a clamped negative offset. Dragging a bar drives the
  offset (and `scroll_by`/`scroll_to` do it programmatically). Validated by the
  toolkit smoke test (`user-interface/tests/smoke.c`).
- **C ABI + built-in.** `slui_scrolled_view(parent, policy_h, policy_v)` plus
  `slui_scrolled_view_scroll_to/by/x/y`, surfaced to SLeeLa as the
  `uiScrolledView` / `uiScrolledScrollTo` / `uiScrolledScrollBy` /
  `uiScrolledScrollX` / `uiScrolledScrollY` built-ins
  (`impl/core/sleela_ui_ids.h`, `sleela_ui_vm.c`, `impl/frontend/compiler.cpp`).
  A new `slui_window_render()` forces one layout+paint pass (used by headless
  tests to settle geometry).
- **SLeeLa surface (every line).** `SLGuiWidgetFactory` registers the
  `scrolled` / `scrollview` / `scrollarea` tags (→ a native scrolled viewport,
  auto/auto) and an `overflowPolicy()` keyword map (`auto`/`scroll`/`always` vs.
  `hidden`/`visible`/`clip`/`none`). **`SLGuiMount` is overflow-aware**:
  `constructWithOverflow()` promotes **any** plain container carrying an
  `overflow` (or per-axis `overflow-x` / `overflow-y`) attribute to a native
  scrolled viewport — so "frame smaller than content ⇒ scrollbars appear" is a
  declarative, automatic behaviour, not a manual wiring step. `SLGuiApp` adds
  `addScrolled(parentId, id)` and `addOverflowBox(parentId, id, overflow)`.
  A runnable walkthrough is each line's `gui-overflow-scroll-demo.sleela`.

So the earlier "overflow never auto-scrolls" limitation is resolved: the native
viewport clips + scrolls + shows bars automatically, `SLGuiScrollbar` remains the
pure geometry model behind it, and the SLeeLa layer makes it available both as an
explicit element and as an `overflow` attribute on any container.

### Fade transitions + notification surfaces (all lines)

Four line-agnostic **fade / transition** surfaces ship in **every** version line
(1.0 → 5.0). They answer the three ways UI fading is named: the *component* that
fades (toast/snackbar, modal/overlay, skeleton/placeholder, transition wrapper),
the *action* of fading (an enter/exit transition — tween + easing + dissolve),
and the *CSS surface* it maps to (`opacity`, `transition`, `@keyframes`). Each is
a pure, integer-only model with `honored()` / `report()` and a `toStyle()` that
flattens into the descriptor the `SLGuiMount` reconciler copies onto the widget;
SLeeLa owns the curve, timing, and lifecycle, and the host paints the pixels at
the opacity the model reports.

- **`SLGuiTransition`** — the **fading action**: an `enter` / `exit` transition
  (intro/outro) that tweens `opacity` from one percent to another over a
  duration with an **easing** curve — `linear` / `ease-in` / `ease-out` /
  `ease-in-out` / `dissolve`. `advance(deltaMs)` steps the tween and
  `opacityPct()` / `opacityAt(ms)` read the eased value (integer math:
  ease-in ≈ p², ease-out ≈ 1−(1−p)², in-out/dissolve a symmetric quadratic). It
  is the reusable primitive the other three embed. `toStyle()` →
  `fade:<from>-><to>/<ms>ms/<easing>`.
- **`SLGuiToast`** — a **toast / snackbar**: a small self-dismissing
  notification that fades **in** at a screen corner, **dwells** a few seconds,
  then fades **out**. A lifecycle machine (`hidden → fading-in → shown →
  fading-out → hidden`) driven by `tick(deltaMs)`; severity + corner placement;
  `opacityPct()` is what the host paints.
- **`SLGuiModal`** — a **modal / overlay**: a dialog that fades in over a
  **darkened backdrop** (scrim) and fades out on close (`closed → opening → open
  → closing → closed`). The dialog and the scrim each fade through their own
  `SLGuiTransition` (the scrim only to its dim target, never fully opaque), with
  a backdrop-click / Escape dismiss policy. (`requestClose()` is named so because
  bare `close` is a reserved SLeeLa built-in.)
- **`SLGuiSkeleton`** — a **skeleton screen / placeholder**: a subtle panel that
  fades in to stand in for loading content, runs a looping **shimmer** pulse, and
  then **dissolves** to reveal the real content (`idle → loading → revealing →
  revealed`). Reports placeholder vs. content opacity and the shimmer phase.

`SLGuiApp` exposes them as `transition()`, `toast(message)`, `modal(title)`, and
`skeleton(shape, lines)`. `SLGuiWidgetFactory` registers the `toast`/`snackbar`,
`modal`/`dialog`/`overlay`, `skeleton`/`placeholder`, and `transition`/`fade`
container tags. A runnable walkthrough is each line's
`gui-transitions-demo.sleela`. (In the 3.0+ lines these complement the per-node
`SLGuiEffects` fade knob: `SLGuiEffects` sets a static opacity/fade target on a
node, while `SLGuiTransition` et al. are full self-contained enter/exit tweens
with a lifecycle.)

## Events, DHTML-style DOM, and scripting

On top of the integration layer, `lib/gui` provides the backing API for
dynamic interfaces — action callbacks, a browser-DOM-style document, and a
scripting engine. SLeeLa owns the meaning of events and the document; the host
raises activations and mirrors the DOM onto SleelaUI widgets.

| Class | Role |
|---|---|
| `SLGuiEventType` | The event catalogue (click/input/change/keydown/…). |
| `SLGuiEvent` | A dispatched event with `preventDefault`/`stopPropagation`. |
| `SLGuiHandler` | A bound callback `(type, target, operation)`, take-the-latch. |
| `SLGuiEventBus` | The dispatcher: `addEventListener`, phased `dispatch`. |
| `SLGuiNode` | A DOM element: attributes, inline style, children, visibility. |
| `SLGuiDom` | The document: `getElementById`, `createElement`, `onClick`, … |
| `SLGuiScriptCommand` | One script instruction (verb + operands). |
| `SLGuiScript` | The command-list engine that drives the DOM/events. |

The full contract and a runnable walkthrough (`lib/gui/version-mual/gui-events-demo.sleela`)
are in [`EVENTS-DOM-SCRIPTING.md`](EVENTS-DOM-SCRIPTING.md).

## The three integration paths

```
Path 1  Java host    -> SLeeLa logic   (SLGuiHost)
Path 2  SLeeLa intent -> Java GUI       (SLGuiRuntime)
Path 3  shared native ABI bridge        (SLGuiBridge)
```

They are complementary, not mutually exclusive; the preferred long-term setup
enables all three (`SLGuiIntegration.recommended()`).

## Document-change listener (1..14, OS-driven)

Any path can enable the document-change listener option: watch **1 to 14**
documents and refresh the running window whenever any changes **on an OS call**.
Detection is OS-driven (a `WatchService` over inotify / `ReadDirectoryChangesW` /
`kqueue`/FSEvents), not polling; an OS event that does not move a document's
revision fingerprint (`lastModifiedMillis:size`) is coalesced away. The 1..14
bound is a hard contract, enforced in `SLGuiDocumentListener.withinBound()` and
mirrored in the native ABI (`SLGUI_MIN_DOCUMENTS` / `SLGUI_MAX_DOCUMENTS`).

```
Watched documents (1..14)
        |
        v
OS notification (WatchService.poll)
        |
        v
revision moved?  --no--> coalesced (no refresh)
        | yes
        v
SLGuiWindow.refresh(id, revision)   (marshaled onto the toolkit thread)
```

## Building and verifying

- SLeeLa source: every `lib/gui/version-mual/*.sleela` (and its `lib/gui/version-gual/` copy)
  passes `impl/build/sleela check` at library parity (cross-file field types
  resolve through the library index at build/program time, as elsewhere in
  `/lib`). The demo runs: `impl/build/sleela run lib/gui/version-mual/gui-demo.sleela`.
- Native bridge: `cd gui/native && make clean all test` builds the Path 3 ABI
  and runs the round-trip test (`slgui_bridge_test.cpp`).
- CI: `.github/workflows/gui-ci.yml` builds/tests the native bridge and
  validates this source layer on every change under `gui/**`, `lib/gui/**`,
  `lib/gui/version-mual/**`, or `lib/gui/version-gual/**`.

**SLeeLa — MEARVK LLC — 2026**
