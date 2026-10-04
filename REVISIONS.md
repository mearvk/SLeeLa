## 2026-10-03 — Language core: arrays, multi-file input, and /languages packs

Three additive language/runtime features. All existing programs continue to
compile and run unchanged.

### First-class array type

Added a dynamic, growable, zero-indexed **array** type (`T[]`). Surface:
`new T[n]`, `a[i]` read, `a[i] = v` write (runtime bounds-checked), and the
built-ins `arrayNew`, `arrayLength`, `arrayGet`, `arraySet`, `arrayPush`.
Implementation spans the whole pipeline: a `Kind::Array` with element typing in
the semantic analyzer; lowering of index read/write, `new T[n]`, and the
built-ins in the compiler; the parser now accepts scalar element types after
`new` (`new int[n]`); and a new `SL_ARRAY` value tag backed by a mutex-guarded,
GC-integrated, geometrically growing instance store in the core, with opcodes
`OP_NEWARRAY`/`OP_ARRGET`/`OP_ARRSET`/`OP_ARRLEN`/`OP_ARRPUSH` appended at the
end of the `SLOp` enum (so serialized-artifact opcode numbers are unchanged).
The source/native ISA registries (`lib/vm/InstructionSet.sleela`,
`lib/vm/OPCODE-MAP.md`) were updated to match. See `STRUCTS.md`.

### Multiple `.sleela` inputs at run and compile time

`sleela run a.sleela b.sleela ...` and
`sleela compile a.sleela b.sleela ... -o out.sleela` now parse each file and
merge them into one compilation unit (concatenating classes/structs/imports and
annotations), mirroring the existing xclass/langin multi-file loaders. A program
can therefore be split across files; duplicate definitions across files are
reported as before.

### `/languages` localization packs

Added a `languages/` folder of dependency-free `key = value` language packs that
localize SLeeLa's outputs and prompts: English (reference), Spanish, French,
German, Portuguese, Italian, Japanese, Chinese (Simplified), Hindi, and Arabic
(right-to-left). The pack format, key set, and reference English values are in
`languages/README.md`, `languages/KEYS.md`, and `languages/en.lang`. A SLeeLa
library under `lib/languages/` (`SLLanguage`, `SLLanguageCatalog`, and the
runnable `languages.sleela`) selects a locale and returns localized text; the
packs are original translations authored for this repository, so no third-party
pack licenses are bundled.

## 2026-10-03 — New subsystems: Website Generator and AutoCAD Renderer

Two new application subsystems were added, each following the repository rule
*JavaFX presents; SLeeLa decides*: the SLeeLa source is authoritative and the
JavaFX GUI is a thin authoring/preview surface that reaches SLeeLa through the
common connector (`com.mearvk.sleela.connector`).

### `website-generator/`

Builds custom websites from a **design** (theme, palette, an `energy` dial, and
a layout grammar) and an ordered list of semantic **signs** (hero, feature,
gallery, pricing, testimonial, cta, nav, footer, richtext).

- SLeeLa generator (authoritative): `website/design.sleela`, `website/signs.sleela`,
  `website/emitter.sleela`, and the runnable `website/site.sleela`, which emit a
  single self-contained HTML + CSS site.
- JavaFX authoring surface: `website-generator/gui/` (`WebsiteStudioApp`) with a
  design preset + energy dial + palette controls, a sign list editor, and a live
  `WebView` preview.
- Docs and sample: `README.md`, `DESIGN.md`, `samples/starter.site`, `Makefile`.

### `autocad/`

Takes **descriptions, dimensions, plans, and notes** and renders the output as
an AutoCAD **DXF** drawing (R12 text — opens directly in AutoCAD / LibreCAD).

- SLeeLa renderer (authoritative): `cad/model.sleela`, `cad/plan.sleela`,
  `cad/dxf.sleela`, and the runnable `cad/render.sleela`, which parse a plan,
  evaluate the geometry (room expansion, extents, dimension measurement, note
  placement), and emit DXF.
- JavaFX plan-entry surface: `autocad/gui/` (`CadStudioApp`) with a plan editor,
  a `Canvas` geometry preview drawn from the renderer's entity list, and DXF
  export.
- Docs and sample: `README.md`, `FORMAT.md`, `samples/office.plan`, `Makefile`.

Each GUI mirrors the `audio/gui/` module layout (Maven `pom.xml`,
`module-info.java`, presentation model, JavaFX application). The live connector
transport is each subsystem's next integration point and is marked in the
model's `generate()` / `render()` method; a faithful offline fallback keeps the
studios usable until it is wired. The additions do not affect the `impl/` build
or the SHA-256 verification gate.

## 2026-10-03 — Corrections

