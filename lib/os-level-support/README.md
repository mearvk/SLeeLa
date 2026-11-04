# SLeeLa OS Support Bindings

This package is a small C/C++ bridge-oriented foundation alongside the root `os-level-support/` native library.

The initial implementation provides bounded, in-process event subscriptions and a cooperative task queue. Events are dispatched only when the caller explicitly emits them; callbacks are local to this runtime and do not intercept operating-system calls. Tasks execute only when the caller pumps the queue; this component does not create processes or threads, launch commands, perform OS I/O, install persistent schedules, or modify system settings.

This is not yet an FFI binding into the SLeeLa VM. It does not expose kernel hooks, syscall interception, driver load/unload, privilege changes, or system configuration writes. The root native library remains the source for documented host metadata.

Build and test:

```sh
cmake -S lib/os-level-support -B build/lib-os-level-support
cmake --build build/lib-os-level-support
ctest --test-dir build/lib-os-level-support --output-on-failure
```
