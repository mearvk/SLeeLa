# SLeeLa System Call Surface

SLeeLa intentionally uses a compact **OS + version + operation-family** model instead of a giant syscall-number table.

The unit of support is a SLeeLa operation family. The native implementation then selects the platform API appropriate for the host.

| Family | SLeeLa operation shape | SLeeLa built-ins | Linux | Windows 10+ | macOS |
|---|---|---|---|---|---|
| Memory | alloc/free/realloc/page/time | (runtime) | malloc/free/realloc, sysconf/clock | Heap/CRT allocation, GetSystemInfo/GetTickCount64 | malloc/free/realloc, sysctl/libSystem |
| File I/O | open/read/write/close/unlink | `openFile`/`read`/`write`/`close`/`unlinkFile` | open/read/write/close/unlink | CreateFile/ReadFile/WriteFile/CloseHandle/DeleteFile | open/read/write/close/unlink |
| Filesystem metadata | exists/isDir/size/mkdir/remove/rename | `osExists`/`osIsDir`/`osFileSize`/`osMakeDir`/`osRemove`/`osRename` | stat/mkdir/rmdir/unlink/rename | GetFileAttributes(Ex)/CreateDirectory/RemoveDirectory/DeleteFile/MoveFileEx | stat/mkdir/rmdir/unlink/rename |
| Paths | normalize/query filesystem paths | `osCurrentDir`/`osChangeDir`/`osTempDir` | getcwd/chdir, POSIX filesystem APIs | GetCurrentDirectory/SetCurrentDirectory/GetTempPath | getcwd/chdir, POSIX filesystem APIs |
| Environment & identity | getenv/setenv/host/user/pid | `osGetEnv`/`osSetEnv`/`osHostName`/`osUserName`/`osProcessId`/`osPlatform`/`osCapability` | getenv/setenv, gethostname, getpwuid, getpid, uname | GetEnvironmentVariable/SetEnvironmentVariable, GetComputerName, GetUserName, GetCurrentProcessId | getenv/setenv, gethostname, getpwuid, getpid, sysctl |
| Networking | startup/listen/accept/connect/read/write/close | `listen`/`accept`/`connect`/`sockread`/`sockwrite`/`sockclose` | sockets | Winsock | sockets |
| Threads | mutex/condition/create/join | `spawn`/`join`/`lock`/`unlock`/`send`/`recv` | pthreads | Win32 threads, CriticalSection, ConditionVariable | pthreads |
| Libraries | open/symbol/close | (runtime) | dlopen/dlsym/dlclose | LoadLibrary/GetProcAddress/FreeLibrary | dlopen/dlsym/dlclose |
| Processes | spawn/wait/kill/exit | `osRun`/`osSpawn`/`osWait`/`osKill`/`osProcessClose` | posix_spawn/waitpid/kill, system | CreateProcess/WaitForSingleObject/TerminateProcess, system | posix_spawn/waitpid/kill, system |
| IPC | pipe/named-pipe endpoint | `pipe`/`pipePeer`/`fifoCreate` | pipe/FIFO | anonymous/named pipes | pipe/FIFO |
| Terminal | terminal/PTY operations | (runtime) | termios/PTY facilities | console/Win32 terminal facilities | termios/PTY facilities |
| Time | monotonic/wall-clock | `timeUtcMillis`/`timeMonotonicNanos`/`timeHttpDate` | clock_gettime and POSIX time | Win32 timing APIs | libSystem/POSIX timing APIs |

The **Environment & identity**, **Filesystem metadata**, **Paths**, and
**Processes** families are reached from SLeeLa source through the `os*`
built-ins (syntax 1.5), which lower to the `OP_OS_*` opcodes and are serviced by
`impl/core/sleela_os.c`. See [`/lib/os/OS.md`](../lib/os/OS.md) for the full
built-in surface and the `/lib/os` classes that wrap it, and
[`/lib/vm/OPCODE-MAP.md`](../lib/vm/OPCODE-MAP.md) for the opcode map. A spawned
process is a VM-local bounded handle, never a raw PID or HANDLE.

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
