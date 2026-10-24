# Ballum Tutorials

A progressive, practical learning series for Ballum in SLeeLa. Read in order; each lesson introduces one layer and links to runnable code in [../examples/](../examples/).

## Lessons

1. [What Ballum Does](01-what-is-ballum.md) — architecture and honest capability reporting.
2. [Discover Displays](02-discover-displays.md) — enumerate displays and inspect capabilities.
3. [Pixel Coordinates and Bounds](03-pixels-and-bounds.md) — coordinate conventions and safe writes.
4. [Display Controls](04-display-controls.md) — brightness, warmth, and gamma ranges.
5. [Mouse and Touch Input](05-pointer-input.md) — bounded polling and gesture interpretation.
6. [Status Codes and Testing](06-status-and-testing.md) — failures, headless testing, and validation.
7. [Native Bridge and Platform Limits](07-native-bridge.md) — what the SLeeLa facade does and what the native host must provide.

## Prerequisites

- Familiarity with classes, methods, conditionals, and loops in SLeeLa 2.1.
- A SLeeLa compiler/runtime that supports the version declared by the source.
- For live hardware behavior, a compatible native Ballum backend and VM bridge.

## Important distinction

The SLeeLa model/demo files can teach API shape and validation. Only programs wired through the real `uiBallum*` VM bridge to a functioning native backend can perform actual display I/O. A headless or unsupported host should report that fact rather than simulate success.

See the [Ballum package README](../README.md) for API and platform details.
