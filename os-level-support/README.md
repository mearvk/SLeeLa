# OS Level Support

Portable C ABI with a thin C++ facade for read-only, user-mode host services that complement the SLeeLa standard OS model. Build with `cmake -S os-level-support -B build/osls`, then build and run `ctest --test-dir build/osls --output-on-failure`.

## Available C and C++ surfaces

- Host snapshot and monotonic/wall clocks (`osls_get_system_info`, `osls::SystemInfo`).
- Current-process metadata only (`osls_get_process_info`, `osls_enumerate_processes`). `pid == 0` means self; inspecting another process is explicitly unsupported. Enumeration intentionally returns only the caller's process.
- Current-process environment lookup/enumeration.
- Filesystem metadata and directory-name listing (no file contents, mutation, or traversal beyond the requested directory).
- CPU/memory, local timezone, and network-interface metadata. Unsupported platform capabilities return `OSLS_UNSUPPORTED`; fields not available from a portable API may be reported as `unknown`/empty.
- Cron expression validation and next-time calculation.

The C++ `osls::Host` methods wrap the same status-returning C API and retain the same access boundaries. Buffer/list outputs are caller-owned; do not assume fixed fields contain truncated strings.

These APIs do not perform raw syscall invocation, syscall hooks, kernel memory/object access, driver installation or unload, registry/configuration mutation, command execution, process creation, or automatic execution of compiled artifacts. The native plugin loader remains a separate boundary: loading trusted shared-library code executes inside the current process and is not a sandbox.

This is a portable baseline, not a claim to expose every OS service. The Windows network-interface enumerator currently returns `OSLS_UNSUPPORTED`; platform-specific features should be added behind documented APIs and tested on each OS rather than represented as universally available.
