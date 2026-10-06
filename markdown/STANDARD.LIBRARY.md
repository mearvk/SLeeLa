# SLeeLa Standard Library

## Objective

The standard library is the portable application-facing layer above the SLeeLa core and below domain-specific libraries.

## Core modules

### Data
Strings, Unicode, bytes, arrays, collections, maps, sets, records and serialization.

### Math
Arithmetic, trigonometry, numeric utilities, precision helpers and domain-safe operations.

### Time
Clocks, durations, timestamps, timezone-aware records, NTP integration and protocol timing.

### Files
Paths, files, directories, metadata, streams and configuration.

### Process
Process launch/control, environment, exit status and resource limits.

### Network
TCP, UDP, sockets, listeners, clients, routing and connection state.

### Security
Digest interfaces, integrity manifests, key/certificate interfaces and secure transport contracts.

### HTTP
HTTP 1.x, HTTP 2.0/2.1, HTTP 3/QUIC integration and the experimental SLeeLa HTTP 4.0 profile.

### Data formats
JSON, XML, BODI, structured records and binary framing.

### Email
SMTP message construction, headers, transport and mailbox-facing interfaces.

### Database
Connection, query, transaction, result and persistence abstractions.

### GUI/Terminal
Application surfaces, events, terminal geometry and Phraign™ integration.

### Concurrency
Threads, tasks, locks, channels/mailboxes, futures and cancellation.

### Testing
Assertions, fixtures, unit tests, integration tests and test reporting.

## Domain libraries

Math, Physics, Astrophysics, Chemistry, Economics, Finance, Inference/Statistics and Sociology remain separately versioned subject libraries.

## Design rule

The standard library provides portable contracts. Platform-specific implementations belong behind the OS abstraction or explicitly named platform modules.

**Max Rupplin — MEARVK LLC — 2026**
