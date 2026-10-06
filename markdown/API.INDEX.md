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

## Common, Included — Responsibility Classes
The foundational API contains 75 responsibility classes across three sets: 50 Common, Included classes plus 25 conversational classes.

### Set 1 — Fundamental
Resource, Identifier, Name, Version, Status, Error, Result, Option, Configuration, Parameter, TypeDescriptor, EnumDescriptor, Schema, Validator, Serializer, Deserializer, Buffer, ByteStream, Input, Output, Clock, Timer, Mutex, Condition, Thread.

### Set 2 — Platform / Runtime
Process, Environment, File, Directory, Path, Task, Scheduler, Event, EventBus, Future, CancellationToken, NetworkEndpoint, Transport, TcpTransport, UdpTransport, DnsResolver, SecureChannel, Service, Router, Listener, Server, Client, Session, Connection, Handler.

### Set 3 — Conversation
Participant, Identity, Role, Message, MessageId, MessagePart, Content, Attachment, Conversation, ConversationId, Turn, Transcript, Context, ContextItem, ContextWindow, Topic, Reference, Command, CommandArgument, CommandResult, Intent, Response, ResponsePart, Reply, ResponseStatus.

## Extensibility and Source Routing
A generic hierarchy/chain mechanism routes explicit system context toward a source implementation. The core contract is Scope → Area → Chain → Node → Decision → Target → Source Binding → Implementation.

See api/EXTENSIBILITY_API.md and SYSTEM.PRINCIPLES.md.

### Current extensibility responsibilities
RouteTarget, RouteNode, RouteContext, RouteDecision, RouteChain, SourceBinding, SourceResolver, ExtensionPoint, Extension, ExtensionDescriptor, ExtensionRegistry, HierarchyRouter, SourceRouter.

### Next architectural sets
Network and Program/Process are intentionally sibling expansion areas. Cross-cutting extension areas include configuration, capabilities, permissions, observability, persistence, plugins, scripting, generated code, diagnostics, testing and platform adapters.

**Reference rule:** subsystem documents and source headers remain authoritative for exact signatures and wire/ABI details.

**Max Rupplin — MEARVK LLC — 2026**

## Language Annotations

Document annotations are first-class SLeeLa language metadata and travel through Lexer → Parser → AST → Semantic Analysis → Compiler → Runtime → Server Edition. See impl/frontend/ANNOTATION_PIPELINE.md and server-edition/ANNOTATION_LANGUAGE.md.

## Document Annotations
Document-level annotations connect architecture documents with extensibility, source ownership, production traceability, and operational measurement without embedding executable routing logic.

Defined annotations: @scope, @area, @next, @responsibility, @provider, @source, @requires, @capability, @group, @counter, @stage, @release, @runbook.

See ANNOTATION.md and api/ANNOTATION_API.md.

Max Rupplin — MEARVK LLC — 2026
