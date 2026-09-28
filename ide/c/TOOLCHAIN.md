# C IDE Support

Version: 0.2.0-dev

C support is provided as a native-toolchain integration layer. The IDE does not implement a second C compiler.

## Supported source
- `.c` translation units
- `.h` headers
- C11 as the repository baseline unless a project explicitly selects another standard

## Toolchain discovery
The project records the selected C compiler as `CC`.
Candidates: clang, gcc, or an explicitly configured compiler path.
The adapter obtains include paths, predefined macros, language standard, target triple, sysroot, and warning configuration from the project's compile configuration.

## Compilation database
If `compile_commands.json` exists, it is the preferred source for per-file C compilation arguments.

## SLeeLa interoperability
C declarations referenced through the SLeeLa native bridge are indexed as external/native symbols. ABI ownership remains in the SLeeLa native implementation; the IDE only presents and navigates it.

**Max Rupplin — MEARVK LLC — 2026**