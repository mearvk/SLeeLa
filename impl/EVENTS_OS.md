# SLeeLa Signals / Events — OS Facility Contract

## Purpose

SLeeLa needs one runtime-facing event contract so signal notifications and application events do not require platform-specific code in the VM.

## Backends

- Linux/POSIX: `signal()` plus a mutex-protected runtime event queue.
- Windows: Win32 events plus the same runtime event queue contract.

## Contract

`impl/core/sleela_events.h` provides:

- platform identification;
- signal installation, pending-state inspection, and clearing;
- manual-reset event creation, set, reset, wait, and close;
- a bounded runtime event queue for type/code/value notifications.

The queue is deliberately bounded. Producers receive `EAGAIN` when it is full rather than silently losing an event.

## Portability boundary

Runtime code should use `sleela_events.h` rather than directly calling Win32 event APIs or POSIX signal/event primitives. This keeps platform decisions in one implementation boundary and complements the existing threading, I/O, networking, path, terminal, native-library, and memory facilities.

## Security

Signals and events are notification mechanisms, not trust mechanisms. Existing SLeeLa SHA-256 verification remains the authoritative pre-build and pre-execution integrity gate.

## Smoke test

`core/events_platform_smoke.c` verifies event lifecycle, timeout behavior, queue delivery, and (where supported) signal notification.
