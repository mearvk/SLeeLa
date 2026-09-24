# Skya Server Socket Initialization

The native server now creates a real TCP listener during `skya_start(..., SKYA_SERVER)`.

Startup succeeds only after socket runtime initialization, socket creation, bind to `0.0.0.0:<port>`, and `listen()`. The default port is 8443.

The standalone `skya-server` remains alive after successful startup and stops on SIGINT/SIGTERM. A failed bind/listen returns non-zero and prints the socket failure instead of claiming readiness.

## Telnet / TCP smoke test

After startup, Telnet should receive:

`SKYA/1 server-ready port=8443 room=lobby http=3`

That confirms the TCP listener accepted the connection. It does not claim that this listener is itself an HTTP/3/QUIC implementation; those protocol layers remain separate.

## Client compatibility

Accepted sockets remain open for the Skya session, so the Java remote client can establish its initial TCP connection rather than connecting to a process that immediately exits.
