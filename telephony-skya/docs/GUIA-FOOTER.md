# Skya Guia™ Protocol Footer

The Skya JavaFX client and administrative monitor include a scrolling footer status bar containing 16 numbered protocol-path messages.

The footer represents the documented **Guia™ — SLeeLa GUI Protocol™** path between the JavaFX GUI, Skya client/listener/session objects, SLeeLa circuits, commands, data, and monitoring surfaces.

## Messages 1–16

1. GUI creation
2. Client creation
3. Session creation
4. Client connection
5. Listener startup
6. Session opening
7. Authentication path
8. Circuit loading
9. Circuit startup
10. Event emission
11. Command invocation
12. Data updates
13. Listener reception
14. Listener acknowledgement
15. Monitor status
16. GUI control update

The footer is a visual protocol-status indicator. It does not independently prove that a production network transport, HTTP/2, HTTP/3/QUIC, media transport, or remote circuit has been successfully negotiated. Those layers remain responsible for their actual transport and runtime state.
