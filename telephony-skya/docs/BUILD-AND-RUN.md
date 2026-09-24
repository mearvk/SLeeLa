# Skya Build and Run

## Native SLeeLa-integrated build

The Skya native bridge includes the SLeeLa core ABI and existing SLeeLa HTTP/NAT headers. It probes the real SLeeLa VM memory/handle boundary before starting the Skya engine.

From the repository root:
make -C telephony-skya/native
./telephony-skya/native/skya plan
./telephony-skya/native/skya --server --room lobby
./telephony-skya/native/skya --client --room lobby
./telephony-skya/native/skya --both --room lobby

## Main SLeeLa C/C++ integration

The bridge is compiled into the main impl build as skya_sleela_bridge.o and the main sleela executable exposes the skya command. This keeps Skya on the same C/C++ runtime instead of launching a parallel VM.

## Initial configuration levels

Basic is the safe first-run profile. Intermediate preserves explicit SLeeLa NAT settings. Advanced permits explicit HTTP and firewall policy. See NAT-FIREWALL-CONFIG.md.

## Runnables

SLeeLa source programs remain under telephony-skya/sleela/: SkyaServer.sleela, SkyaClient.sleela, Skya.sleela, and SkyaRoom.sleela.

## Current completeness

The Skya Server is not yet a complete telephony server. Lifecycle, room policy, SLeeLa runtime binding, and initial NAT/firewall decision logic are implemented. Full production HTTP/2/HTTP/3 wire service, media capture/codec negotiation, certificate-quality extraction, NAT traversal/relay transport, resumable file transfer, and OS firewall lifecycle still require their existing SLeeLa subsystems to be wired into Skya transport/session adapters.
