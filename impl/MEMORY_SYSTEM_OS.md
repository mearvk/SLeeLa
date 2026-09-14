# SLeeLa Memory / System Facilities

## Purpose

SLeeLa now has a small operating-system boundary for memory allocation and basic system timing/page information. The interface is intended to keep runtime code from depending directly on platform-specific allocation and system calls as the runtime expands.

## Facilities

- Standard allocation: `slmemory_alloc`, `slmemory_calloc`, `slmemory_realloc`, `slmemory_free`.
- Aligned allocation: `slmemory_aligned_alloc` and `slmemory_aligned_free`.
- System page size: `slmemory_page_size`.
- Monotonic elapsed time in milliseconds: `slmemory_monotonic_millis`.
- Platform identification and availability checks.

## Backends

### Linux / POSIX

Uses the native C/POSIX allocation facilities, `sysconf(_SC_PAGESIZE)`, and `CLOCK_MONOTONIC`.

### Windows

Uses the native C runtime allocation facilities, `_aligned_malloc` / `_aligned_free`, `GetSystemInfo`, and `GetTickCount64`.

## Runtime policy

This layer is an operating-system abstraction, not a memory safety policy. Callers remain responsible for ownership, lifetime, bounds, and failure handling. It does not replace SLeeLa's existing SHA-256 verification gate, which remains required before build, testing, and diagnostics.

## Smoke test

`memory_platform_smoke.c` exercises ordinary allocation, zero allocation, reallocation, aligned allocation, page-size discovery, and monotonic time ordering.

The smoke test is designed to be compiled and run on each supported host once the implementation is integrated into the platform build matrix.
