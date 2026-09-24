# SLeeLa GUI Protocol™

**Protocol name:** Guia™  
**Specification:** SLeeLa GUI Protocol™ 1.0  
**Owner:** MEARVK LLC

## 1. Guia™

**Guia™** is the protocol language for SLeeLa graphical interfaces. It defines a language-neutral contract for GUI lifecycle, controls, events, data bindings, commands, monitoring, and application circuits.

The trademark notation **Guia™** is used throughout SLeeLa documentation to identify the protocol name.

## 2. Architecture

BODI/XML → Guia™ — SLeeLa GUI Protocol™ → JavaFX/native/web adapters → SLeeLa Runtime/Circuit

BODI provides declarative interface description. Guia™ provides the runtime communication and behavioral contract.

## 3. Core vocabulary

| Operation | Meaning |
|---|---|
| GUI.CREATE | Create a GUI surface |
| GUI.SHOW | Show a GUI surface |
| GUI.HIDE | Hide a GUI surface |
| GUI.CLOSE | Close a GUI surface |
| WINDOW.CREATE | Create a window |
| WINDOW.RESIZE | Change window dimensions |
| CONTROL.CREATE | Create a control |
| CONTROL.UPDATE | Update control state/value |
| CONTROL.ENABLE | Enable a control |
| CONTROL.DISABLE | Disable a control |
| EVENT.BIND | Bind a GUI event to an action |
| EVENT.EMIT | Emit a GUI event |
| DATA.GET | Retrieve bound data |
| DATA.SET | Set bound data |
| DATA.SUBSCRIBE | Subscribe to data changes |
| COMMAND.INVOKE | Invoke a SLeeLa command or method |
| CIRCUIT.LOAD | Load a SLeeLa GUI circuit |
| CIRCUIT.START | Start a circuit |
| CIRCUIT.STOP | Stop a circuit |
| MONITOR.SUBSCRIBE | Subscribe to runtime monitoring |
| MONITOR.STATUS | Publish runtime status |
| MONITOR.LOG | Publish runtime log information |
| ERROR.REPORT | Report a protocol/runtime error |

## 4. Versioning

Guia™ implementations MUST identify their protocol version. The initial reference version is **Guia™ 1.0**. Incompatible changes MUST receive a new protocol version.

## 5. JavaFX

JavaFX is an implementation target, not the definition of Guia™. The JavaFX adapter translates Guia™ operations into JavaFX application, scene, control, event, and property operations.

## 6. BODI relationship

BODI documents may describe windows, controls, actions, monitoring sources, and circuit relationships. A BODI loader may translate those declarations into Guia™ operations.

## 7. Skya

The Skya client uses Guia™ as the protocol boundary for its JavaFX monitoring/control surface. The BODI document describes the interface; Guia™ defines communication with the SLeeLa circuit and runtime.

## 8. Security

GUI commands MUST respect the permissions of the underlying SLeeLa circuit and runtime. A GUI adapter MUST NOT silently elevate privileges or alter operating-system security policy.

## 9. Reference status

This document is the normative starting reference for Guia™ within SLeeLa.

— Max Rupplin - MEARVK LLC - 2026
