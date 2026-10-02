# SLVM OS Capability Architecture

The SLeeLa Virtual Machine is designed to support the breadth of operating-system facilities expected by a modern programming language without making the VM itself an uncontrolled operating-system syscall trampoline.

## Quality Principle

**Expressiveness belongs in the VM API; authority belongs in explicit capabilities.**

A SLeeLa program should request modern OS functionality through stable VM/native interfaces while the VM retains a clear boundary around what the program is permitted to do.

## Capability Domains

- process and thread management
- filesystem, descriptors, paths, directories, metadata and watching
- environment and process configuration
- standard I/O
- sockets and networking
- DNS and endpoint resolution
- clocks, timers and scheduling
- signals and platform events
- memory mapping and virtual memory
- shared libraries and dynamic loading
- device access
- terminals and consoles
- IPC, pipes, queues, shared memory and local sockets
- synchronization
- identity, credentials, permissions and security
- cryptographic/randomness providers
- system information and resource limits
- locale and encoding services
- graphics/windowing
- audio/video/device integration
- platform-specific extensions

This is architectural scope, not a promise that every facility becomes a raw syscall opcode.

## Layering

```
SLeeLa Program
      |
      v
SLVM Instruction / Standard Library API
      |
      v
Capability Broker
      |
      +--> Portable OS Adapter
      +--> Linux Adapter
      +--> Windows 10+ Adapter
      +--> macOS Adapter
      +--> Controlled Native Library Adapter
      |
      v
Operating System
```

The VM core must not embed Linux-only, Windows-only, or macOS-only syscall semantics.

## Capability Handles

OS resources are represented by opaque VM-managed handles rather than native pointers.

Examples include file, socket, process, thread, timer, directory, mapped-memory, device and shared-memory handles.

The capability broker owns validation, lifecycle, ownership and permission checks.

## No Implicit Privilege Escalation

The VM must never turn an ordinary program operation into administrator/root/SYSTEM authorization. Privileged operations require explicit capability and platform authorization.

## Blocking and Async Operations

The architecture supports both synchronous/blocking calls and asynchronous operations with completion objects/events. Blocking native calls must not silently stall unrelated VM execution contexts.

## Error Model

OS errors preserve VM error class, operation, resource, native error code, portable category, diagnostic text and recoverability information.

## OS Boundary Quality Gate

Every OS boundary should have:

1. capability validation
2. argument validation
3. resource/lifetime validation
4. adapter selection
5. execution
6. native-error translation
7. audit/diagnostic hook
8. cleanup on failure

Copyright (c) Max Rupplin - MEARVK LLC - 2026
