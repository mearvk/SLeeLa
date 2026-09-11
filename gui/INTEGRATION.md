# SLeeLa GUI Integration

The GUI library supports three complementary integration paths. They are not mutually exclusive: the first provides the easiest Java host model, the second makes GUI intent visible to SLeeLa, and the third supplies a native ABI for a tighter runtime boundary.

## 1. Java host → SLeeLa business logic

Java owns the process and the desktop lifecycle. `SleelaGuiHost` connects a Swing or JavaFX implementation to a `SleelaRuntime`.

```text
Java application
      |
      v
SleelaGuiHost
      |
      +---- Swing / JavaFX
      |
      v
SleelaRuntime
      |
      v
SLeeLa business logic
      |
      v
slcore_exchange()
```

Use this path when Java is the natural application launcher but SLeeLa must remain the business-logic authority.

## 2. SLeeLa intent → Java GUI

`SleelaGuiRuntime` models the inverse direction. A SLeeLa-facing dispatcher can request a window, update text, and bind named actions while Java supplies the actual desktop controls.

```text
SLeeLa program
      |
      v
GUI intent / named operation
      |
      v
SleelaGuiRuntime
      |
      +---- Swing
      |
      +---- JavaFX
```

Use this path when the `.sleela` Wrapper™ should describe the application's UI behavior and Java should remain a presentation/runtime substrate.

## 3. Shared native ABI

`native/sleela_gui_bridge.h` and `native/sleela_gui_bridge.cpp` provide a small C-compatible callback boundary. It can later be attached directly to the existing SLeeLa native runtime and exposed to Java through JNI or another Java/native mechanism.

```text
SLeeLa
  |
  v
C/C++ runtime
  |
  +---- slcore_exchange()
  |
  +---- slgui_bridge
             |
             v
       Java native bridge
          /        \
       Swing      JavaFX
```

Use this path when low-overhead native calls, shared values, or direct VM integration matter.

## Recommended combined architecture

The preferred long-term architecture is to use all three layers:

- **SLeeLa** owns business logic, state transitions, domain operations, and the meaning of events.
- **Java** owns Swing/JavaFX presentation and desktop lifecycle.
- **The native bridge** provides a stable low-level boundary to the C/C++ execution core.

The result is intentionally asymmetric: Java is not a second business-logic implementation. It is a native Java desktop host for SLeeLa.

## Value boundary

The production bridge should eventually define an explicit `SLValue` ↔ Java value mapping rather than relying on arbitrary `Object` values. The existing VM's tagged value model is the natural source for that contract.

## JavaFX lifecycle

`FxGui` currently assumes that the JavaFX toolkit has been initialized by its host. A production JavaFX launcher should extend `javafx.application.Application` (or otherwise initialize the toolkit) before constructing the GUI runtime. This keeps JavaFX lifecycle policy in the Java host rather than inside SLeeLa business logic.
