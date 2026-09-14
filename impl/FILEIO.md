# Sleela Linux File I/O, Pipes, FIFOs, and NPFS Compatibility

## Overview

Sleela 1.1 now exposes a Linux/POSIX-oriented file-descriptor layer to the
C/C++ execution core. The layer supports ordinary file descriptors, anonymous
Linux pipes, filesystem FIFOs (named pipes), and an `npfsCreate(...)`
compatibility spelling for applications that want to describe a named-pipe
endpoint using NPFS terminology.

The implementation deliberately keeps raw operating-system file descriptors
inside the VM. Sleela programs receive bounded VM-local integer handles instead.
This keeps the bytecode interface stable and prevents a Sleela program from
assuming that a host descriptor number has a particular value.

## Built-ins

| Built-in | Result / behavior |
|---|---|
| `pipe()` | Creates an anonymous Linux pipe and returns its read-end handle. |
| `pipePeer(handle)` | Returns the write-end handle associated with a pipe handle. |
| `fifoCreate(path, mode)` | Creates a filesystem FIFO using Linux `mkfifo(3)`; returns `0` or `-1`. |
| `npfsCreate(path, mode)` | Linux compatibility spelling for `fifoCreate`; it creates a POSIX FIFO, not a Windows NPFS mount. |
| `openFile(path, mode)` | Opens a regular file or FIFO and returns a VM-local handle. |
| `read(handle)` | Reads a byte stream and returns a Sleela `String`; EOF/no data returns `""`. |
| `write(handle, data)` | Writes a Sleela `String`; returns the byte count or `-1`. |
| `close(handle)` | Closes a VM-local descriptor handle. |
| `unlinkFile(path)` | Removes the filesystem name; open descriptors may continue to operate. |

Supported `openFile` modes include `r`, `w`, `a`, `rw`, `r+`, `w+`, and `a+`.
Appending `n` requests `O_NONBLOCK`, for example `rn` or `rwn`.
Descriptors are opened with `O_CLOEXEC`.

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

The `pipe()`/`pipePeer()` model follows Linux `pipe(2)`: one end reads and the
other writes. Closing the final write end allows a reader to observe EOF.

## FIFO example

```java
#sleela 1.1

class FifoIO {
    int channel;

    void main() {
        fifoCreate("/tmp/sleela-fifo-demo", 438);
        channel = openFile("/tmp/sleela-fifo-demo", "rw");
        write(channel, "hello from Sleela FIFO");
        print(read(channel));
        close(channel);
        unlinkFile("/tmp/sleela-fifo-demo");
    }
}
```

On Linux, opening a FIFO with read/write access can be used to avoid the
otherwise normal two-sided open synchronization. Applications that need
portable POSIX semantics should use separate reader and writer handles and
coordinate their opens explicitly.

## NPFS terminology

**NPFS is a Windows named-pipe filesystem concept; it is not the Linux FIFO
implementation.** Windows named pipes are exposed through the Windows named
pipe APIs and the `\\.\pipe\...` namespace. On Linux, the corresponding
filesystem-visible primitive is a FIFO created with `mkfifo(3)`.

Sleela therefore treats `npfsCreate(path, mode)` as a source-level
compatibility alias for `fifoCreate(path, mode)` on the Linux core. This keeps
source terminology portable without falsely claiming that Linux has a Windows
NPFS filesystem.

A future Windows backend can map the same Sleela concepts to Windows named
pipe handles while preserving the higher-level `read`, `write`, and `close`
contract.

## Safety and lifecycle

- Raw host file descriptors remain private to the C core.
- VM handles are bounded by `SL_MAX_FILES` (256).
- The core closes active file handles during VM destruction.
- `O_CLOEXEC` is used when opening host files/FIFOs.
- `SIGPIPE` is ignored by the VM so a broken pipe produces an I/O failure
  rather than terminating the process unexpectedly.
- `unlinkFile` removes the filesystem name; an already-open FIFO remains usable
  until its open descriptors are closed.
- Blocking reads and writes retain Linux byte-stream semantics. Applications
  should use `n` modes when nonblocking behavior is required.

## Versioning

All file-I/O built-ins require `#sleela 1.1`. This keeps the feature behind the
existing syntax-version gate and makes older 1.0 Wrappers™ deterministic.

## References

Linux pipes are byte streams with separate read and write ends; FIFOs provide
the same I/O semantics while adding a filesystem pathname. The Linux manual
also specifies that FIFO data is carried by the kernel rather than stored as
ordinary file contents. See the Linux `pipe(7)`, `fifo(7)`, `pipe(2)`, and
`mkfifo(3)` interfaces.

Windows NPFS/named-pipe terminology is documented separately by Microsoft;
Sleela's Linux implementation uses the compatibility mapping described above.
