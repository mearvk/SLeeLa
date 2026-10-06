# SLeeLa Runtime

## Runtime architecture

SLeeLa executes through a C core with a stable exchange ABI. The runtime owns execution state; the compiler owns source interpretation and lowering.

## Core

The current core provides:

- operand stack;
- constants;
- globals;
- locals;
- call frames;
- opcode dispatch;
- arithmetic and comparison;
- control flow;
- calls and returns;
- result retrieval;
- bounded threads;
- locks;
- message passing.

## Runtime services

The runtime boundary is intended to provide:

- memory accounting and limits;
- filesystem/process services through OS adapters;
- sockets and network operations;
- time;
- terminal/PTY;
- dynamic libraries;
- diagnostics;
- controlled native execution.

The host System Call API — environment, identity, working directory, filesystem
metadata, and process execution — is exposed to SLeeLa source through the `os*`
built-ins (`osRun`/`osSpawn`/`osGetEnv`/`osExists`/...), which lower to the
`OP_OS_*` opcodes and are serviced by `impl/core/sleela_os.c` on Windows, Linux,
and macOS. See [`SYSTEM_CALL_SURFACE.md`](SYSTEM_CALL_SURFACE.md),
[`/lib/os/OS.md`](../lib/os/OS.md), and [`/lib/vm/OPCODE-MAP.md`](../lib/vm/OPCODE-MAP.md).

## Concurrency

Thread, lock, join and mailbox operations are bounded. Resource exhaustion is an explicit failure state rather than an implicit unlimited allocation promise.

## Memory

The Memory Manager can impose a configured process-memory ceiling. Native and external resources remain subject to operating-system limits.

## Exceptions and failures

Runtime failures must carry an explicit failure result and must not corrupt VM state. Future exception facilities should preserve the same invariant.

## Async direction

Future asynchronous APIs should be implemented above the core exchange boundary, using explicit task/future state rather than hidden scheduler behavior.

## ABI stability

Runtime internals may evolve without changing the documented exchange contract unless the ABI version is intentionally advanced.

**Max Rupplin — MEARVK LLC — 2026**
