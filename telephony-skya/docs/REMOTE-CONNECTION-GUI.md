# Skya Remote Connection GUI

The normal Skya Client and the administrative Client Monitor are separate JavaFX applications.

- **Normal Client:** user-facing Chat, Video, Audio, and File Transfer experience.
- **Client Monitor:** administrative local circuit/process monitoring with Start, Pause, and Stop controls.
- **Remote Connection GUI:** remote server connection foundation and connection/session controls.

The normal Client is the default user interface. Administrative controls are not part of the normal Client surface.

The controls and client/listener lifecycle follow the Guia™ vocabulary. The administrative local task corresponds to CIRCUIT.START, CIRCUIT.PAUSE, and CIRCUIT.STOP. The remote task corresponds to CLIENT.CONNECT, LISTENER.PAUSE, LISTENER.STOP, CLIENT.DISCONNECT, and SESSION.CLOSED.

The remote adapter remains a TCP foundation. Selecting HTTP/2 or HTTP/3 does not claim that a plain Java TCP socket implements those protocols. Production HTTP/2, HTTP/3/QUIC, TLS, NAT traversal, authentication, and media transport remain native implementation layers.

Guia™ remains the common GUI-to-client/listener contract; JavaFX is the presentation adapter.


### Initial connection timeout

The Remote Connection **Start** operation uses a five-second TCP connection timeout. While the attempt is pending, the status displays `Connecting to host:port (timeout 5s)`. A successful TCP connection changes the status to `Connected: host:port`; a connection that reaches the five-second socket timeout changes the status to `Connection timeout after 5s` and records `CLIENT.TIMEOUT after 5s` in the log. Immediate connection errors such as connection refusal are reported separately as `Connection failed`. A newer Start/Stop attempt invalidates an older pending attempt so a stale worker cannot overwrite the current status.
