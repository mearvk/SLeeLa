# SLeeLa GUI Library

A small desktop GUI bridge for SLeeLa implemented in Java, with parallel adapters for **Java Swing** and **JavaFX**.

## Design

The library keeps SLeeLa-facing concepts independent from the toolkit:

- `SleelaGui` — toolkit-neutral window, component, event, and lifecycle model.
- `SwingGui` — Swing implementation using `JFrame`, `JPanel`, `JButton`, and the Swing event dispatch thread.
- `FxGui` — JavaFX implementation using `Stage`, `Scene`, `VBox`, `Button`, and the JavaFX application thread.
- `SleelaGuiDemo` — simple Java entry point demonstrating the same SLeeLa UI through either backend.

The backend is selected at construction time. SLeeLa code can therefore describe a UI once while the host chooses Swing or JavaFX.

## Layout

```text
gui/
  README.md
  java/
    com/mearvk/sleela/gui/
      SleelaGui.java
      SwingGui.java
      FxGui.java
      SleelaGuiDemo.java
```

## Runtime model

The library deliberately does not require the SLeeLa VM to become a Swing or JavaFX implementation. It provides a narrow host boundary that can later be bound to SLeeLa dispatch, system pointers, events, and monitors.

Swing remains available as the conservative desktop backend; JavaFX provides the richer scene-graph backend.
