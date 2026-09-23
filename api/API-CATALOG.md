# SLeeLa Full API Catalogue

Repository: mearvk/SLeeLa
API root: /api

This is the semantic, source-backed catalogue for the SLeeLa API surface. It complements API.html, SOURCE-COVERAGE.html, and FOUNDATIONAL-API.html. The implementation remains authoritative in the repository source tree, especially impl/, runtime/, connector/, gui/, rmi/, protocol trees, and specialized api modules.

## Documentation rule

Implemented means the repository contains a concrete source or API documentation surface. Partially represented means concrete pieces exist but are not yet one unified public facade. Target means the category is useful for a general-purpose platform but is not claimed here as an implemented SLeeLa interface.

## 1. Core values and language objects
Purpose: fundamental values used by SLeeLa programs.
Concepts: Boolean, integers, floating point, strings, characters, byte sequences, buffers, null/absence, arrays, lists, maps, records, structured values, object identity, runtime metadata.
Sources: impl/, src/, SHEET.sheet, STRUCTS.md, SLEELA.md.
Status: Implemented language/runtime surface.

## 2. Standard input/output
Concepts: stdin, stdout, stderr, streams, buffering, formatted output, line input, terminal detection, terminal size/capabilities.
Sources: impl/core/sleela_terminal.c, impl/core/sleela_terminal.h, sleela-terminal/.
Status: Implemented.

## 3. File I/O
Concepts: open, read, write, append, close, seek, flush, descriptors/handles, file streams, FIFO/file channels.
Sources: impl/fileio/, impl/examples/fileio.sleela, impl/examples/fileio-fifo.sleela.
Status: Implemented.

## 4. Paths and filesystem
Concepts: paths, normalization, current directory, parent/name/extension, directory enumeration, create/remove/rename, metadata, permissions, existence/type checks.
Sources: impl/core/, impl/fileio/.
Status: Implemented / partially unified.

## 5. Memory management
Concepts: allocation, release, resize, zeroed allocation, accounting, managed lifecycle, garbage collection, diagnostics.
Sources: impl/core/sleela_memory.c, impl/core/sleela_memory.h, runtime/garbage_collector.c, runtime/garbage_collector.h, MEMORY_MANAGER.md.
Status: Implemented.

## 6. Threads and concurrency
Concepts: thread creation, start/join, locks, conditions, synchronization, task execution, platform thread backends.
Sources: impl/core/sleela_thread.c, impl/core/sleela_thread.h.
Status: Implemented.

## 7. Processes and native execution
Concepts: process creation, executable launch, arguments, environment, stream attachment, exit status, child lifecycle, PTY/native execution, executable validation.
Sources: connector/java/com/mearvk/sleela/connector/process/, runtime/, native launcher/runtime sources.
Status: Implemented.

## 8. Environment and operating-system services
Concepts: environment variables, process identity, host/platform information, working directory, system properties, dynamic-library loading, native handles.
Sources: impl/core/, connector/, platform API references.
Status: Implemented / partially unified.

## 9. Time, clocks and scheduling
Concepts: wall clock, monotonic clock, timestamp, duration, timer, sleep, timeout, scheduling, conversion.
Sources: impl/core/sleela_time.c, impl/core/sleela_time.h, SLEELA_TIME_API.md.
Status: Implemented.

## 10. Networking and sockets
Concepts: addresses, DNS resolution, sockets, client connections, server listeners, accept, send/receive, shutdown, network errors.
Sources: impl/core/sleela_net.c, impl/core/sleela_net.h, impl/core/socket_smoke.c.
Status: Implemented.

## 11. HTTP
Concepts: request, response, method, URI/path, headers, body, status, router, listener, server, client.
Sources: http-2.0/, http-3.0/, api/posting/, api/listener/, api/router/, connector/java/com/mearvk/sleela/connector/http/.
Status: Implemented.

## 12. HTTP 3.0 extended transport
Concepts: packet representation, logical ports, encoding/decoding, crypto integration, key agreement, transport metadata.
Sources: http-3.0/http3_protocol.h, http-3.0/http3_crypto.h, http-3.0/crypto_key_agreement.h, api/http3-port.html.
Status: Implemented.

