# SLeeLa Build and Makefile Inventory

SLeeLa Version: 0.3.0-dev  
Inventory Date: 2026-09-29  
Purpose: Repository-wide build-surface audit

## Status

The repository has been audited on both main and master for tracked build directories, Makefiles, and .mk fragments.

| Surface | master | main |
|---|---:|---:|
| Tracked build directories | 18 | 18 |
| Makefile / GNUmakefile / makefile / .mk files | 69 | 69 |
| Repository-level Makefile | added by this change | added by this change |

## Build directory inventory

The tracked build surfaces are:

- build/
- coorenagraph/build/
- http-1.0/build/ through http-9.0/build/
- http/build/
- http/7.0/build/, http/8.0/build/, http/9.0/build/
- ide/build/
- regex/build/
- telephony-skya/build/

The HTTP version build directories contain their own Makefiles and negotiation fragments. They are intentionally product/version-local build systems rather than one shared generated output tree.

## Makefile inventory

The 69 tracked build-control files cover:

- API components under api/
- Audio and codec implementations
- CommonRails printing
- Coorenagraph
- Debugger, drivers, examples, ledger, and native layers
- HTTP 1.0 through 9.0 and HTTP server editions
- IDE and terminal components
- impl/ and Nordshrift
- java28/
- Regex and regex test suites
- Server edition
- Skya drivers/native components
- tests/ and video/

.mk fragments such as the HTTP negotiation.mk files are included in the 69-file control inventory.

## Repository-level dispatcher

A new root Makefile provides a stable entry point without replacing product-specific Makefiles:

- make all — core + Java 28 + regex + native test build/test surfaces
- make core — impl/
- make java28 — java28/
- make regex — regex/
- make tests — tests/
- make server — api/server/
- make clean — clean those dispatched surfaces

The dispatcher deliberately does not enumerate source files. Each subsystem Makefile remains authoritative.

## Build-documentation findings

All 18 tracked build directories now have a local README.md or BUILD.md. The repository build surface therefore has both local build notes and a cross-repository inventory.

The existing build/README.md, ide/build/README.md, and telephony-skya/build/README.md document their respective build surfaces. The new root inventory is the cross-repository index.

## CI alignment

The Linux, macOS, and Windows core workflows already invoke impl/Makefile, while the server-launcher workflow invokes api/server/Makefile. This matches the product-specific build ownership model.

## Branch note

main and master remain intentionally divergent. This audit does not merge or force-sync them. The same root build dispatcher and inventory are applied independently so neither branch loses existing work.

## Java 28 note

java28/Makefile is the native/JVM integration build. The large Java 28 SLeeLa source-envelope expansion under /lib/java is a separate source-inventory task and is not treated as generated build output.

— Max Rupplin - MEARVK LLC - 2026
