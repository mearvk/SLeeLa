# SLeeLa File I/O

SLeeLa 1.1 adds a Linux/POSIX file-I/O surface to the C/C++ execution core.
Programs can now use anonymous Linux pipes, filesystem FIFOs (named pipes),
regular file descriptors, and a clearly documented NPFS compatibility name.

## Surface

```text
pipe()                         -> read-end VM handle
pipePeer(readHandle)           -> write-end VM handle
fifoCreate(path, mode)         -> 0 / -1
npfsCreate(path, mode)         -> Linux FIFO compatibility alias
openFile(path, mode)           -> VM handle / -1
read(handle)                   -> String
write(handle, data)             -> byte count / -1
close(handle)                  -> null
unlinkFile(path)               -> 0 / -1
```

The core owns raw host file descriptors and exposes only bounded VM-local
handles. This is the same design principle used by Sleela's socket layer.

## Linux semantics

A Linux anonymous pipe has a read end and a write end. A FIFO is the named
filesystem form of the same byte-stream IPC model. Reads may block until data
is available; writes may block when the pipe buffer is full. Nonblocking file
opens are available by adding `n` to an `openFile` mode.

SLeeLa's implementation uses `O_CLOEXEC` for newly opened descriptors and
closes active descriptors when the VM is destroyed.

## NPFS distinction

NPFS is the Windows named-pipe filesystem concept. It should not be described
as a Linux filesystem. On Linux, `npfsCreate(...)` is intentionally only a
compatibility spelling that creates a POSIX FIFO with `mkfifo(3)`. A future
Windows backend can map the same higher-level Sleela operations to Windows
named-pipe handles.

## Example

See [`impl/examples/fileio.sleela`](impl/examples/fileio.sleela) for an
anonymous-pipe program and [`impl/examples/fileio-fifo.sleela`](impl/examples/fileio-fifo.sleela)
for a Linux FIFO example.

The complete implementation and API contract are in
[`impl/FILEIO.md`](impl/FILEIO.md).
