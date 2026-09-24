Guia™ 1.1 — SLeeLa GUI Protocol™

This reference defines the complete GUI-to-client/listener vocabulary: objects, functions, events, state machines, message envelopes, acknowledgements, data bindings, errors, and lifecycle transitions. The normative specification is maintained in `docs/SLEELA_GUI_PROTOCOL.md`.

## Primary contract

`BODI/XML → Guia™ → GUI Adapter → SLeeLa GUI Client → Guia™ Listener → SLeeLa Runtime/Circuit`

## Implementation rule

GUI implementations MUST treat Guia™ as the protocol boundary. JavaFX, native GUI systems, and future adapters implement Guia™; they do not redefine its semantics.

## Client/listener lifecycle

`GUI.CREATE → CLIENT.CREATE → LISTENER.CREATE → SESSION.CREATE → CLIENT.CONNECT → LISTENER.START → SESSION.OPEN → SESSION.AUTHENTICATE → CIRCUIT.LOAD → CIRCUIT.START → LISTENER.SUBSCRIBE → GUI.SHOW → APP.READY`

## Runtime path

`GUI event → EVENT.EMIT → COMMAND.REQUESTED → COMMAND.INVOKE → SLeeLa Client → Circuit/Service → LISTENER.RECEIVE → LISTENER.ACK → DATA.UPDATE/MONITOR.EVENT → CONTROL.UPDATE → GUI`

## Version

Guia™ 1.0 is the initial reference version.


## Visual vocabulary

Standard BODI/XML visual components are `bevel`, `gradient`, `style`, and `style-ref`. `bevel` provides 2D dimensional edges; `gradient` provides linear, radial, or bilinear surface transitions; `style` groups reusable declarations; `style-ref` applies them. The base BODI/XML document is authoritative for each GUI setup.