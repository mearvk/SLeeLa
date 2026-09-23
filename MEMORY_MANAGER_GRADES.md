# SLeeLa Memory Manager — Three Implementation Grades

## Grade I — Process Memory Manager

This is the existing `slmm_*` implementation in `impl/core/sleela_memmgr.c/.h`.

It provides exact allocation-byte accounting, live/peak statistics, hard limits, thread-safe process-wide accounting, refusal on limit/OOM/overflow, and native-launch integration.

## Grade II — Multi-Process JVM Session Manager

Grade II adds a session layer for one JVM/SLeeLa start that launches **one through three child processes**.

It provides:
- maximum three tracked child-process slots;
- a per-process memory ceiling;
- an aggregate session ceiling;
- per-process live/peak bytes;
- allocation/free/refusal counters;
- invariant validation;
- clean process-close accounting.

Implementation:
- `impl/core/sleela_memmgr_grade2.h`
- `impl/core/sleela_memmgr_grade2.c`

Topology:

`JVM/SLeeLa -> process-1 + process-2 + process-3`

Grade II does not pretend to intercept arbitrary host OS calls. It is the process/session layer above Grade I.

## Grade III — Cross-Platform OS Resource Manager

Grade III expands the accounting envelope to the principal OS-facing resource families already represented by SLeeLa's abstraction layer:

- memory;
- files;
- sockets;
- processes;
- threads;
- dynamic libraries;
- terminals/PTYs;
- generic OS-backed resources.

Implementation:
- `impl/core/sleela_memmgr_grade3.h`
- `impl/core/sleela_memmgr_grade3.c`

Supported platform identity:
- Linux;
- Windows 10+;
- macOS.

The existing OS abstraction remains authoritative for actually making system calls. Grade III is intended to be connected at those abstraction boundaries rather than replacing libc, POSIX, Win32, or macOS system frameworks.

### Meaning of "majority of OS calls"

Grade III targets the **majority of the OS-facing calls made through SLeeLa's own platform abstraction layer**, not literal interception of every kernel/system call made by every library on the host.

The resource families map directly to existing SLeeLa boundaries:
`sleela_memory`, `sleela_io`, `sleela_net`, `sleela_thread`, `sleela_library`, `sleela_terminal`, `sleela_path`, and process launching.

## Layering

`Grade III OS resource envelope`
→ `Grade II JVM/process session accounting`
→ `Grade I exact allocation accounting`
→ `sleela_memory OS allocator abstraction`

Grades are additive. Grade II does not replace Grade I, and Grade III does not replace either lower grade.

## Integration requirements

The next integration phase should connect Grade II to the native process launcher and Grade III to the existing I/O, network, thread, library, terminal/path and process boundaries.

Each boundary should:
1. reserve resources before the operation;
2. release resources after completion;
3. count failures;
4. expose per-process and per-resource statistics;
5. run Linux, Windows 10+, and macOS smoke tests;
6. prevent accidental bypass of the accounting boundary.

The current files establish the three-grade implementation contracts without claiming that Grade III already intercepts every OS operation.