## 13. FTP, SSH and DNS
Concepts: FTP client/server/session, SSH input/server family, DNS resolution, protocol sessions and service endpoints.
Sources: examples/symmetry/src/ftp/, examples/symmetry/src/dns/, inputs/ssh/, impl/nordshrift/examples/symmetry/src/ftp/.
Status: Implemented examples and protocol surfaces.

## 14. Email and SMTP
Concepts: address, message, sender/recipient, headers, body, SMTP, STARTTLS, implicit TLS, configuration, transport result/error.
Sources: api/email/, including sleela_email.c, sleela_email.h, sleela_email.hpp and sleela_email.sleela.
Status: Implemented.

## 15. Audio and video media
Concepts: timestamped audio/video frames, broad image-format recognition, audio/video codec recognition, container recognition, provider capability discovery, synchronized audio mixing, bass/mid/treble/channel control, live and file inputs, master/second track roles, relative quality, timeline offsets, streaming output, audio level/peak/RMS analysis, spectrum extension, video luminance/edge/motion analysis, frame listeners, transforms, visualization, recording/provider boundaries, and future AI input/output hooks.
Sources: impl/core/sleela_media.h, impl/core/sleela_media.c, impl/core/sleela_media_formats.h, impl/core/sleela_media_formats.c, impl/core/sleela_audio_mixer.h, impl/core/sleela_audio_mixer.c, audio/gui/src/main/java/com/mearvk/sleela/audio/gui/SleelaAudioVideo.java, api/media/.
Status: Native media foundation, synchronized mixer, and broad format/codec registry implemented; Java/JavaFX interoperability signature added; actual device and codec decode/encode providers remain extensible.

## 15A. Artificial intelligence and multimodal analysis
Purpose: a native SLeeLa AI boundary for data, files, audio, and video with Standard SLeeLa VM integration.
Concepts: typed AI inputs, inspection, classification, extraction, summarization, transformation, observations, model descriptors, declarative XML model definitions, declarative XML data models, XML source locators using source kind/url/args, data-flow and AI-flow documents, native providers, connector providers, and bounded VM invocation.
Sources: impl/core/sleela_ai.h, impl/core/sleela_ai.c, impl/tests/core/ai_smoke.c, api/ai/, api/ai/examples/, audio/gui/src/main/java/com/mearvk/sleela/audio/gui/SleelaAi.java.
Status: Native/connector API implemented with XML model and data-model loading; provider-specific inference remains behind the explicit provider boundary.

## 15. GUI and desktop interfaces
Concepts: application, window, document, event, widget/control, layout, menu, dialog, Swing, JavaFX, native bridge.
Sources: gui/java/com/mearvk/sleela/gui/SleelaGui.java, SleelaDocument.java, SwingGui.java, FxGui.java.
Status: Implemented.

## 16. Terminal and shell
Concepts: terminal sessions, terminal I/O, command execution, terminal capabilities, shell integration, interactive control.
Sources: sleela-terminal/, impl/core/sleela_terminal.c, impl/core/sleela_terminal.h.
Status: Implemented.

## 17. Pipes and inter-process communication
Concepts: anonymous pipes, named pipes/FIFOs, reader/writer endpoints, process stream attachment, channel lifecycle.
Sources: impl/examples/fileio-fifo.sleela, impl/core/, process connector sources.
Status: Implemented / partially unified.

## 18. Serialization and structured data
Concepts: JSON, XML, structured records, parsing, serialization, bounded validation, request/response representations.
Sources: src/implementations/, api/bodi/, api/posting/.
Status: Implemented.

## 19. BODI XML project API
Concepts: science projects, math/physics/chemistry, data analytics, SMTP projects, POST projects, listener declarations, router declarations, XML documents, validation and witnessed execution.
Sources: api/bodi/.
Status: Implemented.
Boundary: BODI declarations do not imply arbitrary command execution; external side effects require explicit higher-level operations.