Build, source, and test corrections made while verifying that the C/C++
implementation builds and runs. These are fixes to pre-existing defects, not new
features. `cd impl && make test` passes end to end; the top-level `make`
completes; the repo-wide sub-project build sweep is clean except for
environment-only targets (macOS, PowerShell, Linux kernel headers, GTK4/VTE).

### Compiler front end (`impl/`)

- Completed the frontend AST: added the ~16 Java statement/expression nodes the
  parser produced but that were never declared in `ast.h` or handled by the
  semantic analyzer and bytecode compiler (`do/while`, `switch`, `break`,
  `continue`, `throw`, `assert`, `yield`, `synchronized`, `try/catch/finally`,
  assignment/compound-assignment, ternary `?:`, `instanceof`, cast,
  `this`/`super`, array access, method reference), and wired each through
  semantic checks and lowering. The front end had never compiled before.
- Fixed a latent `Binary` constructor bug that moved the right operand into the
  left and left the right uninitialized (miscompiling `-`, `/`, `%`, and
  comparisons).
- The analyzer now accepts the Java keyword type names the lexer emits
  (`String`, `boolean`) and treats declared class/interface names as types;
  `+` with a String operand is string concatenation; a bare `return;` in a void
  method is valid; `spawn`, `structUnpack`, `Munction.start`, and the `next.next`
  idiom are recognized to match the compiler.

### Build-blocking defects

- Repaired corrupted sources: mis-escaped HTTP token-separator and logger
  string literals, over-escaped `'\\0'` char constants, and files whose content
  had collapsed onto one line with literal `\n` (several `.c`/`.cpp`/`.h`, and
  `http-8.0/DarkPower.{hpp,cpp}` entirely).
- Removed duplicate `case` labels and declarations (lexer, parser,
  `debugger_backend.hpp`, VM security functions) and a `RegisterSnapshot` name
  clash between two debugger headers.
- Added missing includes/declarations and fixed Makefile paths, missing
  compile/link rules, and cross-module link dependencies across `impl`,
  `http-3.0/4.0`, `telephony-skya/drivers`, `lib/vm`, `lib/compiler`, and the
  `sleela-virtual-machine` generations.

### Memory safety

- The native memory manager (`api/native/memory`) freed the wrong address for
  alignment-padded allocations (`free(): invalid pointer`); it now records and
  frees the true allocation base.

### Tests, fixtures, and tooling

- Reconciled the example/test corpus and version harnesses with the documented
  syntax range (1.3 .. 1.6); fixed genuinely wrong test expectations (regex
  anchored-vs-substring and capture count; sociology weighted mean; GC
  destructor argument order; memory-manager limit).
- Restored the execute bit on 57 `.sh` scripts that Makefiles invoke directly.
- The top-level `make` and `scripts/build-{linux,macos}.sh` now default
  `SLEELA_SHA256_MANIFEST` so the build works from the repo root.
- Added `libs/` download scripts (and a manifest) for the external dependencies
  that are 50 MB or less, with a per-artifact size cap.

These corrections establish build and test integrity; they do not by themselves
claim native runtime qualification beyond what `make test` exercises.

## 2026-10-02 — SLeeLa Source Inventory and Version-Control Refresh

- Advanced the active development line to **0.3.22-dev**.
- Advanced the compiler compatibility gate to **2.9-dev**.
- Recounted `/lib`: **10,033 `.sleela` source units across 75 package families**.
- Preserved **88 module-facade symbols** and updated the manifest total to **10,121 symbol records**.
- Recorded **10,235 repository-wide `.sleela` source files**.
- Synchronized library manifest/index and inventory documentation with the current source tree.
- These are structural source-tree counts and do not by themselves claim native build or runtime qualification.

# REVISIONS

## 2026-09-22 — Discord™ Server Naming Theme

- Named the three SLeeLa Server Edition implementations Discord-1™, Discord-2™, and Discord-3™ in source and documentation.
- Assigned Discord-1™ to the native `sleelas` launcher, Discord-2™ to Service 2, and Discord-3™ to Service 3.
- Added explicit third-party trademark/non-affiliation notices to the Server Edition documentation.
- Kept Discord logos, trade dress, and proprietary Discord Brand Assets out of the repository.


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
## 2026-09-23 — Native Code Cleanup

- Hardened SMTP email transport validation and response-code parsing.
- Added SMTP DATA dot-stuffing for plaintext and TLS email paths.
- Rejected incomplete SMTP username/password configuration.
- Removed the unused OpenSSL BIO include from the email implementation.
- Hardened HTTP/2 send-callback failure reporting.
- Separated HTTP/2 header-count and header-byte limits.
- Kept the existing C/C++ architecture and public API contracts intact.
=======
# SLeeLa Revisions and Repository Cleanup

**Status:** Structural cleanup, C/C++ review, and documentation consolidation  
**Date:** 2026-09-15  
**Branch:** `master`

## Purpose

