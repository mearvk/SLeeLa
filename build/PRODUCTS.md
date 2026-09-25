# SLeeLa Product Build Appropriation

The build/ tree is the native product-build boundary for SLeeLa. Product builds remain separate from source, generated build trees, caches, and runtime state.

## Products

### Slecompiler™
Source: decompiler/

Purpose:
- Native artifact inspection and static analysis.
- ELF, GNU archive, PE/COFF and raw-artifact analysis.
- x86/x86-64 recovery foundation.
- Shared-library and kernel-module metadata recovery.
- Read-only, non-executing analysis.

Platform entry points:
- Linux: build/slecompiler-linux.sh
- macOS: build/slecompiler-macos.sh
- Windows 10+: build/slecompiler-windows.ps1

Build output is placed under:
- build/slecompiler/linux/
- build/slecompiler/macos/
- build/slecompiler/windows/

The scripts configure and build the existing decompiler/CMakeLists.txt. They do not replace or regenerate the Slecompiler source tree.

### Skya
Source: telephony-skya/native/

Existing platform entry points:
- build/skya-linux.sh
- build/skya-macos.sh
- build/skya-windows.ps1

## Build boundary

Each product receives:
1. Its own source-of-truth directory.
2. Platform-specific build entry points.
3. A product-specific output directory.
4. No execution of analyzed native artifacts as part of Slecompiler builds.
5. A path for later packaging/signing without mixing package output into source.

## Future product build layers

The build appropriation is intentionally extensible for:
- SLeeLa core/server editions.
- HTTP server editions.
- Telephony/driver products.
- Native SDK/API packages.
- Analysis libraries and command-line tools.
- Cross-platform installers and signed release bundles.

Generated build trees are disposable. Product source remains authoritative in its product directory.
