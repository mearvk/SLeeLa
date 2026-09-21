# SLeeLa GUI Library

A desktop GUI bridge for SLeeLa implemented in Java, with parallel adapters for **Java Swing** and **JavaFX**.

## Design

The library keeps SLeeLa-facing concepts independent from the toolkit:

- `SleelaGui` — toolkit-neutral window, component, event, and lifecycle model.
- `SwingGui` — Swing implementation using `JFrame`, `JPanel`, `JButton`, and the Swing event dispatch thread.
- `FxGui` — JavaFX implementation using `Stage`, `Scene`, `VBox`, `Button`, and the JavaFX application thread.
- `SleelaGuiDemo` — simple Java entry point demonstrating the same UI through either backend.
- `SleelaGuiHost` — connects a Java GUI to SLeeLa business operations.
- `SleelaGuiRuntime` — lets SLeeLa-facing code express GUI intent while Java supplies presentation.
- `ProcessSleelaRuntime` — runnable Java-hosted adapter that invokes the native SLeeLa executable.
- `JniSleelaRuntime` — Java facade for a direct native/JNI runtime binding.

## Three integration paths

### 1. Java host → SLeeLa business logic

`ProcessSleelaRuntime` allows Java to host the desktop application while SLeeLa remains authoritative for business logic. `SleelaGuiHost` connects the operation to Swing or JavaFX.

```text
Java GUI → SleelaGuiHost → ProcessSleelaRuntime → SLeeLa → C/C++ VM
```

This is the simplest path to deploy today.

### 2. SLeeLa intent → Java GUI

`SleelaGuiRuntime` exposes window, text, and named-action operations. A future compiler/runtime binding can map SLeeLa GUI methods directly onto this object.

```text
SLeeLa → SleelaGuiRuntime → Swing / JavaFX
```

This makes the `.sleela` Wrapper™ the natural place for application behavior and GUI intent.

### 3. Shared native ABI

`native/sleela_gui_bridge.h` and `.cpp` provide a C-compatible callback boundary. `JniSleelaRuntime` is the Java-side facade for connecting that boundary to the JVM.

```text
SLeeLa → C/C++ runtime → slgui_bridge → JNI → Swing / JavaFX
```

This is the preferred high-performance path when direct runtime integration is required.

## Recommended architecture

Use all three layers together when the application warrants it:

- **SLeeLa** owns business logic, state transitions, domain operations, and the meaning of events.
- **Java** owns Swing/JavaFX presentation and desktop lifecycle.
- **The bridge** converts events and values and provides the runtime boundary.
- **The native ABI** can bypass process boundaries when a direct VM connection is needed.

Java is therefore a native Java desktop host for SLeeLa, not a second business-logic implementation.

## Layout

```text
gui/
  README.md
  INTEGRATION.md
  java/
    com/mearvk/sleela/gui/
      SleelaGui.java
      SwingGui.java
      FxGui.java
      SleelaGuiDemo.java
      SleelaRuntime.java
      SleelaGuiHost.java
      SleelaGuiRuntime.java
      ProcessSleelaRuntime.java
      JniSleelaRuntime.java
      SleelaDocument.java       # one watched document (id, path, OS revision)
      DocumentListener.java     # the 1..14 document-change listener option
  native/
    sleela_gui_bridge.h
    sleela_gui_bridge.cpp
  DOCUMENT_LISTENER.md          # the document-change listener option
```

## Runtime model

The GUI library does not turn the SLeeLa VM into Swing or JavaFX. It establishes a controlled host boundary around the existing SLeeLa execution architecture. The SLeeLa core already exposes `slcore_exchange()` as its stable execution boundary, so the native bridge can be attached there rather than duplicating VM semantics.

Swing remains available as the conservative desktop backend; JavaFX provides the richer scene-graph backend.

## JavaFX lifecycle

`FxGui` expects the JavaFX toolkit to have been initialized by its Java host. A production launcher should initialize JavaFX before constructing `FxGui`; this keeps toolkit lifecycle concerns out of SLeeLa business logic.


## Document-change listener option

SLeeLa can **listen to document changes over 1–14 documents** and update the
running GUI when any of them changes. Change detection is driven by the
operating system (a `java.nio.file.WatchService` backed by inotify on Linux,
`ReadDirectoryChangesW` on Windows, and `kqueue`/FSEvents on macOS), so the GUI
**refreshes on OS call(s)** rather than by polling file contents.

- `SleelaDocument` — an immutable snapshot of one watched document: its stable
  `id`, resolved `path`, and an OS-derived `revision` fingerprint
  (`lastModified:size`).
- `DocumentListener` — watches **1..14** documents (the bound is enforced;
  fewer than 1 or more than 14 is rejected). A watch thread blocks in an OS call
  and wakes only on OS activity; each wake re-reads the affected documents and
  emits a coalesced change. `refreshNow()` forces an explicit refresh from an
  OS/lifecycle callback.
- The change flows to `SleelaGui.refresh(id, revision)`, which the Swing/JavaFX
  backends marshal onto their toolkit thread so the live window updates safely.

Enable it through either integration path:

```java
// Path 2 — SLeeLa intent owns the GUI; Java presents it.
SleelaGuiRuntime rt = new SleelaGuiRuntime(SleelaGui.create("swing"));
rt.window("SLeeLa", 640, 400);
rt.listen(DocumentListener.documents(
    List.of("ledger", "config"),
    List.of(Path.of("ledger.sst"), Path.of("config.sleela"))));   // 1..14 docs

// Path 1 — Java hosts; a SLeeLa operation decides what a change means.
SleelaGuiHost host = new SleelaGuiHost(gui, runtime);
host.show("SLeeLa", 640, 400, "onOpen");
host.listen(idToPath, "onDocumentChanged");   // calls SLeeLa on each change
```

See [`DOCUMENT_LISTENER.md`](DOCUMENT_LISTENER.md) for the full contract, and the
`document-listener` block in
[`../systems/xml/sleela-gui.system.xml`](../systems/xml/sleela-gui.system.xml)
for the BODI™ system definition both emitted forms share.
