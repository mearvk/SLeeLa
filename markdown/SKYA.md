# Skya™ — SLeeLa Telephony

## Purpose

Skya™ is the telephony subsystem of SLeeLa. Its native implementation belongs to the SLeeLa C/C++ execution path and is organized under [`telephony-skya/`](telephony-skya/).

Skya is designed around three operating roles:

- **Server** — hosts a room/session endpoint.
- **Client** — joins a room/session endpoint.
- **Both** — runs the combined role for development and controlled deployments.

The current implementation establishes the runtime, configuration, room/peer foundation, and integration contracts. It does **not** yet claim to be a finished Skype-class media and transport stack.

## Main SLeeLa integration

The native path is:

```text
SLeeLa CLI
   |
   +-- sleela skya ...
   |
   v
impl/frontend/driver.cpp
   |
   v
telephony-skya/native/skya_sleela_bridge.*
   |
   +-- SLeeLa VM (impl/core)
   +-- Skya policy
   +-- HTTP capability model
   +-- SLeeLa NAT-aware configuration
   |
   v
telephony-skya/native/skya_engine.*
```

The bridge keeps Skya on the same SLeeLa runtime instead of creating a second language/runtime environment.

Supported entry points:

```sh
./impl/build/sleela skya plan
./impl/build/sleela skya --server --room lobby
./impl/build/sleela skya --client --room lobby
./impl/build/sleela skya --both --room lobby
```

The focused native sources remain available under [`telephony-skya/native/`](telephony-skya/native/), while the integrated build is defined by [`impl/Makefile`](impl/Makefile).

## Initial configuration policy

Skya has three initial configuration levels:

| Level | Initial behavior |
|---|---|
| **Basic** | HTTP/3 preference, TLS required, relay-capable/outbound NAT posture, port 8443, firewall policy `auto`. |
| **Intermediate** | Preserves explicit SLeeLa NAT selections while retaining safe defaults for unspecified values. |
| **Advanced** | Allows explicit HTTP version and explicit firewall policy; `always` fails closed when no supported platform firewall controller is available. |

Environment controls include:

```text
SLEELA_SKYA_CONFIG_LEVEL=basic|intermediate|advanced
SLEELA_SKYA_FIREWALL=never|auto|always
SLEELA_SKYA_HTTP=2|3
SLEELA_NAT_MODE=direct|port-forward|ipv6|outbound|relay
SLEELA_NAT_BIND_PORT=<port>
```

The policy layer reports its decision and platform capabilities. It does not silently rewrite firewall rules.

See [`telephony-skya/docs/NAT-FIREWALL-CONFIG.md`](telephony-skya/docs/NAT-FIREWALL-CONFIG.md) for the configuration contract.

## NAT and firewall architecture

Skya reuses SLeeLa's existing NAT-aware subsystem rather than maintaining a second NAT vocabulary. The established SLeeLa NAT modes are:

- direct;
- port-forwarded;
- IPv6;
- outbound-only;
- relay.

The existing platform port-awareness subsystem remains responsible for OS-specific firewall lifecycle. Skya therefore has one configuration vocabulary and one platform firewall authority.

The intended lifecycle is:

```text
Skya starts
   -> determine NAT mode
   -> determine HTTP transport preference
   -> inspect firewall capability
   -> report/apply the authorized port policy
   -> run session
   -> close the authorized listener
   -> release/close the corresponding firewall exposure
```

The last lifecycle steps are an architectural target; the current policy implementation reports configuration and capability but does not yet implement the complete automatic open/close firewall transaction.

## Security boundary

The Skya configuration and native contracts account for:

- TLS certificate validation;
- RSA-2048 compatibility;
- ephemeral Diffie-Hellman;
- certificate-quality reporting;
- bounded peer/session state;
- integrity-checked and resumable file transfer;
- explicit NAT/relay configuration;
- firewall changes only through the authorized SLeeLa platform layer.

The presence of a configuration field or API contract does not mean that the complete transport or media implementation is already present. Native behavior must be verified before a feature is described as production-complete.

## HTTP transport status

Skya's configuration can select HTTP/2 or HTTP/3, and the repository already contains SLeeLa HTTP multiplexing and native HTTP server implementations.

The current Skya bridge is a configuration/runtime integration layer. The following transport work remains to make Skya a complete telephony transport:

1. HTTP/2 control/session transport wired to Skya peers.
2. HTTP/3/QUIC control/session transport wired to Skya peers.
3. Connection/session state machines with reconnect and failure handling.
4. NAT traversal and relay data paths rather than policy-only mode selection.

Until those adapters are implemented and tested, HTTP version selection must be treated as a capability/configuration decision, not as proof of a completed media transport.

## Media and file-transfer status

The planned media surface includes:

- microphone capture/playback;
- camera capture/display;
- Opus, PCMU, PCMA, G.722, AAC and other supported adapter codecs;
- VP8, VP9, H.264/AVC, AV1, HEVC/H.265, Theora and other supported adapter codecs;
- synchronized audio/video sessions;
- resumable file transfer with integrity verification.