This document records the repository-wide cleanup and source review performed around the SLeeLa source tree, build system, security verification material, and retained documentation. The objective is to make the repository easier to maintain without introducing unreviewed semantic changes to the compiler or runtime.

## Cleanup principles

1. **Preserve behavior.** Cleanup should not silently change language semantics, runtime behavior, file formats, or public command names.
2. **Keep source separate from generated output.** Build artifacts belong under ignored build directories and are not source material.
3. **Keep tools executable.** `tools/` is for scripts and command-line utilities; explanatory security documentation belongs under `security/`.
4. **Keep security gates explicit.** SHA-256 verification remains part of the build and execution contract.
5. **Avoid accidental IDE state.** Editor/project metadata is not part of the portable source distribution.
6. **Prefer small, reviewable revisions.** Large mechanical rewrites are deferred unless they can be verified without changing behavior.
7. **Review security-sensitive C/C++ before stylistic rewriting.** Source formatting must not obscure changes to execution, process invocation, filesystem access, networking, or privilege boundaries.

## Repository structure

### Root

The repository root retains public specifications, language references, examples, catalogs, and project-level documentation that are intentionally addressable from the README.

### `impl/`

The implementation tree contains the C/C++ execution core, frontend/compiler, Nordshrift implementation, catalog, xclass support, subject libraries, platform interfaces, tests, and the authoritative `Makefile`.

### `tools/`

`tools/` contains executable or operational utilities, including:

- `build-verified.sh` — verification-first build entry point.
- `verify-before-execution.py` — fail-closed SHA-256 verifier.
- `generate-sha256-manifest.py` — manifest generator.
- data-generation and migration utilities.

Documentation that defines the security contract is retained under `security/` instead of being mixed with executable tools.

### `security/`

`security/` is the home for the build-verification contract, SHA-256 manifest, and related security documentation.

### `.github/`

GitHub Actions workflows remain under `.github/workflows/`. They are retained because they perform repository-specific generation, migration, testing, and maintenance work.

### `.kiro/`

The Kiro steering material is retained. It is project guidance rather than generated output and therefore is not treated as disposable IDE metadata.

## Changes made in this cleanup

### 1. Removed IDE-specific project state

The committed `.idea/` project metadata was removed. This includes editor dictionaries, IntelliJ project configuration, UI designer state, module metadata, and VCS metadata. These files are local development state rather than portable SLeeLa source.

A root `.gitignore` now excludes `.idea/`, `.vscode/`, IDE module files, common compiler artifacts, build directories, and temporary editor files.

### 2. Consolidated security documentation

`tools/SHA256-VERIFICATION.md` was moved to:

`security/SHA256-VERIFICATION.md`

The executable verification helpers remain in `tools/`, while the security contract and manifest remain together in `security/`.

### 3. Preserved the verified-build architecture

The existing verification-first build path is retained:

`tools/build-verified.sh` → `impl/Makefile` → `verify-security` → compiler/link targets

The committed manifest remains:

`security/sha256-manifest.json`

The native frontend's execution gate remains in `impl/frontend/driver.cpp` and uses the same SHA-256 verification tool.

### 4. Added a conservative C/C++ formatting baseline

A root `.clang-format` was added to establish a stable, reviewable formatting baseline for future C and C++ cleanup. The configuration intentionally avoids an aggressive whole-tree rewrite: it standardizes indentation, braces, spacing, include organization, comments, and line length while keeping short security-sensitive control-flow constructs expanded.

The formatter configuration is a policy baseline, not permission to reformat the entire implementation in one commit. Source changes should still be made subsystem-by-subsystem and reviewed for semantic and security impact.

## C/C++ source review

A focused review of the implementation source was performed after the structural cleanup, with particular attention to `impl/frontend/driver.cpp` and `impl/Makefile`.

### Findings in `impl/frontend/driver.cpp`

- The file contains several heavily compressed one-line functions and statements. This makes security-sensitive control flow harder to audit than necessary.
- Include ordering and spacing are inconsistent with conventional C++ style.
- `namespace fs=std::filesystem;` and several compact declarations should be normalized during a controlled formatting pass.
- Shell/process invocation is concentrated in `runCommand()` and the Defender helpers. These paths deserve especially clear formatting because they cross from native C++ into operating-system commands.
- The SHA-256 gate is clearly present for compile, run, check, xclass, and Defender build/install/provision operations.
- Defender `fetch` is currently a network acquisition operation and is not itself passed through the native SHA-256 gate. This should remain an explicit design decision; it should not be accidentally changed by a formatter or refactor.
- `defender build`, `install`, and `provision` can fetch a missing Defender source tree before the verification gate is invoked. The next security-hardening pass should decide whether verification should occur before any network acquisition, rather than relying only on verification before the build/install step.
- The current command execution helper uses `std::system()`. Any future refactor should preserve quoting and platform behavior while considering a more structured process-execution interface.
- Error handling is generally fail-closed, but the command runner currently reduces process status to success/failure. A later portability pass could preserve signal/exit-status detail without changing command semantics.

