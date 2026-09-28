# SLeeLa GUI — Document-Change Listener Option

**Author:** Max Rupplin — MEARVK LLC — 2026
**BODI™** address: `Bodi.system("sleela.gui.documents")` · reference `urn:sleela:bodi:1`

The document-change listener is SLeeLa's option to **listen to document changes
over 1–14 documents** and **update the running GUI**, **refreshing on OS
call(s)**. It lives in the GUI boundary (Path 2 owns the intent; Path 1 lets a
Java host tie it to a SLeeLa operation; Path 3 exposes a native refresh hook).

## Contract at a glance

| Property | Value |
|---|---|
| Documents watched | **1 to 14** (inclusive); enforced at construction and `watch()` |
| Change source | Operating system — `java.nio.file.WatchService` |
| OS facility | inotify (Linux), `ReadDirectoryChangesW` (Windows), `kqueue`/FSEvents (macOS) |
| Detection | OS wake → re-read each document's revision → emit only real changes |
| Revision fingerprint | `lastModifiedMillis:size` (`absent` when the file is gone) |
| GUI update | `SleelaGui.refresh(id, revision)`, marshaled onto the toolkit thread |
| Explicit refresh | `refreshNow()` — force a re-check from an OS/lifecycle callback |
| Out-of-bounds | `IllegalArgumentException` (BODI™ `contain`) |

## Types

### `SleelaDocument`
An immutable snapshot of one watched document.

- `id()` — stable SLeeLa-facing name.
- `path()` — resolved, normalized host path.
- `revision()` — OS-derived fingerprint (`lastModified:size`, or `absent`).
- `snapshot(id, path)` / `reread()` — capture from current OS metadata.
- `changedFrom(other)` — same document, different revision.

### `DocumentListener implements AutoCloseable`
Watches 1–14 documents and reports changes.

- `new DocumentListener(Map<String,Path> idToPath, Consumer<Change> onChange)`
  — also a `(…, long pollMillis)` overload (default 500 ms wake cadence).
- `DocumentListener.documents(List<String> ids, List<Path> paths)` — ordered
  builder that enforces the 1–14 bound.
- `watch()` — register each document's parent directory with the OS
  `WatchService` and start the daemon watch thread.
- `refreshNow()` — re-evaluate all documents now; returns the number changed.
- `watchedIds()` / `count()` / `isRunning()`.
- `close()` — stop the watch thread and release the OS watch handle (idempotent).
- Constants `MIN_DOCUMENTS = 1`, `MAX_DOCUMENTS = 14`.
- `DocumentListener.Change` — `previous()`, `current()`, `id()`.

## Refreshes on OS call(s)

Detection is not polling. The watch thread blocks in `WatchService.poll(...)`,
which is an operating-system call that returns only when the OS reports activity
in a watched directory (or on a short timeout used as a debounce). On each wake
— an **OS call** — the listener re-reads the OS revision of every watched
document and emits a `Change` **only** when a revision actually moved; a watch
event that does not change a document's fingerprint is coalesced away and does
not disturb the running GUI.

A host that receives its own OS/lifecycle signal can call `refreshNow()` to force
the same re-evaluation on demand.

## Updating the running GUI

A confirmed change is delivered to `SleelaGui.refresh(id, revision)`. The
`SwingGui` and `FxGui` backends run label mutations on their toolkit thread
(Event Dispatch Thread / JavaFX Application Thread), so the live window updates
safely from the listener's thread. The default `refresh` publishes a status line;
a host may override it to re-render richer document state.

## Usage

### Path 2 — SLeeLa intent owns the GUI
```java
SleelaGuiRuntime rt = new SleelaGuiRuntime(SleelaGui.create("swing"));
rt.window("SLeeLa", 640, 400);

DocumentListener listener = rt.listen(DocumentListener.documents(
    List.of("ledger", "config", "policy"),                 // 1..14 ids
    List.of(Path.of("ledger.sst"),
            Path.of("config.sleela"),
            Path.of("policy.md"))));

// ... later, on an explicit OS/lifecycle event:
int changed = rt.refreshNow();

rt.close();  // stops the listener, then closes the window
```

### Path 1 — Java host, SLeeLa decides meaning
```java
SleelaGuiHost host = new SleelaGuiHost(gui, runtime);
host.show("SLeeLa", 640, 400, "onOpen");

// On each change, call the SLeeLa operation with (id, revision) and show its
// result; if it returns null, the GUI's default refresh(id, revision) is used.
host.listen(idToPath, "onDocumentChanged");
```

### Path 3 — native ABI refresh hook
`gui/native/sleela_gui_bridge.h` exposes the native counterpart:

```c
void slgui_bridge_on_document_change(SLGuiBridge *bridge, SLGuiRefreshFn refresh);
int  slgui_bridge_document_changed(SLGuiBridge *bridge,
                                   const char *document_id, const char *revision);
```

The document count attached through the bridge is bounded to
`SLGUI_MIN_DOCUMENTS..SLGUI_MAX_DOCUMENTS` (1..14) to match the Java listener.

## BODI™ boundary

The listener stays inside the mitigative circumference:

- Fewer than 1 or more than 14 documents is rejected (`contain`).
- An OS event that does not move a revision is coalesced (no GUI update).
- A faulty refresh callback never stops the listener; the document stays
  observable.

The single source of truth for both emitted forms (Java and the SLeeLa
Wrapper™) is the `document-listener` block in
[`../systems/xml/sleela-gui.system.xml`](../systems/xml/sleela-gui.system.xml).
