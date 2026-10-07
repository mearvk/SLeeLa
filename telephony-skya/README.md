<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-logo-004.jpg" alt="SLeeLa">






# Skya Telephony

Skya is the SLeeLa telephony application boundary for client, server, and combined operation.

## Layout

- native: C/C++ engine boundary and build files.
- sleela: runnable .sleela Wrapper programs for server, client, and combined room operation.
- docs: protocol, runtime, security, media, NAT, and runnable-program documentation.
- config: deployment defaults.

The native layer remains authoritative for networking, media, security, NAT, and file transfer. The .sleela programs are application-level runnables and orchestration examples; they do not create a second native runtime.

## Emblematic modules (loadable naturals)

Alongside the server/client/room runnables, Skya carries five **emblematic
modules** — self-describing SLeeLa classes the Skya program can load and compose
as *naturals* (load now, or load on demand). Each shares one loadable-module
contract (`load()` / `loaded()` / `moduleName()` / `describe()`) so a loader can
treat them uniformly, and the `SkyaModules` registry brings them up together.

| Module | File | Layer it emblematizes |
|---|---|---|
| **Socio** | [`sleela/Socio.sleela`](sleela/Socio.sleela) | the social fabric — members, standing, and ties above raw peers |
| **Network** | [`sleela/Network.sleela`](sleela/Network.sleela) | transport reachability — local/public address and careful, non-destructive NAT posture |
| **Servers** | [`sleela/Servers.sleela`](sleela/Servers.sleela) | server-side presence — the always-open comm listener and the roles it serves |
| **Communication** | [`sleela/Communication.sleela`](sleela/Communication.sleela) | message exchange — client-side reach/send/receive with a coherence tally |
| **RealAcquaintances** | [`sleela/RealAcquaintances.sleela`](sleela/RealAcquaintances.sleela) | the confirmed trust roster — acquaintances made real only after a verified exchange |
| *(loader)* | [`sleela/SkyaModules.sleela`](sleela/SkyaModules.sleela) | loads and composes the five modules as naturals |

These are descriptive application-level models composed over the same portable
VM socket primitives as `SkyaServer` / `SkyaClient`; they open no second native
runtime, and NAT/firewall lifecycle stays owned by the native port-awareness
subsystem.

## Command line — `sleela-skya`

`sleela-skya` is the Skya command-line program. It **loads/unloads** the
emblematic modules and **starts the default, ordered components** — the
`SleelaServer` (component 1) then the `SkyaGui` (component 2) — through the
existing Skya engine. Build it with `make -C telephony-skya/native sleela-skya`.

```sh
sleela-skya start                 # load all modules, then server + GUI (ordered)
sleela-skya start --no-gui        # server only
sleela-skya load Socio Network    # load specific modules
sleela-skya unload --all          # unload every module
sleela-skya list                  # modules + ordered components
sleela-skya gui                   # launch the Skya GUI (JavaFX studio)
```

The model lives in `native/sleela_skya_cli.{h,cpp}` (a `ModuleRegistry`, an
ordered `Component` set, and the `App` dispatch); it orchestrates the native
engine rather than creating a second runtime. See
[`docs/SLEELA-SKYA-CLI.md`](docs/SLEELA-SKYA-CLI.md).

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