### Findings in `impl/Makefile`

- The build already has an explicit `verify-security` target and makes `all` and `build-verified` depend on it.
- Object targets use `verify-security` as an order-only prerequisite, which correctly keeps the phony verification target out of link command `$^` expansion.
- The test aggregate explicitly starts with `verify-security`.
- Direct requests for the named binary targets should also be checked against the verification invariant: a user should not be able to bypass verification merely by invoking an already-built executable target when its object prerequisites are already current.
- Individual test targets and direct binary invocations should continue to be reviewed for the invariant that no executable or diagnostic is run without the applicable SHA-256 verification gate.
- The Makefile is readable enough to retain as the authoritative build definition; a full rewrite is not warranted merely for stylistic reasons.

## C/C++ cleanup policy

The C/C++ review does **not** authorize a blind whole-tree reformatter. The source contains compiler, runtime, filesystem, process, platform, and security-sensitive code. Formatting and simplification should therefore proceed in subsystem-scoped passes.

Recommended order for the next controlled source pass:

1. `impl/frontend/driver.cpp` — command dispatch, verification, filesystem and process boundaries.
2. `impl/frontend/` lexer/parser/compiler/artifact sources.
3. `impl/core/` C runtime and platform interfaces.
4. `impl/runtime/` security and memory/runtime support.
5. `impl/subjects/` subject modules.
6. `impl/catalog/`, `impl/xclass/`, and `impl/nordshrift/`.
7. Smoke tests and remaining test sources.

The first source pass should make the `driver.cpp` control flow auditable without changing command behavior, and should separately address the Defender network-acquisition ordering after a security review. The Makefile should then be hardened so direct binary targets retain the verification invariant even when their object files are already current.

Each pass should preserve behavior, build successfully through the verified entry point, run applicable tests, and regenerate the SHA-256 manifest for changed verified files.

## Retained documentation

The existing language, compiler, runtime, subject-library, filesystem, networking, terminal, catalog, and security documents remain available unless they are clearly generated, obsolete, or replaced. This cleanup does not delete technical references merely because they are old or unusually named; their relationships should be reviewed before removal.

The repository README remains the high-level map. Implementation-specific documentation remains alongside `impl/`, while security/build documents remain under `security/`.

## Verification requirements after future cleanup

Before accepting future source or build changes:

1. Regenerate `security/sha256-manifest.json` when a verified source file changes.
2. Run the verification gate.
3. Build through `tools/build-verified.sh`.
4. Run the appropriate test targets.
5. Review generated/build output to ensure it is not accidentally tracked.
6. Update this document when the repository structure or retained-document policy changes.

## Deferred cleanup

The following are intentionally deferred until they can be handled with complete source visibility and verification:

- broad mechanical formatting of all C/C++ files;
- immediate semantic changes to Defender acquisition ordering without a separate security review;
- changes to direct Makefile binary-target verification without a complete current Makefile revision;
- mass renaming of public documentation files;
- relocation of root-level language specifications;
- deletion of historical domain documents whose current consumers have not been identified;
- changes to compiler/runtime behavior solely for stylistic reasons;
- redesign of process execution or Defender acquisition without a separate security review.

## Revision record

### 2026-09-15 — Repository hygiene pass

- removed committed IntelliJ `.idea/` metadata;
- added root repository hygiene `.gitignore`;
- moved SHA-256 security documentation from `tools/` to `security/`;
- retained the verification-first build architecture;
- documented the source/folder ownership model;
- established a conservative policy for future source formatting and document retirement.

The goal is a smaller, clearer, more portable repository without sacrificing the existing implementation or security contracts.
=======
### 2026-09-15 — C/C++ review pass

- reviewed the native frontend command/security boundary in `impl/frontend/driver.cpp`;
- reviewed the verification ordering and build/test structure in `impl/Makefile`;
- identified compressed source formatting as an auditability issue rather than immediately applying a risky whole-tree reformat;
- identified Defender fetch/verification ordering as a security-hardening point for a subsequent controlled code change;
- identified direct binary-target verification as an additional Makefile hardening point;
- documented a subsystem-by-subsystem C/C++ cleanup order;
- preserved the existing compiler/runtime behavior while recording the source-quality and security findings.

### 2026-09-15 — Formatting baseline pass

- added root `.clang-format` with conservative C/C++ formatting rules;
- established a common baseline for the upcoming `driver.cpp` and subsystem cleanup passes;
- deliberately avoided mass formatting until complete source visibility and verified builds are available.

The goal remains a smaller, clearer, more portable repository without sacrificing the existing implementation or security contracts.
