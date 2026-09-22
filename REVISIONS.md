# REVISIONS

## 2026-09-22 — Server hardening

### Server Edition
- Added an atomic run lock to `server-edition/serverd` so overlapping server passes fail closed.
- Applied a private default umask to server-created state.
- Replaced the broad verification-output filter with explicit security/defender-line filtering.
- Preserved the one-shot, local-only execution model.

### HTTP 3.0 Key Distribution Server
- Made bootstrap-key file reads and writes resilient to short POSIX I/O operations.
- Reset the issuance window when the system clock moves backward instead of relying on unsigned timestamp subtraction.
- Added TTL overflow protection before computing contract expiration.
- Preserved TLS 1.3, X25519, HKDF-SHA-256, AES-256-GCM, request limits, connection limits, and issuance limits.

### Java HTTP Connector
- Added protocol-version and request-ID headers for request correlation.
- Added an explicit Accept header.
- Added configurable response-size limits and validation.
- Kept explicit request timeouts and structured failure results.

### Architecture
- Added `SERVER.md` documenting the distinction between local Server Edition and the network-facing HTTP 3.0 KDS.
- Documented the remaining portability boundary: the KDS listener still uses POSIX socket/thread APIs and needs a shared native platform adapter before Windows 10+ and macOS can be claimed as native server targets.

## Verification status

Source-level changes were committed to `main`. Native Windows/macOS server builds were not represented as verified by this revision.

## 2026-09-22 — Complete API documentation

- Added `/api/index.html` as the API documentation entry point.
- Added `/api/API.html` as a repository-wide source API catalog covering native C/C++, C++, Java, and SLeeLa source units.
- Included the Java connector class surfaces and existing Email Facade API documentation.
- Kept the catalog source-derived and avoided inventing undocumented signatures.

## 2026-09-22 — HTTP 3.0 extended logical ports

- Added a 160-bit HTTP 3.0 logical port type supporting exactly (10^{48}) distinct values, from 0 through (10^{48}-1).
- Added exact decimal conversion, validation, comparison, increment, and 20-byte network-order encoding in `http-3.0/http3_port.c`.
- Added the actual logical PORT field to HTTP 3.0 textual and binary envelopes.
- Included PORT in the canonical keyed-MAC input so a packet cannot silently change its destination/service port.
- Added envelope APIs for setting a decimal or uint64 port and resealing the packet digest.
- Added a boundary self-test and `http-3.0/PORTS.md`.
- Documented the distinction between the protocol's logical port namespace and smaller native operating-system socket-port limits.


## 2026-09-22 — README and build documentation refresh

- Updated `README.md` with the HTTP 3.0 extended logical-port model and the current protocol build commands.
- Updated `BUILD.md` with dedicated HTTP 3.0 verification targets for the logical-port boundary, native pipeline, and reference tests.
- Documented the requirement that future any-order authenticated packet fields be canonicalized before integrity verification rather than authenticated according to raw wire ordering.
## 2026-09-22 — Full HTML source coverage catalog

- Added `api/SOURCE-COVERAGE.html`, generated from the `master` Git tree, covering all recognized source-code files with language, file size, repository-area role, and direct source links.
- Updated `api/index.html` to expose the exhaustive source inventory.
- Updated `api/API.html` to reflect the current 1,090-file source inventory and link the exhaustive coverage document.
- Kept semantic API claims separate from structural source coverage so the documentation does not invent interfaces that are not present in source.

## 2026-09-22 — BODI XML projects

- Added the `/api/bodi` BODI XML API.
- Added a versioned XML schema for science and SMTP email projects.
- Added the native C++ `bodi-xml-runner` with bounded XML parsing and an allow-listed operation dispatcher.
- Added executable Math and Physics XML operations using the same formula families represented by SLeeLa native subject implementations.
- Added SMTP XML project support using the existing `api/email/sleela_email.h` facade.
- Kept SMTP submission dry-run by default; actual network submission requires `--send-email`.
- Kept SMTP credentials outside XML through `username-env` and `password-env` references.
- Added HTML API documentation and Math, Physics, and SMTP XML examples.
- Unknown disciplines and operations are rejected and witnessed rather than interpreted as arbitrary native or shell commands.


## 2026-09-22 — XML posts, listeners, and routers

- Added native C vocabulary for HTTP-style posts, listener declarations, and deterministic method/path routing.
- Added bounded validation and POST request serialization in `api/posting/sleela_post.c`.
- Added listener contract validation in `api/listener/sleela_listener.c`.
- Added exact route validation and matching in `api/router/sleela_router.c`.
- Extended the BODI XML runner with `post`, `listener`, and `router` project kinds.
- Extended the BODI XSD and added XML examples 07–09.
- Kept XML execution explicit: the new runner validates and witnesses these declarations without implicitly opening sockets or transmitting network traffic.


## 2026-09-22 — Apache/Tomcat Web Server Monitor