Codec names describe adapter targets. They do not mean every codec library is bundled with SLeeLa.

These media paths remain implementation work beyond the current native room/peer foundation.

## BODI and JavaFX

The declarative UI is [`telephony-skya/bodi/skya-ui.xml`](telephony-skya/bodi/skya-ui.xml).

The presentation path is:

```text
BODI XML
   -> SLeeLa application model
   -> Skya native engine / bridge
   -> JavaFX renderer
```

The current JavaFX loader implements the supported Skya UI subset. It is not presented as a general-purpose BODI interpreter.

See:

- [`telephony-skya/docs/SLEELA-APPLICATION-MODEL.md`](telephony-skya/docs/SLEELA-APPLICATION-MODEL.md)
- [`telephony-skya/docs/RUNNABLES.md`](telephony-skya/docs/RUNNABLES.md)
- [`telephony-skya/docs/BUILD-AND-RUN.md`](telephony-skya/docs/BUILD-AND-RUN.md)

## .sleela runnables

The runnable application documents are:

- [`telephony-skya/sleela/Skya.sleela`](telephony-skya/sleela/Skya.sleela)
- [`telephony-skya/sleela/SkyaServer.sleela`](telephony-skya/sleela/SkyaServer.sleela)
- [`telephony-skya/sleela/SkyaClient.sleela`](telephony-skya/sleela/SkyaClient.sleela)
- [`telephony-skya/sleela/SkyaRoom.sleela`](telephony-skya/sleela/SkyaRoom.sleela)

These are SLeeLa-native runnable/integration documents. They exercise the SLeeLa application/socket/thread model; they do not claim to replace the native media, HTTP/2, HTTP/3, or NAT transport layers.

## Build

Main integrated build:

```sh
cd impl
make
./build/sleela skya plan
```

Focused native build and policy inspection:

```sh
make -C telephony-skya/native
./telephony-skya/native/skya plan
```

Before treating the focused target as independently link-complete, verify that the required SLeeLa core, HTTP, and NAT implementation objects are available to that Makefile. The authoritative integrated target is the main SLeeLa build.

More build guidance is in [`BUILD.md`](BUILD.md) and [`telephony-skya/docs/BUILD-AND-RUN.md`](telephony-skya/docs/BUILD-AND-RUN.md).

## Current completion boundary

### Implemented foundation

- C/C++ Skya engine;
- SLeeLa VM binding;
- main `sleela skya` command;
- server/client/both role model;
- room and peer state foundation;
- Basic/Intermediate/Advanced configuration policy;
- NAT mode awareness;
- firewall capability detection;
- reuse of SLeeLa port-awareness architecture;
- BODI UI definition;
- JavaFX supported-subset loader;
- .sleela runnable documents;
- build and configuration documentation.

### Not yet production-complete

- live HTTP/2 session transport;
- live HTTP/3/QUIC session transport;
- real NAT traversal and relay data path;
- audio/video device integration;
- codec runtime adapters and negotiation;
- certificate-quality extraction from live connections;
- resumable file transfer;
- automatic firewall open/close lifecycle;
- complete multi-peer call/session orchestration.

Skya should be described as **integrated foundation / active implementation**, not as a finished telephony service, until those remaining layers have source implementation and verification.

## Documentation map

| Document | Purpose |
|---|---|
| [`SKYA.md`](SKYA.md) | System-level Skya architecture, integration boundary, configuration, and completion status. |
| [`telephony-skya/README.md`](telephony-skya/README.md) | Project directory overview. |
| [`telephony-skya/docs/ARCHITECTURE.md`](telephony-skya/docs/ARCHITECTURE.md) | Detailed architecture. |
| [`telephony-skya/docs/NAT-FIREWALL-CONFIG.md`](telephony-skya/docs/NAT-FIREWALL-CONFIG.md) | NAT/firewall configuration levels and policy. |
| [`telephony-skya/docs/BUILD-AND-RUN.md`](telephony-skya/docs/BUILD-AND-RUN.md) | Build and execution instructions. |
| [`telephony-skya/docs/RUNNABLES.md`](telephony-skya/docs/RUNNABLES.md) | .sleela runnable organization. |
| [`telephony-skya/docs/SLEELA-APPLICATION-MODEL.md`](telephony-skya/docs/SLEELA-APPLICATION-MODEL.md) | SLeeLa application-model boundary. |
| [`telephony-skya/native/SKYA-SLEEELA-ABI.md`](telephony-skya/native/SKYA-SLEEELA-ABI.md) | Native SLeeLa/Skya ABI contract. |
| [`telephony-skya/native/SKYA-CXX-INTEGRATION.md`](telephony-skya/native/SKYA-CXX-INTEGRATION.md) | C++ integration boundary. |

---

*Copyright (C) 2026 MEARVK LLC*
