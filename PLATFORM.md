# SLeeLa Platform Matrix

## Supported operating systems

### Linux
POSIX process/thread facilities, TCP/UDP, files, pipes, PTY, dynamic libraries, terminal signals and native executable integration.

### macOS
Darwin/POSIX facilities, Apple Clang toolchain, sockets, files, processes, PTY and dynamic libraries.

### Windows 10+
Win32 process/thread facilities, Winsock networking, files, named pipes, console/PTY-compatible services and dynamic libraries.

## Architecture

The platform layer must identify:

- OS;
- architecture;
- compiler/toolchain;
- pointer width;
- ABI;
- endianness;
- native library format;
- available security facilities.

## Capability model

Applications should request capabilities rather than infer privileges. Platform adapters expose availability and return explicit unsupported errors.

## Packaging

Platform packages should map to native conventions rather than forcing one installer format.

## CI

Every supported platform should have build, unit, integration and failure-path CI. Cross-platform behavior must be tested at the abstraction boundary and at the native backend.

## Portability rule

Portable SLeeLa source must not depend directly on platform internals unless it explicitly imports a platform module.

**Max Rupplin — MEARVK LLC — 2026**