- Added `api/webserver/` as a cross-platform SLeeLa deployment and monitoring module for web-aware applications.
- Added a portable C configuration API with bounded server, port, health-path, and module-path validation.
- Added Linux and macOS shell adapters and a Windows 10+ PowerShell adapter.
- Added install, status, health, upgrade, port-change, validation, and explicit local Tomcat WAR deployment operations.
- Added backup-before-port-edit behavior on Unix adapters.
- Kept administrator/root operations explicit and prevented silent selection of unsupported Apache distributions on Windows.
- Added HTML API documentation and an example deployment environment profile.


## 2026-09-22 — Native Database Connector

- Added `api/database/` as a native database connectivity contract for web-aware and general SLeeLa applications.
- Added PostgreSQL, MySQL, MariaDB, SQLite, Microsoft SQL Server, Oracle Database, and ODBC database-family vocabulary.
- Added bounded C configuration validation and stable database-family names.
- Added Linux, macOS, and Windows 8+ client/driver management adapters.
- Added HTML API documentation and security guidance for credential handling and parameterized SQL.
- Kept database server binaries and vendor drivers as explicit platform-managed dependencies rather than silently bundling or downloading arbitrary software.


## 2026-09-22 — Data Analytics science shelf

- Added `api/data-analytics/` with a native C data-set facade and bounded numeric analytics operations.
- Added sum, mean, min, max, population variance, population standard deviation, and correlation operations.
- Limited each data set to 1–1024 finite values and rejected invalid or non-finite input.
- Extended the BODI science dispatcher with the allow-listed `data-analytics` discipline.
- Kept analytics in-process with no file, SQL, or network side effects.
- Added HTML API documentation describing the analytics contract.


## 2026-09-22 — Nordshrift unified input objects

- Added `impl/nordshrift/input_object.{h,cpp}` as a bounded generic SLeeLa input-object model.
- Added closed architecture categories for Core, IO, System, Network, Application, Data, Science, Security, and Deployment.
- Extended the `.sst` parser and sheet model with `object <category> <identity>:` declarations.
- Added bounded `type`, `source`, `target`, `value`, `inputs`, `outputs`, and named-property members.
- Rejected unknown object categories and members instead of silently accepting unsupported contracts.
- Added a representative `examples/input-objects.sst` sheet and `INPUT-OBJECTS.md` specification.
- Added the new source to the main `impl/Makefile` Nordshrift build.
- Kept input objects declarative: declarations do not implicitly execute processes, perform network I/O, modify files, send email, access databases, or deploy services.


## 2026-09-22 — Nordshrift API catalog

- Added `api/nordshrift/API.html` as the public HTML API reference for the Nordshrift `.sst` model.
- Documented all unified SLeeLa input-object categories: Core, IO, System, Network, Application, Data, Science, Security, and Deployment.
- Documented the `InputObject` data structure, syntax, bounds, validation, specialized Nordshrift inputs, semantic subject model, and execution boundary.
- Added Nordshrift navigation to `api/index.html` and `api/README.md`.


## 2026-09-22 — Compiled sleelas Server Launcher

- Added `api/server/sleelas.cpp`, a native C++17 executable that starts the authoritative `server-edition/src/Server.sleela` through the compiled SLeeLa engine.
- Added portable root/engine discovery, Server Edition state-directory setup, atomic single-run locking, `--tick`, `--foreground`, and `--help` handling.
- Added native POSIX process startup for Linux/macOS and Win32 process startup for Windows 10+.
- Added `api/server/Makefile`, `README.md`, and `API.html` documenting the compiled launcher contract.
- Added `.github/workflows/build-sleelas.yml` to compile and upload native Linux x86_64, macOS, and Windows 10+ x86_64 binaries as workflow artifacts.
- Kept server semantics in `Server.sleela`; `sleelas` is the compiled process boundary and does not duplicate or silently expose a public listener.


## 2026-09-22 — sleelas single-instance guard

- Updated `sleelas` to detect an active Server Edition owner process before starting a new engine instance.
- Added a PID owner record to the atomic server lock.
- Existing active servers now cause `sleelas` to exit successfully without starting a duplicate instance.
- Stale locks from terminated processes are eligible for replacement.
- Documented the single-instance behavior in the server API documentation.


## 2026-09-22 — NAT-aware Server deployment

- Added api/server/NAT.md with a complete NAT-aware deployment model for direct addressing, port forwarding/PCP, IPv6, outbound reverse connectivity, and relay fallback.
- Documented CGN and double-NAT conditions, NAT mapping lifetime, keepalives, firewall configuration, reverse rendezvous requirements, failure behavior, and security boundaries.
- Added api/server/nat_aware.h and nat_aware.cpp as a native C++17 configuration/planning boundary.
- Added bounded NAT mode, bind/public endpoint, rendezvous, keepalive, and TLS configuration validation.
- Added sleelas --nat-plan to validate and report the selected deployment path without opening sockets or changing router state.
- Updated the server Makefile to compile the NAT-aware source with sleelas.
- Updated the HTML and Markdown server API documentation.
- Added a Nordshrift deployment object, NatAwareServer, so NAT-aware transport can be declared as a bounded, non-executing SLeeLa input object.
- Kept actual PCP, UPnP, STUN, TURN, TLS transport, and reverse-proxy networking as separate future transport adapters rather than silently embedding network side effects in Server.sleela.