## 20. Database and persistence
Concepts: database, connection, transaction, query, result/record set, prepared operation, repository/storage abstraction, driver selection and configuration.
Families represented: PostgreSQL, MySQL, MariaDB, SQLite, Microsoft SQL Server, Oracle, ODBC.
Sources: api/database/.
Status: Implemented connector family.

## 21. Data analytics
Concepts: data sets, numeric samples, statistics, correlation, bounded computation and analytical results.
Sources: api/data-analytics/.
Status: Implemented.

## 22. Mathematics
Concepts: numeric quantities, equations, formulas, transformations, measurement and validation.
Sources: impl/subjects/math/, impl/tests/subjects/math_values.sleela, Nordshrift subject examples.
Status: Implemented subject-library surface.

## 23. Physics
Concepts: physical quantities, units, relations, formulas, transformations, measured results and validation.
Sources: impl/subjects/physics/, impl/tests/subjects/physics_values.sleela.
Status: Implemented subject-library surface.

## 24. Chemistry
Concepts: chemical values, quantities/units, relations/formulas, transformations and scientific validation.
Sources: impl/subjects/chemistry/, impl/tests/subjects/chemistry_values.sleela.
Status: Implemented subject-library surface.

## 25. Economics and finance
Concepts: economic quantities, financial quantities, currency/value representations, relations, formulas, transformations and comparative results.
Sources: impl/subjects/economics/, impl/subjects/financial/, corresponding subject tests.
Status: Implemented subject-library surfaces.

## 26. Inference and evidence
Concepts: inference, evidence, explanation, assumption, relation, validation, comparative norm and result.
Sources: impl/subjects/inference/, INFERENCE.md, Nordshrift model.
Status: Implemented semantic model.

## 27. Nordshrift semantic API
Core chain: Subject -> Quantity -> Unit -> Assumption -> Relation -> Formula -> Transformation -> Result -> ComparativeNorm -> Evidence -> Explanation -> Validation.
Additional concepts: Todo and WorkPlan.
Sources: api/nordshrift/, impl/nordshrift/, SST.model, SST-2.0.model, NORDSHRIFT.md.
Status: Implemented.

## 28. Security, integrity and execution supervision
Concepts: execution policy, security supervisor, integrity verification, SHA-256 verification, memory supervision, native execution gate, privilege boundary and diagnostics.
Sources: runtime/security_supervisor.c, runtime/security_supervisor.h, runtime/runtime_security.sleela, runtime/SECURITY_SUPERVISOR.md, runtime/Parameters.c, runtime/Parameters.h.
Status: Implemented runtime security surface.

## 29. Cryptography and key agreement
Concepts: digest/hash, cryptographic keys, key agreement, integrity checks, authenticated protocol data and TLS-related configuration where supplied by the platform.
Sources: http-3.0/http3_crypto.h, http-3.0/crypto_key_agreement.h and related native sources.
Status: Implemented protocol-facing cryptographic surfaces.

## 30. Dynamic libraries and native interoperability
Concepts: dynamic library load/unload, symbol lookup, native handles, C ABI integration, C++ implementation bridge and Java/native connector boundary.
Sources: impl/core/, connector/, native C/C++ implementation.
Status: Implemented / partially unified.

## 31. Java/JVM interoperability
Concepts: Java, Kotlin, Scala, Groovy and Clojure source ingestion, JVM connectors, Java class/object integration.
Sources: langin/, connector/java/, src/implementations/.
Status: Implemented ingestion/connector surfaces.

## 32. Remote method invocation and distributed objects
Concepts: remote object, endpoint, client, server, service, session and invocation context.
Sources: rmi/.
Status: Implemented Java RMI family.

## 33. Server and service runtime
Concepts: server launcher, service process, runtime initialization, configuration, lifecycle, service endpoint.
Sources: server-edition/src/Server.sleela, api/server/.
Status: Implemented.

## 34. Web-server deployment and monitoring
Concepts: installation, status, health checks, upgrades, ports, validation, local WAR deployment and platform adapters.
Platforms represented: Linux, Windows 10+, macOS.
Sources: api/webserver/.
Status: Implemented.

