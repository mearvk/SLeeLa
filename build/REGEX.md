# SLeeLa Regex Build

Version: 1.0.0-dev

The regex subsystem is built from its authoritative native implementation in
`regex/`. The SLeeLa library objects under `lib/regex/` are the language
surface and object model; they are not a second native implementation.

## Local build

From the repository root:

    make -C regex test

C only:

    make -C regex c

C++ only:

    make -C regex cpp

Clean:

    make -C regex clean

The generated binaries are disposable and remain under:

    regex/build/

## Toolchains

Linux and macOS use the host C/C++ compiler. The C implementation requires a
POSIX ERE implementation through `regex.h`; the C++ implementation uses
`std::regex`.

Windows builds should use a toolchain providing the required C regex
compatibility layer for the C target. The C++ target uses the standard library
regex implementation.

## Build boundary

- Native source of truth: `regex/src/`
- Public native interfaces: `regex/include/`
- Native tests: `regex/tests/`
- SLeeLa language objects: `lib/regex/`
- Disposable output: `regex/build/`

Do not copy the native implementation into `lib/regex/`; that directory
remains the SLeeLa-language representation and integration surface.

## CI integration

The regex build should be invoked as a focused native conformance target in CI:

    make -C regex test

The test target covers compilation, full matching, searching, captures,
replacement, splitting, escaping, and invalid-pattern diagnostics.

SLeeLa — MEARVK LLC — 2026