## 2026-09-22 — Static protected managed-memory source boundary

- Added the protected keyword to the SLeeLa lexer and parser.
- Added static/protected member metadata to the AST.
- Enforced the rule that protected fields and methods must also be static.
- Added declaring-class access enforcement for protected fields and methods.
- Added slvm_memory_safe_mode() to make the managed-memory prerequisite explicit at the C/C++ core boundary.
- Added the System Degree 2 definition: class member -> VM-managed storage/handle, not raw pointer arithmetic or pointer dereference count.
- Added impl/examples/static-protected.sleela as a source example.
- Added DEFINITIONS.md and expanded GLOSSARY.md with the static, protected, System Degree 2, managed-memory boundary, and protected-source gate terminology.
- Preserved the existing VM handle model: Sleela source does not receive raw native pointers for runtime resources.


## 2026-09-22 — Next / Next.Next system-degree idiom

- Added the SLeeLa system-navigation idiom `next` for System Degree 1.
- Added the exact `next.next` form for System Degree 2.
- Defined compiler-side back-propagated degree validation so the two-degree protected boundary is explicit and bounded.
- Defined the idiom as VM-safe symbolic navigation rather than native pointers or pointer arithmetic.
- Kept longer `.next` chains outside the protected two-degree contract.
- Added the terms to `DEFINITIONS.md` and `GLOSSARY.md`.
- Updated `impl/examples/static-protected.sleela` to exercise both forms.


## 2026-09-22 — Static Degree-2 viewpoint extension

- Extended the System Degree model so a static Degree-2 proposal gains another two degrees of viewpoint assumption.
- Defined the resulting bounded Viewpoint Degree 4 as `next.next.next.next`.
- Documented that the higher viewpoint can represent both Degree 1 (`next`) and Degree 2 (`next.next`).
- Required programmatic back-propagation when the higher viewpoint requests Degree-1 visibility.
- Kept the entire model symbolic and VM-managed; it does not introduce native pointer arithmetic or unrestricted memory traversal.

## 2026-09-22 — Linux and macOS native API references

- Added API.LINUX.md as the Linux-native SLeeLa API catalog, covering system calls, libc/POSIX, processes, threads, memory, filesystem I/O, sockets, epoll/io_uring, IPC, security, dynamic libraries, terminals, devices, GUI backends, architecture/ABI boundaries, and exposed-call inventory rules.
- Added API.MACOS.md as the macOS-native SLeeLa API catalog, covering Darwin/POSIX, libSystem, Foundation, AppKit, Objective-C Runtime, Core Foundation, Network, Security, IOKit, Metal, XPC, architecture/ABI boundaries, and exposed method/function/class inventory rules.
- Updated api/index.html so the Linux and macOS native API references are directly discoverable from the SLeeLa API documentation entry point.
- Kept native API documentation separate from implementation claims: documented APIs are not automatically exposed to SLeeLa until binding, validation, error handling, ownership, security, tests, examples, and platform behavior are defined.

## 2026-09-22 — Design Activity and Scientific Variance

- Added `impl/core/sleela_design_activity.h/.c` as a shared C ABI for executable design-activity normalization and variance measurement.
- Added six engineering dimensions: correctness, reproducibility, observability, safety, resource discipline, and interoperability.
- Added reference activity profiles for General, Mathematics, Physics, Chemistry, Economics, Inference, Finance, and Data Analytics.
- Added mean-squared variance, RMS deviation, and L2 distance calculations with finite/range validation and no dynamic allocation.
- Added `sleela design-activity ...` and `nordshrift design-activity ...` so both principal executables expose the same Design Activity contract.
- Updated the native build to link one shared implementation into both executables.
- Added `impl/DESIGN-ACTIVITY.md` documenting the norming model and the distinction between software-design variance and scientific measurement uncertainty.


## 2026-09-22 — Server and Service Service 2

- Expanded `server-edition/moral/2/` into the configuration-driven Server and Service package.
- Added declarative configuration for engine, source, inbox, state, logs, integrity manifest, lifecycle mode, tick, foreground operation, and network policy.
- Added Linux/systemd, macOS/launchd, and Windows installation/removal artifacts.
- Added platform service definitions and installation documentation.
- Kept runtime state outside the source package and defaulted Service 2 to local-only operation.
- Moved the authoritative Server Edition source into the Service 2 source boundary at `moral/2/src/Server.sleela`.


## 2026-09-22 — Server and Service Service 3

- Added `server-edition/moral/3/` as the next Server and Service implementation generation.
- Added a remedial server model with explicit preflight, execution, recovery logging, and completion evidence.
- Added declarative configuration for engine, source, inbox, state, log, recovery log, lifecycle mode, tick, foreground operation, preflight, integrity manifest, and network policy.
- Added Linux/systemd, macOS/launchd, and Windows installation/removal artifacts.
- Added Service 3 implementation and installation documentation.
- Kept Service 2 independently available rather than silently replacing its source or behavior.
- Added explicit `SLEELA_SERVER_LEVEL=3` selection to the native `sleelas` launcher.
- Preserved the local-only default network posture.
- Defined recovery records as operational evidence rather than cryptographic or independent verification.