## 35. Posting, listeners and routers
Posting: POST representation, validation and bounded serialization.
Listener: bind, port, protocol, route vocabulary and validation.
Router: method, path and deterministic matching.
Sources: api/posting/, api/listener/, api/router/.
Status: Implemented.

## 36. HTTP-style service composition
Concepts: server, listener, router, request, response, handler and service lifecycle.
Sources: api/server/, api/webserver/, api/posting/, api/listener/, api/router/.
Status: Implemented across specialized modules.

## 37. Phraign terminal/frame interface
Concepts: frame, pixel/geometry representation, terminal frame control, Bash protocol boundary and C++ geometry/frame implementation.
Status: repository-defined specialized surface; exact contracts remain in the corresponding Phraign source and documentation.

## 38. Compiler and language tooling
Concepts: parse/check, compile, version, runtime invocation, native execution, language metadata and source ingestion.
Sources: COMPILER.md, SLEELA.md, SOURCE.md, langin/, impl/.
Status: Implemented.

## 39. Runtime introspection and diagnostics
Concepts: runtime version, platform/backend identification, memory diagnostics, security diagnostics, execution status and errors.
Sources: runtime/, impl/, Parameters.*.
Status: Implemented / partially unified.

## 40. Error and status handling
Concepts: success/failure, error codes, diagnostics, validation results, transport errors, execution status and security rejection.
Sources: impl/core/, runtime/ and specialized api modules.
Status: Implemented as distributed source contracts.

## 41. Application configuration
Concepts: parameters, runtime configuration, network configuration, server configuration, database configuration and security configuration.
Sources: runtime/Parameters.c, runtime/Parameters.h and specialized API configuration sources.
Status: Implemented / distributed.

## 42. Examples and executable demonstrations
Sources: api/examples/, examples/, tutorial/demos/, impl/examples/, impl/nordshrift/examples/.
Status: Implemented.

## 43. Simple Regex and pattern matching
Concepts: four-level user-facing text matching; Find, Shape, Structure, and Expression; literal matching; named character classes; repetition; alternatives; groups; ranges; optional values; captures; Unicode properties; compiled patterns; bounded deterministic matching.
Sources: api/regex/README.md, api/regex/API.html, api/regex/REGEX-DESIGN.md, api/regex/regex-patterns.sleela, api/regex/LEVEL-1-FIND.sleela, api/regex/LEVEL-2-SHAPE.sleela, api/regex/LEVEL-3-STRUCTURE.sleela, api/regex/LEVEL-4-EXPRESSION.sleela, impl/core/sleela_regex.h, impl/core/sleela_regex.c, impl/tests/core/regex_smoke.c, impl/REGEX-NATIVE.md.
Status: **Draft with native Level 1/2 foundation; Level 3/4 source contracts are defined but their native execution remains intentionally unsupported pending semantic review.**

# API directory map

- api/API.html — generated repository-wide API/source catalogue.
- api/SOURCE-COVERAGE.html — exhaustive recognized-source inventory.
- api/FOUNDATIONAL-API.html — foundational semantic map.
- api/API-CATALOG.md — this full semantic catalogue.
- api/email/ — email/SMTP.
- api/database/ — database connectors.
- api/data-analytics/ — numeric/data analysis.
- api/bodi/ — BODI XML project model.
- api/posting/ — POST API.
- api/listener/ — listener API.
- api/router/ — routing API.
- api/regex/ — Simple Regex API: four-level text pattern matching and Unicode-aware design draft.
- api/nordshrift/ — Nordshrift semantic API.
- api/server/ — compiled server launcher.
- api/webserver/ — Apache/Tomcat deployment and monitoring.
- api/examples/ — API demonstrations.
- api/http3-port.html — HTTP 3.0 logical-port reference.

# Source-of-truth rule

The catalogue does not make /api the implementation authority. Actual source declarations and implementations remain authoritative. Every future API entry should identify its source and avoid documenting capabilities that are not present in the repository.

# Maintenance

When a public API is added: update its implementation, add a focused /api reference when appropriate, add it here, link declaration and implementation, add an example or test when practical, regenerate SOURCE-COVERAGE.html, and keep API.html, index.html and README.md as navigation surfaces.
