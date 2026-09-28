# Skya Telephony

Skya is the SLeeLa telephony application boundary for client, server, and combined operation.

## Layout

- native: C/C++ engine boundary and build files.
- sleela: runnable .sleela Wrapper programs for server, client, and combined room operation.
- docs: protocol, runtime, security, media, NAT, and runnable-program documentation.
- config: deployment defaults.

The native layer remains authoritative for networking, media, security, NAT, and file transfer. The .sleela programs are application-level runnables and orchestration examples; they do not create a second native runtime.

## User Client

The default Skya GUI is the non-administrative user client, `SkyaClientApp`.

It provides the primary user-facing surface for:

- Chat
- Video
- Audio
- File Transfer
- connection and room selection

Administrative lifecycle and local circuit monitoring remain separate in `SkyaApp`, launched through the Client Monitor.

## Build

make -C telephony-skya/native
./telephony-skya/native/skya --both --http3 --room lobby

The C++ executable is the native bridge. The .sleela programs are compiled and run through the normal SLeeLa toolchain and use SLeeLa socket/thread primitives where a pure-SLeeLa runnable is appropriate.

## Security and media

Skya supports TLS certificate verification, RSA-2048 compatibility, ephemeral Diffie-Hellman, NAT/relay awareness, resumable file-transfer contracts, and codec negotiation. Codec names describe adapter capabilities; deployment must provide the corresponding libraries and comply with applicable licensing.

## Guia™ GUI Protocol

The Skya JavaFX client uses Guia™ 1.0 as its standard GUI-to-SLeeLa client/listener protocol, with BODI providing declarative UI definitions and Guia™ providing runtime lifecycle, events, commands, data, monitoring, and listener transitions.

See `docs/SLEELA_GUI_PROTOCOL.md`, `docs/GUIA_PROTOCOL_REFERENCE.md`, `docs/GUIA_OBJECTS.md`, and `docs/GUIA_TRANSITIONS.md` for the normative Guia™ references.
