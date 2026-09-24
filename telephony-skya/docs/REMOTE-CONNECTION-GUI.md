# Skya Remote Connection GUI

The normal Skya Client and the administrative Client Monitor are separate JavaFX applications.

- **Normal Client:** user-facing Chat, Video, Audio, and File Transfer experience.
- **Client Monitor:** administrative local circuit/process monitoring with Start, Pause, and Stop controls.
- **Remote Connection GUI:** remote server connection foundation and connection/session controls.

The normal Client is the default user interface. Administrative controls are not part of the normal Client surface.

The controls and client/listener lifecycle follow the Guia™ vocabulary. The administrative local task corresponds to CIRCUIT.START, CIRCUIT.PAUSE, and CIRCUIT.STOP. The remote task corresponds to CLIENT.CONNECT, LISTENER.PAUSE, LISTENER.STOP, CLIENT.DISCONNECT, and SESSION.CLOSED.

The remote adapter remains a TCP foundation. Selecting HTTP/2 or HTTP/3 does not claim that a plain Java TCP socket implements those protocols. Production HTTP/2, HTTP/3/QUIC, TLS, NAT traversal, authentication, and media transport remain native implementation layers.

Guia™ remains the common GUI-to-client/listener contract; JavaFX is the presentation adapter.
