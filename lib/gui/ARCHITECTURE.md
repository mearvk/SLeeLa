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

A runnable, self-contained walkthrough is `gui-demo.sleela`.

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

- SLeeLa source: every `lib/gui/*.sleela` passes `impl/build/sleela check` at
  library parity (cross-file field types resolve through the library index at
  build/program time, as elsewhere in `/lib`). The demo runs:
  `impl/build/sleela run lib/gui/gui-demo.sleela`.
- Native bridge: `cd gui/native && make clean all test` builds the Path 3 ABI
  and runs the round-trip test (`slgui_bridge_test.cpp`).
- CI: `.github/workflows/gui-ci.yml` builds/tests the native bridge and
  validates this source layer on every change under `gui/**` or `lib/gui/**`.

**SLeeLa — MEARVK LLC — 2026**
