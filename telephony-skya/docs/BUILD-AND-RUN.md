# Skya Build and Run

Native build:
make -C telephony-skya/native
./telephony-skya/native/skya --server --http3 --room lobby

The native command currently validates engine lifecycle and room selection. It is not yet a complete wire-level media server.

SLeeLa runnables:
- telephony-skya/sleela/SkyaServer.sleela
- telephony-skya/sleela/SkyaClient.sleela
- telephony-skya/sleela/Skya.sleela
- telephony-skya/sleela/SkyaRoom.sleela

For a two-process smoke test, start the server runnable first and then the client runnable. For a single-process test, use Skya.sleela.

HTTP/2 and HTTP/3 transport, TLS/certificate inspection, media capture/codecs, NAT traversal, resumable file transfer, and explicit OS firewall integration remain adapter work against existing SLeeLa subsystems.
