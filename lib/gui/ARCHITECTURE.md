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

The lines are source-compatible: each higher line keeps every entry point of the
one below and adds its surface on top (1.0 → +settings → +visual effects →
+professional components).

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
