# SLVM OS Capability Architecture

The SLeeLa Virtual Machine is designed to support the breadth of operating-system facilities expected by a modern programming language without making the VM itself an uncontrolled operating-system syscall trampoline.

## Quality Principle

**Expressiveness belongs in the VM API; authority belongs in explicit capabilities.**

A SLeeLa program should be able to request modern OS functionality through stable VM/native interfaces while the VM retains a clear boundary around what the program is permitted to do.

## Capability Domains

The architecture reserves capability domains for:

- process and thread management
- filesystem and file descriptors
- paths, directories, metadata, links, and file watching
- environment variables and process configuration
- standard input/output/error
- sockets and networking
- DNS and endpoint resolution
- timers, clocks, monotonic time, and scheduling
- signals and platform events
- memory mapping and virtual memory facilities
- shared libraries and dynamic loading
- device access
- terminals and consoles
- IPC, pipes, queues, shared memory, and local sockets
- synchronization primitives
- user, group, credential, and permission queries
- security facilities
- cryptographic/randomness providers
- system information and resource limits
- locale, character, and encoding services
- graphics/windowing integration through controlled native adapters
- audio/video/device integration through native adapters
- platform-specific extensions

This list is an architectural scope, not a promise that every facility is exposed as a raw syscall.

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
      +--> portable OS adapter
      |
      +--> Linux adapter
      |
      +--> Windows 10+ adapter
      |
      +--> macOS adapter
      |
      +--> controlled native library adapter
      |
      v
Operating System
```

The VM should not embed Linux-only, Windows-only, or macOS-only syscall semantics into core opcodes.

## Capability Handles

OS resources should be represented by opaque VM-managed handles rather than exposing native pointers as language values.

Examples:

- file handle
- socket handle
- process handle
- thread handle
- timer handle
- directory handle
- mapped-memory handle
- device handle
- shared-memory handle

The capability broker owns validation, lifecycle, ownership, and permission checks.

## No Implicit Privilege Escalation

The VM must never turn an ordinary program operation into administrator/root/SYSTEM authorization.

Privileged operations require an explicit capability and platform authorization. Failure must be represented as a structured VM error.

## Portability

Portable VM instructions describe intent. OS adapters translate that intent into platform APIs.

For example:

`SLVM_FILE_OPEN → capability broker → POSIX open()/Windows CreateFile()/macOS POSIX layer`

The VM instruction set therefore remains stable while native implementations evolve.

## Async and Blocking Operations

The architecture must support both:

- synchronous/blocking calls
- asynchronous operations with completion objects/events

Blocking OS calls must not silently stall an unrelated VM execution context. The scheduler/runtime integration should make blocking behavior explicit.

## Error Model

OS errors must preserve:

- VM error class
- operation
- capability/resource
- platform error code
- portable error category
- human-readable diagnostic
- recoverability indication

Native error codes should not be discarded.

## Security and Quality Gates

Every OS boundary should have:

1. capability validation
2. argument validation
3. resource/lifetime validation
4. platform adapter selection
5. operation execution
6. native error translation
7. audit/diagnostic hook
8. resource cleanup on failure

This architecture keeps OS breadth in scope from the beginning while preventing the VM core from becoming an unmaintainable collection of platform-specific syscalls.

Copyright (c) Max Rupplin - MEARVK LLC - 2026
