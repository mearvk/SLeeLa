# SLeeLa Testing

## Test layers

1. Lexer/parser tests.
2. Semantic/type tests.
3. Compiler/code-generation tests.
4. Core VM tests.
5. ABI tests.
6. Runtime/resource tests.
7. Standard-library tests.
8. Subject-library numeric tests.
9. Protocol tests.
10. Segmentation/reassembly tests.
11. Security/failure tests.
12. Platform tests.
13. Integration tests.
14. Packaging/install tests.

## Standalone translation-unit audit

In addition to the layers above, `test-suites/run-all.sh --audit` (and
`--headers`) compiles every SLeeLa-authored translation unit on its own under
the strict warning set (`-Wall -Wextra -Wpedantic -Wconversion -Wshadow
-Wformat=2 -Werror`) to catch self-inconsistent files — duplicate type
definitions, wrong struct-member names, missing includes, and type errors —
before they reach a link. Each unit is compiled with the same include roots the
per-module Makefiles use (its own directory and module `include/`/`src/` dirs
first, then the shared repository roots). Only committed source is audited;
build output and files that require a foreign OS SDK (macOS CoreAudio, the
Windows SDK, GTK) are excluded or reported as SKIP. See
[`test-suites/README.md`](test-suites/README.md) for details.

## Negative testing

Every public parser, decoder, resource allocator and protocol boundary should have malformed-input tests.

## Fuzzing

The compiler front end, XML/BODI parsers, binary decoders, HTTP frame decoder and package manifest parser should be fuzz targets.

## Concurrency

Test deadlock, duplicate delivery, lock contention, cancellation and mailbox exhaustion.

## Reproducibility

Release CI should rebuild from a clean environment and compare declared artifact hashes.

## Required gate

A feature is not considered release-ready merely because a happy-path example works. It should have implementation, positive tests, negative tests, documentation and platform coverage appropriate to its scope.

**Max Rupplin — MEARVK LLC — 2026**
