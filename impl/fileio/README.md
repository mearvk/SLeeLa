# Sleela File I/O Subsystem

This directory documents the Linux file-descriptor abstraction added to the
Sleela 1.1 C/C++ core.

## Primitives

- `pipe()` creates an anonymous Linux pipe.
- `pipePeer(handle)` obtains the paired end.
- `fifoCreate(path, mode)` creates a filesystem FIFO.
- `npfsCreate(path, mode)` is the Linux compatibility alias for `fifoCreate`.
- `openFile(path, mode)` opens regular files or FIFOs.
- `read(handle)` and `write(handle, data)` perform byte-stream I/O.
- `close(handle)` closes the VM-owned descriptor.
- `unlinkFile(path)` removes a filesystem name.

Raw Linux descriptors never become Sleela values. The runtime assigns a
bounded VM-local handle, currently limited to 256 simultaneous file handles.

See [`../FILEIO.md`](../FILEIO.md) for the full API, examples, lifecycle rules,
and the Linux-vs-Windows NPFS distinction.
