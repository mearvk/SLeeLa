# OS Level Support

This directory contains a small, portable C API and C++ wrapper for OS-level capabilities that complement (rather than duplicate) the SLeeLa standard OS model. It targets Linux, Windows 10+, and macOS.

## Included

- Read-only host and current-process facts: platform, PID, hostname, user name, working directory, wall-clock time, and monotonic clock.
- Cron-style five-field schedule parsing and calculation of the next matching local-time minute. This is a pure calculation; it does not install a cron job, create a service, or launch a process.
- A C++ value-type wrapper over the C API.

These are deliberately user-mode operations. The library does not expose raw syscall dispatch, syscall interception/hooking, kernel memory access, kernel-object manipulation, or driver/kernel-module load and unload. Those are privileged, platform-specific security boundaries and are not safe portable library features. For authorized diagnostics, use documented OS tracing and driver-management tools with their normal administrative controls.

Registry and similar configuration stores are not implemented here: the platform semantics, permissions, and persistence policy need a separate explicit API. This library never changes system configuration or creates persistence.

## Build

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

The API uses the process local timezone for cron calculations. Callers that need a named timezone should convert using a timezone-aware library before/after calling this API; timezone database handling is outside this small portable layer. The next-time search is bounded to five years. Build and run tests on each target OS before shipping.

## API

- `osls_get_system_info` gathers the documented read-only snapshot.
- `osls_cron_validate` validates a five-field expression (`minute hour day-of-month month day-of-week`).
- `osls_cron_next_after` finds the next matching local-time minute strictly after the supplied epoch time.

Cron day-of-week accepts `0` or `7` for Sunday. As in common cron implementations, day-of-month and day-of-week are ANDed when either field is a wildcard; when both are restricted, either may match.
