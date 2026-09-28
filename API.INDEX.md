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

## Common, Included — Fundamental Responsibility Classes
The Common, Included layer now contains 50 reusable responsibility classes, organized as two 25-class sets.

### Set 1 — Fundamental responsibilities
Resource, Identifier, Name, Version, Status, Error, Result, Option, Configuration, Parameter, TypeDescriptor, EnumDescriptor, Schema, Validator, Serializer, Deserializer, Buffer, ByteStream, Input, Output, Clock, Timer, Mutex, Condition, Thread.

### Set 2 — Supported platform/runtime responsibilities
Process, Environment, File, Directory, Path, Task, Scheduler, Event, EventBus, Future, CancellationToken, NetworkEndpoint, Transport, TcpTransport, UdpTransport, DnsResolver, SecureChannel, Service, Router, Listener, Server, Client, Session, Connection, Handler.

Native contracts and implementations are maintained under impl/fundamental/. Set 1 is specified by api/COMMON_INCLUDED_FUNDAMENTAL.md; Set 2 is specified by api/COMMON_INCLUDED_NEXT_25.md.

**Max Rupplin — MEARVK LLC — 2026**


## Language Annotations

Document annotations are first-class SLeeLa language metadata and travel through Lexer → Parser → AST → Semantic Analysis → Compiler → Runtime → Server Edition. See impl/frontend/ANNOTATION_PIPELINE.md and server-edition/ANNOTATION_LANGUAGE.md.

## Document Annotations
Document-level annotations connect architecture documents with extensibility, source ownership, production traceability, and operational measurement without embedding executable routing logic.

Defined annotations: @scope, @area, @next, @responsibility, @provider, @source, @requires, @capability, @group, @counter, @stage, @release, @runbook.

See ANNOTATION.md and api/ANNOTATION_API.md.

Max Rupplin — MEARVK LLC — 2026
