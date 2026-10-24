# Lesson 1 — What Is Ballum?

Ballum is the SLeeLa display and pointer-input interface. Its intended surface includes display discovery, pixel-level writes where supported, whole-display controls, and mouse/touch event polling.

## Three layers

1. **SLeeLa facade** — convenient classes, argument checks, capability gates, and readable status values under `lib/ballum/`.
2. **VM bridge** — the `uiBallum*` functions exposed to SLeeLa programs.
3. **Native backend** — platform-specific code that talks to the operating system and reports what it can actually do.

A SLeeLa method returning a valid status is meaningful only if it is the status returned by the bridge/backend. A demonstration that binds a made-up handle or changes a local model is not evidence that the physical display changed.

## First run

Start with [the headless-safe example](../examples/05-headless-safe.sleela), then run [the screen inspection example](../examples/01-screen-inspection.sleela). These should help establish whether the host has a usable display backend without intentionally changing display settings.

## Learning objective

Be able to explain the difference between API validation, bridge availability, and a successful operating-system operation.
