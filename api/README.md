# SLeeLa API

## Full API Catalogue

See [API-CATALOG.md](API-CATALOG.md) for the complete semantic catalogue of SLeeLa's general-purpose API families, source locations, and implementation status. The catalogue covers core I/O, files, memory, threads, processes, networking, HTTP, GUI, email, databases, security, Nordshrift, subject libraries, JVM interoperability, RMI, servers, and related services.

The API tree contains native C/C++ facades and SLeeLa mappings. The documentation is intended to describe source-backed contracts rather than placeholder interfaces.

## General API families

- **Posting** — `api/posting/`: HTTP-style POST representation, validation, and bounded serialization.
- **Listener** — `api/listener/`: listener bind/port/protocol/route vocabulary and validation.
- **Router** — `api/router/`: deterministic method/path route vocabulary and exact matching.
- **Email** — `api/email/`: SMTP-aware submission using plain SMTP, STARTTLS, and implicit TLS.
- **BODI XML** — `api/bodi/`: declarative XML projects and witnessed execution, including bounded media projects.
- **Audio / Video** — `api/media/`: timestamped native frames, broad image/video/audio format and codec registry, signal analysis, listeners, transforms, and future AI provider hooks. **Audio Mixer** — `api/audio-mixer/`: synchronized live/file inputs, master/second tracks, bass/mid/treble/channel controls, relative quality, timeline offsets, and JSON/XML/text configurations.
- **Data Analytics** — `api/data-analytics/`: bounded numeric statistics and correlation operations for scientific data sets.
- **HTTP 3.0** — protocol-specific packet and logical-port APIs.

## BODI project model

BODI currently represents:

1. Science projects for math, physics, chemistry, and data analytics.
2. Email/SMTP projects.
3. HTTP-style post projects.
4. Listener declaration projects.
5. Router declaration projects.

See `api/bodi/API.html` for the XML contract and `../examples/` for runnable source examples and expected evidence.

## Network boundary

The XML layer does not turn an XML document into arbitrary network execution. POST, listener, and router declarations are validated through native C APIs and witnessed by the runner. Actual socket lifecycle or external network transmission must be an explicit higher-level operation.

## Build

    cd api/bodi
    make

## Web Server Monitor

`api/webserver/` provides the SLeeLa Apache/Tomcat deployment and monitoring module. It includes Linux, Windows 10+, and macOS adapters for installation, status, health checks, upgrades, port changes, validation, and explicit local WAR deployment to Tomcat. See `api/webserver/API.html` and `api/webserver/README.md`.


## Native Database Connector

See `api/database/` for the cross-platform database API, HTML reference, C facade, and Linux/macOS/Windows 8+ driver-management adapters. Supported families include PostgreSQL, MySQL, MariaDB, SQLite, Microsoft SQL Server, Oracle, and ODBC.


## Nordshrift

See `api/nordshrift/API.html` for the complete Nordshrift API reference, including the `.sst` semantic model and the unified input-object vocabulary for Core, IO, System, Network, Application, Data, Science, Security, and Deployment. The generic object layer complements the specialized Nordshrift sections and remains declarative; it does not implicitly execute side effects.

## sleelas

See `api/server/API.html` for the compiled `sleelas` server launcher. It starts `server-edition/src/Server.sleela` through the normal compiled `sleela` engine. Native binaries are produced for Linux x86_64, macOS, and Windows 10+ x86_64 by the repository CI workflow.

- [AI API](ai/README.md) — data, file, audio, and video AI with native/VM connector support and XML model/data contracts.

## Regex API

See `api/regex/` for the **SLeeLa Regex API 1.0.0 draft**. It introduces a deliberately simple four-level pattern language: **Find**, **Shape**, **Structure**, and **Expression**. The design puts readable text questions ahead of punctuation-heavy regex syntax while preserving a documented Unicode-aware and deterministic implementation boundary. Level exemplars are `LEVEL-1-FIND.sleela` through `LEVEL-4-EXPRESSION.sleela`; the native C foundation lives in `impl/core/sleela_regex.c` and `impl/core/sleela_regex.h`.
