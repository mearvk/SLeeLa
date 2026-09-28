# SLeeLa Server Edition

SLeeLa Server Edition is the rugged network service for the SLeeLa HTTP 1.0 through HTTP 9.0 application packet family.

Rugged means bounded memory and frame sizes, timeouts, malformed-input rejection, and deterministic shutdown. Tough means strict framing, routing filters, rate controls, and structured audit logs. Clean means a small server core with a documented wire boundary and no payload execution.

## Supported traffic

The server accepts SLeeLa HTTP grades 1 through 9 through the Server Edition wire envelope. Generation-specific fields remain application metadata and follow PACKET.md.

HTTP 9.0 preserves the HTTP 8.0 metadata boundary and adds its International Data and Dark Band records.

## Build

Linux and macOS:

    make -C server-edition

Windows:

Compile server-edition/sleela_server.cpp and server-edition/server.cpp as C++17 and link Winsock2.

## Run

    ./server-edition/sleelas --bind 0.0.0.0 --port 19866

Default TCP listener: 19866.

Existing PORT-AWARENESS.md remains the host-firewall lifecycle contract. Firewall policy is deliberately separate from the socket engine.

## Processing

    accept
      |
      v
    fixed envelope
      |
      v
    header parser
      |
      v
    generation and routing policy
      |
      v
    filters and heuristics
      |
      +---- reject -> audit log + status
      |
      v
    admission boundary
      |
      v
    application service handler

The current implementation is an admission and transport server. Service handlers can be attached above it without weakening packet validation.

## Safety boundary

Payloads are bytes. The server does not execute programs, shell commands, scripts, XML procedures, or metadata.

POLICE, SECURITY, SAFETY, CLASSIFICATION, DARK-BAND, frequency, and related fields are application metadata. They do not grant authority, clearance, identity verification, network control, or access.

## Production integration

Before public deployment, add authenticated peer identity, TLS or another protected carrier, per-service authorization, persistent replay and sequence state, service-manager integration, metrics, and conformance vectors for every HTTP grade.
