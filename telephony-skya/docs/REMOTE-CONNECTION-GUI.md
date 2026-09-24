# Skya Remote Connection GUI

Both Skya JavaFX tasks now expose the same Admin UI lifecycle controls: Start, Pause, and Stop.

- Local Client Monitor: Start runs or resumes the SLeeLa client circuit, Pause pauses monitor output while the SLeeLa process remains running, and Stop terminates the local circuit process.
- Remote Connection: Start connects to the configured server, Pause pauses listener delivery while retaining the socket, and Stop closes the remote connection.

The controls follow the Guia™ lifecycle vocabulary. The local task corresponds to CIRCUIT.START, CIRCUIT.PAUSE, and CIRCUIT.STOP. The remote task corresponds to CLIENT.CONNECT, LISTENER.PAUSE, LISTENER.STOP, CLIENT.DISCONNECT, and SESSION.CLOSED.

The remote adapter remains a TCP foundation. Selecting HTTP/2 or HTTP/3 does not claim that a plain Java TCP socket implements those protocols. Production HTTP/2, HTTP/3/QUIC, TLS, NAT traversal, authentication, and media transport remain native implementation layers.

Guia™ remains the common GUI-to-client/listener contract; JavaFX is the presentation adapter.
