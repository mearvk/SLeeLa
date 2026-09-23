# SLeeLa System Call Surface

SLeeLa intentionally uses a compact **OS + version + operation-family** model instead of a giant syscall-number table.

The unit of support is a SLeeLa operation family. The native implementation then selects the platform API appropriate for the host.

| Family | SLeeLa operation shape | Linux | Windows 10+ | macOS |
|---|---|---|---|---|
| Memory | alloc/free/realloc/page/time | malloc/free/realloc, sysconf/clock | Heap/CRT allocation, GetSystemInfo/GetTickCount64 | malloc/free/realloc, sysctl/libSystem |
| File I/O | open/read/write/close/unlink | open/read/write/close/unlink | CreateFile/ReadFile/WriteFile/CloseHandle/DeleteFile | open/read/write/close/unlink |
| Paths | normalize/query filesystem paths | POSIX filesystem APIs | Win32 path/file APIs | POSIX filesystem APIs |
| Networking | startup/listen/accept/connect/read/write/close | sockets | Winsock | sockets |
| Threads | mutex/condition/create/join | pthreads | Win32 threads, CriticalSection, ConditionVariable | pthreads |
| Libraries | open/symbol/close | dlopen/dlsym/dlclose | LoadLibrary/GetProcAddress/FreeLibrary | dlopen/dlsym/dlclose |
| Processes | spawn/wait/exit | POSIX process APIs | CreateProcess/process handles | POSIX process APIs |
| IPC | pipe/named-pipe endpoint | pipe/FIFO | anonymous/named pipes | pipe/FIFO |
| Terminal | terminal/PTY operations | termios/PTY facilities | console/Win32 terminal facilities | termios/PTY facilities |
| Time | monotonic/wall-clock | clock_gettime and POSIX time | Win32 timing APIs | libSystem/POSIX timing APIs |

## Version policy

**Linux:** capability-first. Kernel/libc versions are recorded for diagnostics, while the SLeeLa abstraction is the compatibility contract.

**Windows 10+:** Windows 10 and later are the supported desktop baseline. Runtime capability checks remain authoritative for optional features.

**macOS:** supported macOS hosts use the native Apple/libSystem and POSIX interfaces exposed by the SLeeLa abstraction. The detected macOS product version is recorded for diagnostics.

An unfamiliar version string does not by itself make SLeeLa refuse to start. A missing capability does.

## Why this model

Linux documents that most system calls are normally reached through C-library wrapper functions and that wrapper behavior can select an appropriate underlying syscall. Direct syscall invocation can also be architecture-specific and can fail with ENOSYS when an operation is not implemented.

Therefore SLeeLa's portable contract is:

SLeeLa operation -> platform abstraction -> native OS API -> kernel/runtime

rather than:

SLeeLa operation -> universal syscall-number table

## Unknown operations

When a custom extension requests an operation outside this surface:

custom operation -> native provider -> Heuristic System Monitor -> policy decision

The monitor can record the operation as unknown/native without requiring SLeeLa to pretend that the operation is portable.

## Grade relationship

- Grade I: memory management plus platform identity, capability detection and HSM.
- Grade II: Grade I plus three-process session accounting.
- Grade III: Grade II plus resource accounting across the operation families above.

The surface is intentionally compact and extensible. New OS functionality is added as an operation family or capability, not by turning this document into a raw syscall-number encyclopedia.
