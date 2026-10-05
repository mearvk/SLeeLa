# SLeeLa Build and Makefile Inventory

SLeeLa Version: 0.3.0-dev  
Inventory Date: 2026-10-05  
Purpose: Repository-wide build-surface audit

## Status

The repository has been audited on both main and master for tracked build directories, Makefiles, and .mk fragments.

| Surface | master | main |
|---|---:|---:|
| Tracked build directories | 22 | 22 |
| Makefile / GNUmakefile / makefile / .mk files | 93 | 93 |
| Repository-level Makefile | present | present |

## Build directory inventory

The 22 tracked build surfaces are:

- build/
- coorenagraph/build/
- http-1.0/build/ through http-9.0/build/
- http/build/
- http/7.0/build/, http/8.0/build/, http/9.0/build/
- ide/build/
- regex/build/
- sleela-virtual-machine/7/build/, sleela-virtual-machine/8/build/, sleela-virtual-machine/9/build/, sleela-virtual-machine/11/build/
- telephony-skya/build/

The HTTP version build directories contain their own Makefiles and negotiation fragments. They are intentionally product/version-local build systems rather than one shared generated output tree.

## Makefile inventory

The 93 tracked build-control files cover:

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
- Application subsystems: website-generator/ and autocad/ (each with a driver Makefile and a JavaFX GUI Maven module under its gui/)

.mk fragments such as the HTTP negotiation.mk files are included in the 93-file control inventory.

## Application subsystem build surfaces

Two application subsystems add their own driver Makefiles plus a JavaFX GUI
Maven module. Each Makefile only *dispatches* into the authoritative SLeeLa
source (it runs the SLeeLa runtime on the subsystem's `.sleela` entry point and
captures the output); the GUI module builds with Maven like `audio/gui/`.

| Subsystem | Driver Makefile | SLeeLa entry | GUI module |
|---|---|---|---|
| `website-generator/` | `website-generator/Makefile` (`make generate` → `out/index.html`) | `website/site.sleela` | `website-generator/gui/pom.xml` |
| `autocad/` | `autocad/Makefile` (`make render` → `out/drawing.dxf`) | `cad/render.sleela` | `autocad/gui/pom.xml` |

These dispatchers follow the same ownership model as the rest of the inventory:
the subsystem's SLeeLa source and the GUI `pom.xml` remain authoritative.

## Repository-level dispatcher

The root Makefile provides a stable entry point without replacing product-specific Makefiles:

- make all — core + Java 28 + regex + compiler + decompiler + VM + scripting + JetBrains helper + install + tutorial-check + tests + config + route
- make core — impl/
- make java28 — java28/
- make regex — regex/
- make compiler — lib/compiler/
- make decompiler — lib/decompiler/
- make vm — lib/vm/
- make scripting — sleela-scripting/
- make jetbrains — JetBrains source acquisition helpers
- make install — install/
- make tutorial-check — verify tutorial/example inventories (via `scripts/tutorial-check.sh`): README-linked lessons resolve, lesson numbering is contiguous, example XML is well-formed, and every declared expected-evidence witness is paired with its example
- make tests — tests/
- make server — api/server/
- make route — validate unified route data (route/ROUTE.DATA.json)
- make config — show unified configuration location
- make clean — clean those dispatched surfaces

The dispatcher deliberately does not enumerate source files. Each subsystem Makefile remains authoritative.

## Build-documentation findings

All 22 tracked build directories now have a local README.md or BUILD.md. The repository build surface therefore has both local build notes and a cross-repository inventory.

The existing build/README.md, ide/build/README.md, and telephony-skya/build/README.md document their respective build surfaces. The new root inventory is the cross-repository index.

## CI alignment

The Linux, macOS, and Windows core workflows already invoke impl/Makefile, while the server-launcher workflow invokes api/server/Makefile. This matches the product-specific build ownership model.

## Branch note

main and master remain intentionally divergent. This audit does not merge or force-sync them. The same root build dispatcher and inventory are applied independently so neither branch loses existing work.

## Java 28 note

java28/Makefile is the native/JVM integration build. The large Java 28 SLeeLa source-envelope expansion under /lib/java is a separate source-inventory task and is not treated as generated build output.

— Max Rupplin - MEARVK LLC - 2026

## Platform build surfaces

The repository now exposes explicit per-OS build folders under `build/linux/`, `build/macos/`, and `build/windows/`. Each contains a README and Makefile dispatcher. The dispatchers call the existing platform-specific scripts rather than duplicating native source lists.

- Linux: GCC/G++ or Clang/Clang++, POSIX/Linux C/C++ footing.
- macOS: Apple Clang, Darwin/POSIX C/C++ footing.
- Windows 10+: MinGW-w64/GCC/G++, Win32 C/C++ footing; PowerShell build entry point.

The core runtime, debugger, Skya native layer, and Slecompiler now have explicit platform build entry points where their platform-specific implementation exists. Platform-specific API controllers are also present for database and webserver services where required.

## Three-platform build contract

The top-level `build/` surface now has explicit Linux, macOS, and Windows 10+ entry points:

| Platform | Build folder | Makefile | Primary scripts | Native footing |
|---|---|---|---|---|
| Linux | `build/linux/` | yes | `scripts/build-linux.sh`, Skya and Slecompiler Linux scripts | POSIX C/C++ |
| macOS | `build/macos/` | yes | `scripts/build-macos.sh`, Skya and Slecompiler macOS scripts | Darwin/POSIX C/C++ with Apple Clang |
| Windows 10+ | `build/windows/` | yes | `build-windows.ps1`, Skya and Slecompiler Windows PowerShell scripts | Win32 C/C++ with MinGW-w64 |

These platform dispatchers are documentation/build entry points; subsystem Makefiles remain authoritative. The platform-specific native scripts produce their own documented outputs and do not replace the underlying C/C++ implementations.
