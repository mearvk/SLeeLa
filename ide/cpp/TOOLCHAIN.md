# C++ IDE Support

Version: 0.2.0-dev

C++ support is a native-toolchain integration layer. It does not replace the repository's C++ compiler or the host IDE's C++ semantic engine.

## Supported source
- `.cpp`, `.cc`, `.cxx`
- `.hpp`, `.hh`, `.hxx`, and compatible headers

## Toolchain
The project may select clang++ or g++, or an explicit compiler path. C++ standard, target, sysroot, include paths, macros, language-server configuration, and linker settings belong to module configuration.

## Language-server boundary
Where a C++ language server is available, the IDE integration should consume its semantic results rather than maintaining a second C++ parser. `compile_commands.json` is the canonical compilation-argument source.

## SLeeLa integration
C++ symbols used by the SLeeLa implementation are exposed through navigation/index adapters. The IDE must preserve the distinction between SLeeLa source symbols and native C++ implementation symbols.

**Max Rupplin — MEARVK LLC — 2026**