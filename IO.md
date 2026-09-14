# SLeeLa OS-Aware I/O

SLeeLa provides a portable file and pipe vocabulary while selecting native
Linux or Windows transport at the I/O boundary.

## Targets

**Linux**

- Anonymous pipes use POSIX `pipe(2)`.
- Named filesystem IPC uses FIFOs via `mkfifo(3)`.
- Regular files use POSIX file descriptors.

**Windows**

- Anonymous pipes use Win32 `CreatePipe`.
- Named IPC uses Win32 named pipes in the `\\.\pipe\...` namespace and the
  Windows NPFS implementation.
- Regular files use Win32 `HANDLE` objects.

## Source compatibility

The SLeeLa operations are intentionally host-neutral:

```text
pipe()
pipePeer(handle)
fifoCreate(path, mode)
npfsCreate(path, mode)
openFile(path, mode)
read(handle)
write(handle, data)
close(handle)
unlinkFile(path)
```

Programs should not depend on Linux descriptor numbers or Windows `HANDLE`
values. SLeeLa keeps those values private and exposes VM-local handles.

## Selecting the host

The backend detects the native operating system automatically. Use:

```text
SLEELA_IO_PLATFORM=auto
```

for normal operation. `linux` and `windows` can be used to declare an intended
target; a non-native request is rejected rather than silently emulated.

The implementation lives in [`impl/core/sleela_io.h`](impl/core/sleela_io.h)
and [`impl/core/sleela_io.c`](impl/core/sleela_io.c). The dedicated smoke test
is `make test-io-platform` from `impl/`.

See [`impl/FILEIO.md`](impl/FILEIO.md) for API details and lifecycle semantics.
