# OS Level Support

Portable C ABI with a thin C++ facade for read-only, user-mode host services that complement the SLeeLa standard OS model. Build with `cmake -S os-level-support -B build/osls`, then build and test using `cmake --build build/osls` and `ctest --test-dir build/osls --output-on-failure`.

Included APIs cover host facts and clocks, self-process metadata, current-process environment, filesystem metadata and directory names, CPU/memory, local timezone, network interface addresses, and cron parsing/next-time calculations. C++ `osls::Host`, `osls::SystemInfo`, and `osls::CronExpression` wrap the same status-returning C API.

Process inspection is limited to the current process. Platform-dependent support may return `OSLS_UNSUPPORTED`; unavailable fields may be empty or unknown. Windows network-interface enumeration is not implemented yet.

The library does not expose raw syscall invocation or hooks, kernel memory/object access, driver installation or unload, registry/configuration mutation, command execution, process creation, or automatic execution of built artifacts. The separate trusted native module loader executes loaded code in-process and is not a sandbox.
