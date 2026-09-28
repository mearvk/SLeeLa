# SLeeLa File I/O Subsystem

SLeeLa 1.1 provides a common file and pipe API with an OS-aware native
backend. The source-level operations remain stable while the implementation
uses the native transport of the host operating system.

## Platform backends

- **Linux/POSIX:** regular files, anonymous `pipe(2)` pipes, and filesystem
  FIFOs via `mkfifo(3)`.
- **Windows:** regular files through Win32 handles, anonymous pipes through
  `CreatePipe`, and named pipes through the `\\.\pipe\...` namespace.

The Windows named-pipe implementation is the NPFS-side counterpart of the
Linux FIFO implementation. Linux does not expose a Windows NPFS filesystem.

## Primitives

- `pipe()` creates an anonymous native pipe.
- `pipePeer(handle)` obtains the paired endpoint.
- `fifoCreate(path, mode)` creates the native named-pipe/FIFO endpoint.
- `npfsCreate(path, mode)` uses the same portable source spelling for a
  Windows named pipe or Linux FIFO.
- `openFile(path, mode)` opens regular files or pipe endpoints.
- `read(handle)` and `write(handle, data)` perform byte-stream I/O.
- `close(handle)` closes the VM-owned handle.
- `unlinkFile(path)` removes a Linux FIFO pathname; Windows named-pipe names
  disappear with their server instances.

Raw host descriptors/handles never become Sleela values. The runtime assigns a
bounded VM-local handle, currently limited to 256 simultaneous file handles.

## Platform choice

The native platform is detected automatically. `SLEELA_IO_PLATFORM=auto` is
the default. `SLEELA_IO_PLATFORM=linux` or `SLEELA_IO_PLATFORM=windows` may
be used to declare the intended target; a non-native request is rejected rather
than silently emulated.

The C backend exposes `slio_platform()` and `slio_platform_name()` for
platform-aware diagnostics.

## Verification

The verified Makefile builds `core/sleela_io.c` and runs the dedicated
`test-io-platform` smoke test. The existing SHA-256 verification barrier still
runs before compilation and diagnostics.

See [`../FILEIO.md`](../FILEIO.md) for the full API, examples, lifecycle rules,
platform semantics, and the Linux-vs-Windows NPFS distinction.
