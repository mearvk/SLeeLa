# Skya SLeeLa Runnables

The files under telephony-skya/sleela are executable SLeeLa Wrapper programs. They are split by role so one source tree can exercise server, client, combined, and room operation.

SkyaServer.sleela: the SKYA/1 server. Loops accepting peers on :8443 and
  answers `client-hello` (→ `SKYA/1 server-ready …`) and chat frames
  (→ `SKYA/1 delivered`).
SkyaClient.sleela: the Guia control agent. Loops on :8700 accepting `GUIA/1`
  commands from the GUI, dispatches each verb, and for CLIENT.CONNECT / CHAT.SEND
  performs the real SKYA/1 call to the server before replying with a `GUIA/1`
  event. See GUIA-PROTOCOL.md.
Skya.sleela: one-process combined server/client smoke test.
SkyaRoom.sleela: bounded multi-peer room model.

These are SLeeLa-native runnable contracts. They are useful for toolchain and integration tests and do not claim that TCP alone implements production HTTP/2, HTTP/3, media, or NAT traversal.
