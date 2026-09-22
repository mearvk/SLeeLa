# SLeeLa — Linux API Reference and Integration Guide

**Project:** SLeeLa  
**Platform:** Linux  
**API family:** Linux kernel system calls, POSIX/Linux userspace APIs, libc, and native Linux facilities  
**Purpose:** Linux-native API catalog parallel to API.WIN32.md.

## Scope

Linux has several native API layers: POSIX and ISO C, libc, Linux system calls, Linux-specific facilities, procfs/sysfs/cgroup interfaces, device interfaces, and desktop APIs such as Wayland, X11, GTK, and Qt.

The Linux system-call interface is the kernel entry boundary. Normal applications commonly reach system calls through C-library wrappers; Linux man-pages separates system calls in section 2 from library functions in section 3. citeturn0search0turn0search1turn0search8

## API layers

| Layer | Examples | SLeeLa boundary |
|---|---|---|
| System call | open, read, mmap, socket | Low-level native |
| libc | malloc, pthreads, dlopen | C ABI |
| POSIX | files, processes, threads, sockets | Portable Unix |
| Linux-specific | epoll, inotify, io_uring, eventfd | Linux backend |
| Kernel virtual FS | procfs, sysfs, cgroupfs | Validated file boundary |
| Desktop | Wayland, X11, GTK, Qt | GUI adapters |
| Device | ioctl, udev, DRM, V4L2, ALSA | Hardware boundary |

A libc wrapper is not necessarily identical to its underlying syscall; wrappers can perform architecture-specific work and error translation. citeturn0search0turn0search5

## Processes

fork, vfork, execve, execv, execvp, posix_spawn, waitpid, waitid, _exit, exit, getpid, getppid, getuid, geteuid, getgid, getegid, setuid, setgid, kill, and prctl are representative process interfaces.

Process creation, credential changes, and signal delivery must remain behind explicit SLeeLa permissions.

## Threads and synchronization

Representative interfaces include pthread_create, pthread_join, pthread_detach, pthread mutexes, condition variables, read/write locks, barriers, thread-local storage, futexes, POSIX semaphores, and file locks.

Native thread objects must preserve SLeeLa lifecycle and managed-memory rules.

## Memory

Native facilities include mmap, munmap, mprotect, madvise, mlock, munlock, brk, POSIX shared memory, System V shared memory, huge pages, and memory-mapped files.

SLeeLa's managed memory remains the language/runtime policy. The current SLeeLa memory manager default is 2 GiB and is configurable down to 256 MiB. Native mappings must not silently bypass that policy.

## Filesystem and I/O

Important calls include open, openat, close, read, write, pread, pwrite, readv, writev, lseek, fsync, fdatasync, fcntl, stat, fstat, lstat, statx, mkdir, mkdirat, unlink, unlinkat, rename, renameat2, link, symlink, readlink, access, and faccessat.

File descriptors are process-local resources. SLeeLa should track ownership, closure, inheritance, and errors explicitly.

## Event and asynchronous I/O

Linux provides select, poll, ppoll, epoll_create1, epoll_ctl, epoll_wait, eventfd, timerfd_create, signalfd, and io_uring.

Portable SLeeLa event abstractions should hide epoll/io_uring implementation details inside the Linux backend.

## Networking

The native socket surface includes socket, socketpair, bind, listen, accept, accept4, connect, shutdown, send, sendto, sendmsg, recv, recvfrom, recvmsg, getsockname, getpeername, getsockopt, setsockopt, getaddrinfo, freeaddrinfo, and interface-name/index conversion.

Linux-specific networking may additionally use netlink, routing APIs, namespaces, traffic control, and eBPF. These should remain explicit Linux extensions rather than portable POSIX APIs.

## Signals, timers, and clocks

Relevant interfaces include sigaction, sigprocmask, pthread_sigmask, sigwait, raise, kill, timer_create, timer_settime, clock_gettime, clock_getres, clock_nanosleep, nanosleep, and gettimeofday.

SLeeLa should distinguish wall-clock time from monotonic elapsed time and should not perform arbitrary managed-runtime operations from unsafe native signal-handler context.

## IPC

Linux IPC includes pipes, FIFOs, Unix-domain sockets, shared memory, POSIX message queues, POSIX semaphores, eventfd, System V IPC, signals, and pidfds.

SLeeLa should prefer typed resource objects over raw descriptor integers.

## Linux-specific system facilities

Important interfaces include pidfd_open, pidfd_send_signal, prctl, namespaces, cgroups, capabilities, seccomp, procfs, sysfs, netlink, inotify, fanotify, and ioctl.

These can affect processes, devices, namespaces, or system policy and require explicit security boundaries.

## Security

Relevant Linux security surfaces include UID/GID credentials, supplementary groups, file permissions, POSIX ACLs, Linux capabilities, seccomp, namespaces, SELinux, AppArmor, audit interfaces, and kernel keyrings.

SLeeLa should use host authorization mechanisms rather than bypassing them.

## Dynamic libraries

The common loader surface includes dlopen, dlsym, dlclose, and dlerror.

