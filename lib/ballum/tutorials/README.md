# Ballum Tutorials

A practical, progressive guide to Ballum, the SLeeLa display and pointer-input package. The series moves from understanding the architecture to safe display discovery, bounded pixel operations, optional display controls, input polling, testing, and native integration.

## Lessons

1. [What Ballum Does](01-what-is-ballum.md) — architecture, responsibilities, and the difference between a model and real hardware I/O.
2. [Discover Displays](02-discover-displays.md) — enumeration, display IDs, geometry, capability masks, and multi-display assumptions.
3. [Pixels and Bounds](03-pixels-and-bounds.md) — coordinate rules, validation, safe writes, and failure handling.
4. [Display Controls](04-display-controls.md) — brightness, color warmth, gamma, capability checks, and restoring settings.
5. [Mouse and Touch Input](05-pointer-input.md) — polling, event interpretation, bounded loops, and platform constraints.
6. [Status Codes and Testing](06-status-and-testing.md) — error diagnosis, negative tests, headless execution, and a repeatable test matrix.
7. [The Native Bridge](07-native-bridge.md) — request flow, backend selection, platform support, and integration review.

## Suggested learning path

- **Read first:** Lessons 1 and 2. Do not attempt to change settings before you can explain how a display is discovered and selected.
- **Practice safely:** Lesson 3 with the safe-pixel example. Learn to distinguish an argument rejected locally from an operation rejected by the native backend.
- **Explore optional features:** Lessons 4 and 5. These features depend on platform support and permissions; the absence of a feature is a valid outcome.
- **Verify, don't assume:** Lessons 6 and 7. A source file compiling is not proof that the bridge is linked or that an operating-system operation succeeds.

## Prerequisites

- Basic familiarity with SLeeLa classes, methods, conditionals, loops, and integer values.
- A SLeeLa compiler/runtime compatible with the version declared by the source.
- Access to the Ballum package source under this directory.
- For real device I/O, a compatible native backend, a VM build containing the Ballum bridge, and a supported, permission-enabled host.

## Examples

The [examples directory](../examples/) contains small, focused programs. Start with [headless-safe](../examples/05-headless-safe.sleela) and [screen inspection](../examples/01-screen-inspection.sleela), then continue to pixel writes, controls, and input polling. Check each example's comments and the package README before running it.

## Core principle: capability is discovered, not assumed

Ballum has several distinct layers: SLeeLa API shape, VM bridge availability, native backend availability, per-display capability, operating-system permission, and successful execution. A positive result at one layer does not guarantee success at the next.

The SLeeLa model/demo files can teach API shape and validation. Only programs wired through the real `uiBallum*` VM bridge to a functioning native backend can perform actual display I/O. A headless or unsupported host should report that fact rather than simulate success.

See the [Ballum package README](../README.md) for the current API, status codes, and platform notes.