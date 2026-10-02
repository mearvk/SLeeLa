# SLVM OS Capability Matrix

| Domain | Portable VM Contract | Platform Adapter |
|---|---|---|
| Files | open/read/write/close/stat | POSIX / Win32 |
| Directories | enumerate/create/remove | POSIX / Win32 |
| Processes | spawn/wait/terminate/query | POSIX / Win32 |
| Threads | create/join/synchronize | pthreads / Win32 |
| Networking | sockets/connect/listen/accept | POSIX sockets / Winsock |
| DNS | forward/reverse resolution | resolver / platform DNS |
| Time | wall/monotonic clocks/timers | POSIX / Win32 |
| IPC | pipes/local sockets/shared memory | POSIX / Win32 |
| Signals/events | subscribe/dispatch | POSIX / Win32 |
| Memory | map/protect/unmap | mmap / VirtualAlloc |
| Libraries | load/symbol/unload | dlopen / LoadLibrary |
| Devices | enumerated controlled access | platform-specific |
| Security | identity/permissions/credentials | platform-specific |
| System | host/resources/limits | platform-specific |
| Terminal | console/TTY operations | POSIX / Win32 |
| Randomness | secure random source | OS provider |
| Crypto | provider boundary | OS/native crypto |
| GUI/media | native adapter boundary | platform/framework-specific |

Most OS facilities should be capability-broker operations rather than hard-coded opcodes.

Copyright (c) Max Rupplin - MEARVK LLC - 2026
