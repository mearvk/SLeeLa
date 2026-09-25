# SLeeLa Product Build Appropriation

The `build/` tree is the native product-build boundary for SLeeLa. Product builds remain separate from source, generated build trees, caches, and runtime state.

## Products

### Slecompiler™
Source: `decompiler/`

Purpose:
- Native artifact inspection and static analysis.
- ELF, GNU archive, PE/COFF and raw-artifact analysis.
- x86/x86-64 recovery foundation.
- Shared-library and kernel-module metadata recovery.
- Read-only, non-executing analysis.

Platform entry points:
- Linux: `build/slecompiler-linux.sh`
- macOS: `build/slecompiler-macos.sh`
- Windows 10+: `build/slecompiler-windows.ps1`

Build output is placed under:
- `build/slecompiler/linux/`
- `build/slecompiler/macos/`
- `build/slecompiler/windows/`

The scripts configure and build the existing `decompiler/CMakeLists.txt`. They do not replace or regenerate the Slecompiler source tree.

### Slecompiler™ API exemplars

The decompiler CMake project provides focused executable exemplars for the public analysis surface:

| Exemplar | Demonstrates |
|---|---|
| `slecompiler-api-inspect` | Artifact opening, identification, and metadata inspection |
| `slecompiler-api-cfg` | Decode-oriented analysis and control-flow graph work |
| `slecompiler-api-library` | Library/archive metadata inspection |
| `slecompiler-api-graph` | Library-family dependency graph analysis |
| `slecompiler-api-slir-vm` | SLIR/lifted representation and VM-oriented analysis |

These examples are deliberately small entry points for application developers. They are
documentation-by-execution: each should remain buildable with the same CMake target
used by the product library.

### Skya
Source: `telephony-skya/native/`

Existing platform entry points:
- `build/skya-linux.sh`
- `build/skya-macos.sh`
- `build/skya-windows.ps1`

## Build boundary

Each product receives:
1. Its own source-of-truth directory.
2. Platform-specific build entry points.
3. A product-specific output directory.
4. No execution of analyzed native artifacts as part of Slecompiler builds.
5. A path for later packaging/signing without mixing package output into source.

## Documentation boundary

Slecompiler documentation is split by purpose:
- `decompiler/README.md` — product scope and safety boundary.
- `decompiler/API.md` — API concepts and exemplar map.
- `decompiler/TUTORIAL.md` — developer tutorial from artifact input to analysis/report.
- `build/PRODUCTS.md` — product build and platform boundary.
- `TERMINOLOGY.md` — shared vocabulary and evidence semantics.

The tutorial and exemplars are intentionally aligned with the CMake targets rather
than inventing a second build system.

## Future product build layers

The build appropriation is intentionally extensible for:
- SLeeLa core/server editions.
- HTTP server editions.
- Telephony/driver products.
- Native SDK/API packages.
- Analysis libraries and command-line tools.
- Cross-platform installers and signed release bundles.

Generated build trees are disposable. Product source remains authoritative in its product directory.

## Windows 10+ and macOS portability

Slecompiler's build boundary is explicitly cross-platform:

### Windows 10+
- CMake 3.20+ is the build-system baseline.
- Visual Studio generators may select x64, Win32, or ARM64 explicitly.
- MinGW-w64 or another CMake-supported compiler may be used when the selected generator supports it.
- The Windows build script does not require POSIX shell tools.
- PowerShell 5.1+ is sufficient for the build script on supported Windows installations.
- The product is native C++20 and does not depend on Linux-only headers, ELF runtime libraries, or POSIX execution APIs for the core build.

Example with Visual Studio 2022:

    powershell -ExecutionPolicy Bypass -File .\\build\\slecompiler-windows.ps1 -Generator "Visual Studio 17 2022" -Architecture x64

### macOS
- Xcode Command Line Tools are required.
- CMake 3.20+ is the build-system baseline.
- The build script detects the host architecture and permits an explicit CMAKE_OSX_ARCHITECTURES override.
- MACOSX_DEPLOYMENT_TARGET defaults to 11.0 and can be overridden for the supported deployment range.
- Universal binaries can be requested through CMAKE_OSX_ARCHITECTURES="x86_64;arm64" when the local toolchain supports both architectures.

The portability boundary applies to the build system as well as the product source:
platform-specific build mechanics stay in build/, while Slecompiler source remains
authoritative under decompiler/.