Library paths and symbols must be validated. Arbitrary native function execution must not become an implicit SLeeLa language feature.

## Terminal and devices

Terminal interfaces include isatty, tcgetattr, tcsetattr, ioctl, and pseudo-terminal facilities.

Device interaction can involve /dev, ioctl, sysfs, udev, netlink, DRM/KMS, input events, ALSA, V4L2, and USB. SLeeLa should expose narrowly scoped device APIs rather than unrestricted ioctl.

## GUI

Linux has no single universal native GUI API. SLeeLa should keep Wayland, X11/Xlib/XCB, GTK, Qt, EGL, Vulkan, and OpenGL adapters separate and document their actual dependencies.

## Standard C/POSIX library

The native catalog also covers userspace services for strings, memory, regular expressions, locale, character conversion, mathematics, threads, networking, file I/O, process management, and dynamic loading.

SLeeLa should prefer standards-based interfaces when they satisfy the requirement and use Linux-specific APIs only intentionally.

## Architecture and ABI

Native code must not assume pointer size, structure packing, endianness, syscall numbering, register conventions, alignment, long width, or native time-type layout.

The syscall() interface is architecture-sensitive, so direct raw syscalls should be isolated from portable SLeeLa code. citeturn0search5

Each binding should record ABI, compiler target, pointer width, byte order, calling convention, library requirements, kernel requirements, and feature availability.

## Version targeting

SLeeLa should distinguish minimum kernel version, distribution release, and libc version. Optional APIs should have documented requirements, runtime capability detection where appropriate, controlled fallbacks, and explicit failure when a required capability is unavailable.

## Exposed Linux functions and interfaces

| Field | Purpose |
|---|---|
| API name | Exact function/syscall/interface |
| Layer | Kernel, libc, POSIX, Linux-specific, toolkit |
| Header | Declaring header |
| Library | Required userspace library |
| Syscall | Underlying syscall where applicable |
| Parameters | Native parameter types |
| Return value | Native result |
| Error behavior | errno/subsystem status |
| Minimum version | Kernel/libc availability |
| Architecture | ABI/architecture |
| SLeeLa binding | SLeeLa API/class |
| Resource ownership | FD, mapping, thread, socket, etc. |
| Security boundary | Privilege/device implications |
| Status | Implemented/planned/unsupported/deprecated |

The Linux man-pages project documents the distinction between system calls and library functions and records availability/version information. citeturn0search0turn0search8

## Representative callable surfaces

### File API

open/openat; read/pread/readv; write/pwrite/writev; lseek; fstat/statx; fcntl; fsync/fdatasync; close.

### Socket API

socket/socketpair; bind; listen; accept/accept4; connect; send/sendmsg/sendto; recv/recvmsg/recvfrom; getsockopt/setsockopt; shutdown/close.

## SLeeLa implementation rule

Documenting Linux does not automatically expose every native function.

A Linux function becomes a SLeeLa API only after a defined SLeeLa signature, parameter validation, return/error translation, resource ownership rules, managed-memory compliance, security checks, architecture validation, kernel/libc availability behavior, tests, examples, and documentation.

## Platform mapping

| SLeeLa facility | Linux native layer |
|---|---|
| Files | POSIX/Linux descriptors |
| Directories | POSIX/Linux directory APIs |
| Processes | fork/exec/wait/pidfd |
| Threads | pthreads/Linux synchronization |
| Memory | mmap/mprotect + managed runtime |
| Networking | sockets/netlink |
| Event loop | epoll/io_uring |
| File events | inotify/fanotify |
| IPC | pipes, Unix sockets, shared memory, queues |
| Terminal | termios/PTY |
| Dynamic libraries | dlopen/dlsym |
| Security | credentials/capabilities/seccomp |
| GUI | Wayland/X11/GTK/Qt |
| Time | POSIX/Linux clocks |
| Devices | ioctl/sysfs/udev |
| System information | procfs/sysfs |

## Security boundary

Native Linux calls should validate input, check every return value, preserve errno when necessary, close descriptors deterministically, use close-on-exec where appropriate, avoid arbitrary shell execution, validate native module paths, respect UID/GID/capability boundaries, avoid unrestricted ptrace/mount/namespace/ioctl/raw-syscall exposure, and isolate Linux-specific code from portable SLeeLa code.

## Primary references

- Linux system calls: https://man7.org/linux/man-pages/man2/syscalls.2.html
- System-call introduction: https://man7.org/linux/man-pages/man2/intro.2.html
- Indirect syscall interface: https://man7.org/linux/man-pages/man2/syscall.2.html
- Library functions: https://man7.org/linux/man-pages/man3/intro.3.html
- Linux man-pages: https://www.kernel.org/doc/man-pages/
- Linux kernel documentation: https://docs.kernel.org/
- POSIX specification: https://pubs.opengroup.org/onlinepubs/9699919799/

## Status

This document defines the Linux native API boundary relevant to SLeeLa. It is a catalog and integration policy, not a claim that every listed interface is implemented.

---

**SLeeLa — Linux API Reference**  
**MEARVK LLC — 2026**
