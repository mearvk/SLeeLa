# SLeeLa OS-Aware File I/O, Pipes, FIFOs, and NPFS

## Overview

SLeeLa 1.1 exposes one source-level file-I/O API while selecting the native
host transport at the operating-system boundary.

- **Linux/POSIX:** ordinary files, anonymous `pipe(2)` pipes, and filesystem
  FIFOs created with `mkfifo(3)`.
- **Windows:** ordinary files through Win32 file handles, anonymous pipes
  through `CreatePipe`, and named pipes through the Windows `\\.\pipe\...`
  namespace backed by the Windows named-pipe/NPFS implementation.

The implementation keeps raw host handles inside the native backend. SLeeLa
programs receive bounded VM-local integer handles, so bytecode does not depend
on Linux descriptor numbers or Windows `HANDLE` values.

## Platform selection

The host backend is selected automatically from the native build target.
`SLEELA_IO_PLATFORM=auto` is the normal setting.

For an explicit deployment choice, set:

```text
SLEELA_IO_PLATFORM=linux
SLEELA_IO_PLATFORM=windows
```

An explicit request for a non-native platform is rejected rather than silently
emulated. Cross-compiling therefore requires building SLeeLa with the
corresponding Windows or Linux target toolchain.

The native platform is also exposed by the C backend as `slio_platform()` and
`slio_platform_name()`.

## Common built-ins

| Built-in | Linux | Windows |
|---|---|---|
| `pipe()` | anonymous POSIX pipe | anonymous Win32 pipe |
| `pipePeer(handle)` | paired pipe endpoint | paired pipe endpoint |
| `fifoCreate(path, mode)` | filesystem FIFO via `mkfifo` | named-pipe endpoint via Win32 named-pipe APIs |
| `npfsCreate(path, mode)` | compatibility spelling for FIFO | Windows named-pipe/NPFS endpoint |
| `openFile(path, mode)` | `open(2)` | `CreateFile` |
| `read(handle)` | `read(2)` | `ReadFile` |
| `write(handle, data)` | `write(2)` | `WriteFile` |
| `close(handle)` | `close(2)` | `CloseHandle` |
| `unlinkFile(path)` | `unlink(2)` | named-pipe names disappear with their server instances; ordinary file deletion remains a Win32 concern |

The SLeeLa language API remains stable while the backend changes with the
host operating system.

## Anonymous pipe example

```java
#sleela 1.1

class FileIO {
    int readEnd;
    int writeEnd;

    void writer() {
        write(writeEnd, "hello from Sleela pipe");
        close(writeEnd);
    }

    void main() {
        readEnd = pipe();
        writeEnd = pipePeer(readEnd);
        spawn(writer);
        print(read(readEnd));
        close(readEnd);
        join();
    }
}
```

The same SLeeLa source can use this interface on both supported hosts.

## Named pipe / FIFO example

```java
#sleela 1.1

class ChannelIO {
    int channel;

    void main() {
        npfsCreate("/tmp/sleela-channel", 438);
        channel = openFile("/tmp/sleela-channel", "rw");
        write(channel, "hello from Sleela");
        close(channel);
        unlinkFile("/tmp/sleela-channel");
    }
}
```

On Linux, the pathname is a FIFO pathname. On Windows, deployments should use
a named-pipe pathname such as `\\.\pipe\sleela-channel`.

## NPFS terminology

**NPFS is Windows terminology.** Windows named pipes are implemented through
the Windows named-pipe APIs and exposed through the `\\.\pipe\...` namespace.
Linux does not have an NPFS filesystem; its corresponding filesystem-visible
IPC primitive is a POSIX FIFO.

SLeeLa deliberately keeps `npfsCreate(...)` as a portable source-level name.
The OS backend determines whether that request becomes a Linux FIFO or a
Windows named pipe.

## File modes

The common mode strings are `r`, `w`, `a`, `rw`, `r+`, `w+`, and `a+`.
Appending `n` requests nonblocking behavior where the host backend supports it.
Linux descriptors use `O_CLOEXEC`; Windows handles are native `HANDLE` values
kept private to the backend.

## Safety and lifecycle

- Raw host descriptors/handles remain private to the C core.
- VM handles are bounded by `SL_MAX_FILES` (256).
- Active file handles are closed during VM destruction.
- Broken Linux pipe writes are prevented from terminating the process by the
  existing `SIGPIPE` policy.
- A platform mismatch is rejected instead of silently emulated.
- Blocking behavior is delegated to the native OS pipe/FIFO/named-pipe API.

## Build and diagnostics

The verified Makefile now builds `core/sleela_io.c` and includes an
`io-platform` smoke test. The test reports the native platform and verifies an
anonymous pipe round trip.

```text
make test-io-platform
SLEELA_IO_PLATFORM=auto make test-io-platform
```

The existing SHA-256 verification barrier remains before compilation and
execution.
