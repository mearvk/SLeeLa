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
