<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-logo-004.jpg" alt="SLeeLa">





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


## Core vocabulary
Holding Document → Forwarding Annotation → Nexter Colony.

Forwarding is never an implicit privilege grant and remains subject to security, capability, resource, release, and deployment controls.

Max Rupplin — MEARVK LLC — 2026


## HTTP generation support
The Server Edition is prepared to dispatch the SLeeLa HTTP generation family from 1.0 through 9.0 through explicit generation adapters. HTTP 1.0/1.1 remain Internet compatibility targets; HTTP 2.0–9.0 are SLeeLa project generations where applicable. Annotation forwarding remains uniform: Holding Document → Forwarding Annotation → Nexter Colony.


## First-class language annotation path

The Server Edition consumes annotation metadata produced by the language front end:

**Wrapper → Lexer → Parser → AST → Semantic Analysis → Compiler → Runtime → Server Edition**

The concrete bridge is server-edition/annotation_language_bridge.hpp/.cpp. It installs the same AnnotationRuntime used by the direct SLeeLa runtime, so the Server Edition does not maintain a second annotation parser or forwarding engine.

A single valid @next becomes the Nexter Colony candidate. Multiple @next declarations remain metadata but are rejected by the current runtime until an explicit multi-destination forwarding policy exists.

See server-edition/ANNOTATION_LANGUAGE.md.