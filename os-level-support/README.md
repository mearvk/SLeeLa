# OS Level Support

This directory contains a small, portable C API and C++ wrapper for OS-level
capabilities that complement (rather than duplicate) the SLeeLa standard OS
model. It targets Linux, Windows 10+, and macOS.

## Included

- Read-only host and current-process facts: platform, PID, hostname, user name,
  working directory, wall-clock time, and monotonic clock.
- Cron-style five-field schedule parsing and calculation of the next matching
  local-time minute. This is a pure calculation; it does not install a cron job,
  create a service, or launch a process.
- A C++ value-type wrapper over the C API.
- The C/C++ native module loader in [`loader/`](loader/README.md), for trusted
  shared libraries built from up to nine linked C/C++ source/object units.

These are deliberately user-mode operations. The system-information and cron
APIs do not change host configuration. The native loader is an execution
boundary, not a sandbox: loading a library executes native code in the current
process, so only trusted modules should be loaded. It does not interpret C/C++
source, load relocatable object files directly, or compile caller-provided code
at runtime.

Registry and similar configuration stores are not implemented here: the
platform semantics, permissions, and persistence policy need a separate
explicit API. This library never changes system configuration.

## Build

```sh
cmake -S os-level-support -B build/os-level-support
cmake --build build/os-level-support
ctest --test-dir build/os-level-support --output-on-failure
```

The API uses the process local timezone for cron calculations. Callers that
need a named timezone should convert using a timezone-aware library; timezone
database handling is outside this API.

## API

- `osls_get_system_info` gathers the documented read-only snapshot.
- `osls_cron_validate` validates a five-field expression (`minute hour day-of-month month day-of-week`).
- `osls_cron_next_after` finds the next matching local-time minute strictly after the supplied epoch time.

Cron day-of-week accepts `0` or `7` for Sunday. As in common cron
implementations, day-of-month and day-of-week are ANDed when either field is a
wildcard; when both are restricted, either may match.
