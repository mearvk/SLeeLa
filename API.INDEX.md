# SLeeLa API Index

## Core
VM, values, exchange ABI, opcodes, execution and results.

## Language
Source, lexer, parser, AST, compiler, versioning, diagnostics and semantic rules.

## Runtime
Memory, threads, locks, mailboxes, processes, time and OS abstraction.

## I/O
Streams, files, paths, directories, terminal and structured records.

## Networking
TCP, UDP, sockets, clients, listeners, routing, NAT-aware services and transport security interfaces.

## HTTP
HTTP 1.x, 2.0/2.1, 3.0/QUIC integration and experimental HTTP 4.0.

## Data
XML, BODI, JSON direction, binary framing, serialization and package manifests.

## Email
SMTP construction, messages, headers and transport.

## Database
Connections, queries, transactions and records.

## GUI
Windows/surfaces, controls, events and OS integration.

## Subjects
Math, Physics, Astrophysics, Chemistry, Economics, Finance, Inference/Statistics and Sociology.

## Telephony
Sessions, media, devices, drivers, buffers, queues and network integration.

## Native
Dynamic libraries, static archives, executables and binary-format inspection.

## Tooling
Nordshrift, XCLASS, Sigil, build/package tooling and future IDE/language-server services.

## Security
Integrity, permissions, resource limits, authenticated carriers and security diagnostics.

## Reference rule

This index points to subsystem documents. Individual headers, models and specifications remain authoritative for exact signatures and wire/ABI details.

**Max Rupplin — MEARVK LLC — 2026**

## Common, Included — Fundamental Responsibility Classes

The following 25 classes form the reusable fundamental responsibility layer. Each owns one coherent responsibility and is intended for reuse by higher-level SLeeLa subsystems.

### Identity and lifecycle
- **Resource** — lifecycle ownership and release.
- **Identifier** — stable identity.
- **Name** — validated symbolic naming.
- **Version** — version representation.
- **Status** — operation state.
- **Error** — structured failure.

### Values, configuration and contracts
- **Result** — success/failure value transport.
- **Option** — optional value transport.
- **Configuration** — typed runtime configuration.
- **Parameter** — named operation input.
- **TypeDescriptor** — runtime type metadata.
- **EnumDescriptor** — enumerated-value metadata.
- **Schema** — structural data definition.
- **Validator** — constraint validation.

### Data representation
- **Serializer** — object/data encoding.
- **Deserializer** — object/data decoding.
- **Buffer** — bounded byte storage.
- **ByteStream** — sequential byte transport.
- **Input** — input-source abstraction.
- **Output** — output-sink abstraction.

### Time and concurrency
- **Clock** — monotonic and wall-clock time source.
- **Timer** — deadline and timer responsibility.
- **Mutex** — mutual exclusion.
- **Condition** — condition synchronization.
- **Thread** — execution-thread lifecycle.

### Source location
Native contracts and implementations are maintained under `impl/fundamental/`. The detailed specification is maintained in `api/COMMON_INCLUDED_FUNDAMENTAL.md`.

### Java responsibility correspondence
These names describe responsibility correspondence, not identical implementation or ABI. Examples include `Thread` ↔ `java.lang.Thread`; `Clock`/`Timer` ↔ Java time and scheduling facilities; `Mutex`/`Condition` ↔ Java locking and condition facilities; `Buffer`/`ByteStream` ↔ Java byte-buffer and stream responsibilities; and `Serializer`/`Deserializer` ↔ Java data-encoding responsibilities.
