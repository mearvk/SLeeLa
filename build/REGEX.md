# SLeeLa Regex Build

Version: 1.0.0-dev

The regex subsystem is built from its authoritative native implementation in
`make/regex/`. The SLeeLa library objects under `lib/regex/` are the language
surface and object model; they are not a second native implementation.

## Local build

From the repository root:

    make -C make/regex test

C only:

    make -C make/regex c

C++ only:

    make -C make/regex cpp

Clean:

    make -C make/regex clean

The generated binaries are disposable and remain under:

    make/regex/build/

## Toolchains

Linux and macOS use the host C/C++ compiler. The C implementation requires a
POSIX ERE implementation through `regex.h`; the C++ implementation uses
`std::regex`.

Windows builds should use a toolchain providing the required C regex
compatibility layer for the C target. The C++ target uses the standard library
regex implementation.

## Build boundary

- Native source of truth: `make/regex/src/`
- Public native interfaces: `make/regex/include/`
- Native tests: `make/regex/tests/`
- SLeeLa language objects: `lib/regex/`
- Disposable output: `make/regex/build/`

Do not copy the native implementation into `lib/regex/`; that directory
remains the SLeeLa-language representation and integration surface.

## CI integration

The regex build should be invoked as a focused native conformance target in CI:

    make -C make/regex test

The test target covers compilation, full matching, searching, captures,
replacement, splitting, escaping, and invalid-pattern diagnostics.

SLeeLa — MEARVK LLC — 2026
