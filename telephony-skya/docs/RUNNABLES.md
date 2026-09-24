# Skya SLeeLa Runnables

The files under telephony-skya/sleela are executable SLeeLa Wrapper programs. They are split by role so one source tree can exercise server, client, combined, and room operation.

SkyaServer.sleela: server lifecycle and a bounded peer greeting.
SkyaClient.sleela: client lifecycle and a bounded greeting exchange.
Skya.sleela: one-process combined server/client smoke test.
SkyaRoom.sleela: bounded multi-peer room model.

These are SLeeLa-native runnable contracts. They are useful for toolchain and integration tests and do not claim that TCP alone implements production HTTP/2, HTTP/3, media, or NAT traversal.
