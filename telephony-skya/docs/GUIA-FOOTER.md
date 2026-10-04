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

## The footer is a live protocol client

The footer is both the visual status bar **and** the GUI's Guia™ transport. When
a GUI control fires, it calls `SkyaProtocolFooter.send(command)` /
`sendAsync(command)`, which:

1. opens a short-lived TCP control connection to the SLeeLa Skya client
   (default `127.0.0.1:8700`, overridable via `SKYA_GUIA_HOST` /
   `SKYA_GUIA_PORT`);
2. writes the Guia command verb as `GUIA/1 <COMMAND>`;
3. reads the one-line `GUIA/1 <EVENT>` reply the SLeeLa client returns;
4. advances the scrolling bar to the matching step and notifies the GUI's event
   sinks so labels and lists update with the real result.

If the SLeeLa client is not running, `send` returns `GUIA/1 CLIENT.OFFLINE`
instead of throwing, so the GUI stays responsive. See
[`GUIA-PROTOCOL.md`](GUIA-PROTOCOL.md) for the full command/event vocabulary.

The footer still visualizes the 16-step Guia™ path. It does not independently
prove that a production media transport, HTTP/2, HTTP/3/QUIC, or remote circuit
has been negotiated end to end; those layers remain responsible for their actual
transport and runtime state. What it does prove, when an event other than
`CLIENT.OFFLINE` returns, is that the GUI reached the SLeeLa client and the
SLeeLa client ran the corresponding command (including the `SKYA/1` network leg
for `CLIENT.CONNECT` and `CHAT.SEND`).
