# SLeeLa OS-Aware Threading

SLeeLa now has a platform-neutral threading backend in `impl/core/sleela_thread.h` and `impl/core/sleela_thread.c`.

## Supported backends

- **Linux/POSIX** — maps to POSIX threads, mutexes, and condition variables.
- **Windows** — maps to Win32 threads, critical sections, and condition variables.

The public abstraction is intentionally small:

- `SLThreadHandle`
- `SLThreadMutex`
- `SLThreadCond`
- `slthread_create()` / `slthread_join()`
- `slthread_mutex_*()`
- `slthread_cond_*()`
- `slthread_platform()` / `slthread_platform_name()`

This prevents the VM's threading contract from requiring Linux-only `pthread_*` types.

## Platform smoke test

`impl/core/thread_platform_smoke.c` exercises the backend through the abstraction and reports the native backend name. It verifies thread creation/join, mutex operations, and condition-variable broadcast.

Example Linux build from `impl/`:

```sh
gcc -std=c11 -Wall -Wextra -pthread -Icore \
  core/sleela_thread.c core/thread_platform_smoke.c \
  -o /tmp/sleela-thread-platform-smoke
/tmp/sleela-thread-platform-smoke
```

## VM migration status

The existing VM still contains direct `pthread_*` calls in `sleela_core.c`. The new backend is therefore the portability foundation rather than a claim that the entire VM is already Windows-native.

The next threading migration step is to replace VM mutex, condition-variable, thread-create, and thread-join calls with the `sleela_thread.*` interface, then remove the direct POSIX threading dependency from the VM source and build rules.

## Security/build relationship

The existing SHA-256 verification gate remains the required pre-build/pre-test control. Threading changes must pass the same verification gate before compilation or diagnostics are executed.
