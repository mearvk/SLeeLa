# OS-Level Support Designation

**Status:** Design inventory and implementation roadmap  
**Target platforms:** Linux, Windows 10 and later, and macOS  
**Implementation languages:** C ABI with a thin C++ facade; SLeeLa bindings are a separate integration layer  
**Scope:** User-mode host services and portable operating-system capabilities

This document designates the common OS-support function surface and the platform-dependent function families that the SLeeLa OS-level-support layer should account for. It is a capability inventory, not a claim that every item is implemented. Each capability is classified as **Implemented**, **Partial**, **Planned**, or **Platform-specific candidate**.

The portable API should expose stable, OS-neutral names and result types. Platform backends may use native APIs internally. A capability that is unavailable, denied by policy, or unsupported on a particular platform must return an explicit status; it must not silently fabricate a value or weaken host security.

## 1. Platform and implementation policy

| Platform | Required target | Native implementation family |
|---|---|---|
| Linux | Supported distributions with a documented minimum toolchain/kernel baseline | POSIX interfaces, Linux system calls through documented user-mode libraries, procfs/sysfs where available, and libc |
| Windows | Windows 10 and later | Win32 / Kernel32, Userenv, IP Helper, Winsock, and documented Windows APIs |
| macOS | Supported macOS releases with a documented minimum deployment target | Darwin / POSIX interfaces, SystemConfiguration, sysctl, Mach APIs, and documented Apple frameworks |

The implementation must not assume that Linux, Windows, and macOS expose identical metadata. Normalize values where semantics genuinely match; otherwise expose a platform extension or a capability query.

### Status definitions

- **Implemented:** A corresponding public function exists in the current C ABI and has an implementation in the repository.
- **Partial:** A public function exists, but one or more platforms or returned fields are incomplete or unsupported.
- **Planned:** A useful cross-platform function family is designated here but is not yet part of the current public ABI.
- **Platform-specific candidate:** A native capability that should be exposed only through an explicitly platform-qualified extension when there is no honest common abstraction.

## 2. Common OS-supported function catalogue

These are the common, OS-neutral function families. The same conceptual operation should be available on all three platforms wherever the host OS permits it.

| Function family | Designated operations | Current repository status |
|---|---|---|
| Host and system identity | Detect OS/platform; retrieve hostname, logged-in user name, current working directory, process ID, system time and monotonic time | **Implemented** — `osls_get_system_info` |
| Process inspection | Retrieve current-process metadata: PID, parent PID where available, thread count, start time, executable path, command line, working directory, state and owner | **Partial** — `osls_get_process_info`; current implementation is deliberately limited to the caller's own process |
| Process enumeration | Enumerate processes visible to the caller under the library's documented policy | **Partial** — `osls_enumerate_processes` returns only the caller's process; system-wide enumeration is not implemented |
| Environment | Read one environment variable; enumerate the current process environment | **Implemented** — `osls_get_environment_value`, `osls_enumerate_environment` |
| Filesystem metadata | Query file type, size, timestamps, and readable/writable/executable indicators | **Implemented** — `osls_get_file_info`; access indicators are observations, not guarantees that a later operation will succeed |
| Directory inspection | List directory entry names | **Implemented** — `osls_list_directory` |
| Filesystem operations | Open, read, write, append, create, truncate, flush, close, copy, move/rename, remove, create/remove directory, inspect links, and query available space | **Planned** — add explicit handles, error mapping, permissions and race-safe semantics before exposing these operations |
| Path handling | Normalize paths, resolve relative paths, join/split components, inspect roots/volumes, and query canonical paths where supported | **Implemented** — `osls_path_normalize`, `osls_path_join` (pure computation), `osls_path_resolve` (canonical, read-only), `osls_get_path_info` (normalized/root/absolute/exists) |
| CPU and machine information | Logical/physical processor counts, architecture, vendor/model, cache-line information, and total/available memory | **Implemented** — `osls_get_cpu_info` now populates vendor/model (CPUID on Windows, `/proc/cpuinfo` on Linux, sysctl on macOS) and cache-line size on all three; `osls_get_system_load` adds load averages (POSIX; Windows reports -1) |
| Memory information | Total, available, used and page-size information; process working-set/resident memory where available | **Implemented** — `osls_get_memory_info`, `osls_get_available_memory` (total/available/used/page-size on Linux/Windows/macOS); per-process working-set remains planned |
| Time and clock | Wall-clock time, monotonic time, high-resolution counter, clock frequency/resolution, and UTC/local conversion | **Partial** — system wall-clock and monotonic nanoseconds are returned by `osls_get_system_info` |
| Timezone and calendar | Timezone identifier/abbreviation, UTC offset, daylight-saving state, local-time conversion, and calendar data | **Implemented with portability limits** — `osls_get_timezone_info`; identifier fidelity differs by platform |
| Network interface inspection | Enumerate interfaces and report names, link state, loopback status, MAC address, IPv4 and IPv6 addresses | **Implemented** — `osls_get_network_interfaces`; Windows now uses IP Helper `GetAdaptersAddresses` (names, MAC, IPv4/IPv6, up/loopback) instead of returning `OSLS_UNSUPPORTED`; POSIX uses `getifaddrs` |
| Network configuration inspection | Read DNS resolver configuration, routes, interface addresses, gateways, MTU and interface statistics | **Partial** — `osls_network_resolve` performs read-only hostname/service resolution (no connection); routes, gateways, MTU and statistics remain planned |
| Socket and network I/O | Create/close sockets; bind/listen/accept/connect; send/receive; poll readiness; resolve hostnames; inspect socket endpoints | **Planned** — portable Winsock/POSIX facade with explicit nonblocking and timeout semantics |
| Scheduling expressions | Validate five-field cron expressions and calculate the next local-time occurrence | **Implemented** — `osls_cron_validate`, `osls_cron_next_after`; calculation only, not OS schedule installation |
| Timers and waiting | Monotonic timers, deadlines, sleep, waitable events and cancellation primitives | **Planned** — distinguish process-local timers from OS-managed persistent schedules |
| Runtime event hooks | Register/unregister callbacks, emit in-process events, and dispatch event payloads | **Implemented** — `osls_runtime_subscribe`, `osls_runtime_unsubscribe`, `osls_runtime_emit`; these are library callbacks, not operating-system hooks |
| Cooperative task queue | Post callbacks, run one queued task, and inspect pending task count | **Implemented** — `osls_runtime_post`, `osls_runtime_run_one`, `osls_runtime_pending`; caller pumps the queue |
| Dynamic module loading | Open a trusted shared library, verify the declared ABI version, resolve the declared entry point, call it and close it | **Implemented** — `sl_native_open`, `sl_native_run_module`, `sl_native_close`; native code runs in-process and is not sandboxed |
| OS and runtime capabilities | Query whether an operation is available, permitted, and supported by the current backend | **Implemented** — `osls_get_capabilities` and `osls_get_runtime_info` (backend, ABI version, registered operation count, capability mask), separate from attempting the operation |
| Error and status reporting | Stable portable status codes plus native error detail for diagnostics | **Implemented** — base status enums plus `osls_get_native_error` (native errno/GetLastError code and message); per-operation threading of native detail remains a refinement |


## 3. Common cross-platform API

This is the shared SLeeLa OS-level-support API intended to present one stable C interface across Linux, Windows, and macOS. Platform backends translate these operations to the appropriate native functions. The names below are portable SLeeLa API names, not Win32, Linux, or Darwin symbols.

### 3.1 Existing common C API — alphabetical

These functions are declared in the current public headers. Their implementation status is described in the common catalogue and must be checked per platform.

- `osls_cron_next_after` — calculate the next matching local-time minute
- `osls_cron_validate` — validate a five-field cron expression
- `osls_enumerate_environment` — enumerate the current process environment
- `osls_enumerate_processes` — enumerate processes within the documented visibility policy; currently self-only
- `osls_get_cpu_info` — retrieve CPU and machine summary information
- `osls_get_environment_value` — read one environment variable
- `osls_get_file_info` — retrieve file metadata
- `osls_get_network_interfaces` — enumerate network interfaces
- `osls_get_process_info` — retrieve process metadata within the documented visibility policy; currently self-only
- `osls_get_system_info` — retrieve host identity, process ID, wall-clock and monotonic time, and working directory
- `osls_get_timezone_info` — retrieve timezone information
- `osls_list_directory` — list directory entry names

### 3.2 Existing common runtime API — alphabetical

These functions are declared in `runtime/osls_runtime.h`. They are library-managed, in-process callbacks and cooperative task dispatch; they are not OS hooks, threads, or background workers.

- `osls_runtime_create` — create a runtime instance
- `osls_runtime_destroy` — destroy a runtime instance
- `osls_runtime_emit` — emit an in-process event
- `osls_runtime_pending` — return the queued task count
- `osls_runtime_post` — enqueue a callback for later caller-driven execution
- `osls_runtime_run_one` — execute at most one queued callback
- `osls_runtime_subscribe` — subscribe a callback to an in-process event kind
- `osls_runtime_unsubscribe` — remove an event subscription

### 3.3 Existing shared-module API — alphabetical

These functions are declared in `loader/native_loader.h`. They provide shared-library loading and ABI entry-point handling. Loading a module executes native code in the current process and does not create a sandbox.

- `sl_native_abi_version` — plugin-exported ABI version function
- `sl_native_close` — close a loaded native module
- `sl_native_loader_error` — retrieve the loader diagnostic string
- `sl_native_open` — load a shared library and resolve its declared entry point
- `sl_native_run` — plugin-exported entry point
- `sl_native_run_module` — invoke the loaded module entry point
- `sl_native_status_string` — convert a loader status to a diagnostic string

### 3.4 Planned common API extensions — alphabetical

The following names are proposed portable API additions for the common capability catalogue. They are design designations, not current exported symbols. Final signatures, ownership rules, buffer conventions, and ABI versioning must be specified before implementation.

- `osls_close` — close a library-owned file or resource handle
- `osls_copy_file` — copy a file
- `osls_create_directory` — create a directory
- `osls_file_read` — read bytes from an open file handle
- `osls_file_write` — write bytes to an open file handle
- `osls_flush_file` — flush buffered file data
- `osls_get_available_memory` — retrieve available physical memory
- `osls_get_capabilities` — query supported and permitted backend capabilities
- `osls_get_disk_space` — retrieve total and available space for a path or volume
- `osls_get_memory_info` — retrieve total, available, and page-size information
- `osls_get_native_error` — retrieve the native error detail associated with the last failed operation
- `osls_get_path_info` — retrieve normalized path and root/volume information
- `osls_get_runtime_info` — retrieve OSLS backend and API version information
- `osls_get_socket_info` — retrieve local/peer endpoint information for a socket handle
- `osls_get_system_load` — retrieve system load information where a meaningful platform equivalent exists
- `osls_get_volume_info` — retrieve volume name, filesystem type, and capacity metadata
- `osls_make_directory` — create a directory (alias candidate; choose one canonical name before ABI freeze)
- `osls_move_file` — move or rename a file
- `osls_network_resolve` — resolve a host or service name
- `osls_open_file` — open a file and return an opaque library-owned handle
- `osls_open_socket` — create a socket using portable address-family and type values
- `osls_path_join` — join path components using host-platform rules
- `osls_path_normalize` — normalize a path without claiming filesystem existence
- `osls_path_resolve` — resolve a path where the platform permits canonical resolution
- `osls_read_file` — read a file using a documented buffer/size contract
- `osls_remove_directory` — remove an empty directory
- `osls_remove_file` — remove a file
- `osls_rename_file` — rename a file
- `osls_socket_accept` — accept an incoming connection
- `osls_socket_bind` — bind a socket to a local endpoint
- `osls_socket_close` — close a socket handle
- `osls_socket_connect` — connect a socket to a remote endpoint
- `osls_socket_listen` — listen for incoming connections
- `osls_socket_receive` — receive socket data
- `osls_socket_send` — send socket data
- `osls_socket_set_option` — set an explicitly supported socket option
- `osls_socket_wait` — wait for socket readiness with a timeout
- `osls_sleep` — sleep for a specified duration using a monotonic clock
- `osls_timer_cancel` — cancel a library-managed timer
- `osls_timer_create` — create a process-local timer
- `osls_timer_wait` — wait for a timer or deadline
- `osls_write_file` — write a file using a documented buffer/size contract

**API consistency rules:**

1. Use the `osls_` prefix for portable OS-support functions; keep `osls_runtime_` for runtime queue/event operations and `sl_native_` for native module loading.
2. Keep common API names and result semantics identical on all three platforms. Platform-only capabilities must use an explicitly platform-qualified extension namespace.
3. Use UTF-8 strings at the public C ABI boundary; perform native encoding conversion inside the backend.
4. Use opaque handles for open files, sockets, and timers. Define whether each handle is thread-safe and which function releases it.
5. Distinguish invalid arguments, not found, unsupported, access denied, buffer too small, timeout, and native system failure. Preserve the native error separately from the portable status.
6. Do not expose platform-specific structs, handles, constants, or calling conventions in the common header.
7. Do not finalize duplicate aliases such as `osls_create_directory` and `osls_make_directory`; choose one canonical exported name before freezing the ABI.
8. Mark each function as implemented, partial, or planned in the implementation matrix, and test it on Linux, Windows 10+, and macOS.

## 4. Linux-specific function families

Linux-specific functions belong in a Linux backend or a clearly named Linux extension. They must not leak Linux-only types into the common C ABI.

| Function family | Linux capabilities to support | Designation |
|---|---|---|
| Kernel and distribution identity | Kernel release/version, machine architecture, distribution identity/version when reliably discoverable, boot ID where permitted | Platform-specific candidate |
| Process metadata | Read permitted `/proc` process metadata, status, command line, executable link, thread count, start time and resource counters | Platform-specific candidate; current common process API is self-only |
| Process resource limits | Query current-process limits and accounting data using documented interfaces | Platform-specific candidate |
| Memory and system load | `sysinfo`, `/proc/meminfo`, load averages, swap totals and page size | Platform-specific candidate |
| CPU topology | CPU model/vendor, online CPUs, physical/core/thread topology, cache information and CPU frequency when exposed | Platform-specific candidate |
| Filesystem and mounts | Mount table, filesystem type, mount flags, disk capacity, inode capacity and filesystem statistics | Platform-specific candidate |
| Device and hardware metadata | Read-only device enumeration and selected sysfs metadata, subject to permissions | Platform-specific candidate |
| Network | Interface flags, link-layer addresses, IPv4/IPv6 addresses, routes, neighbors, DNS configuration, MTU and interface counters | Platform-specific candidate |
| Event notification | inotify-based filesystem change notifications; poll/epoll readiness integration | Platform-specific candidate |
| Process and system events | Read-only event sources where available; no ptrace-based process control in the common support layer | Platform-specific candidate |
| Resource controls | Query cgroup membership and readable CPU/memory/pid limits; report namespace/container context when available | Platform-specific candidate |
| Service and session information | Read-only systemd/service-manager and login-session state when the relevant service interface is available | Platform-specific candidate |
| Clock and power metadata | Clock source/resolution and available power/battery metadata where exposed | Platform-specific candidate |

Linux implementation notes:
- Prefer documented interfaces and libc wrappers. Where procfs or sysfs is used, treat it as optional and parse defensively.
- Container, namespace, distribution and service-manager data may be absent or intentionally masked.
- Do not assume that root-only information is available to an ordinary user.
- Do not add kernel modules, syscall interception, ptrace control, or privileged configuration mutation to this library.

## 5. Windows-specific function families

Windows-specific functions belong in a Windows backend or a clearly named Windows extension. Use documented Win32 APIs and preserve Windows access-control behavior.

| Function family | Windows capabilities to support | Designation |
|---|---|---|
| OS identity | Product name/edition, version/build, architecture, computer name and boot/session information using supported version APIs | Platform-specific candidate |
| Process metadata | Current-process image path, command line, parent PID and permitted process counters | Partial in current self-only API; extensions planned |
| Process resource data | Current-process working set, private bytes, handles, thread count and CPU accounting when permitted | Platform-specific candidate |
| CPU and memory | Processor topology, logical/physical core counts, cache data, page size, physical/available memory and processor architecture | Partial — current CPU API does not yet populate all fields |
| Filesystem and volumes | Volume enumeration, filesystem type, volume capacity, free space, file attributes, reparse-point information and file timestamps | Platform-specific candidate |
| Directory change notification | ReadDirectoryChangesW-based change notifications | Platform-specific candidate |
| Network | IP Helper interface/address/route/statistics APIs, DNS configuration and Winsock socket information | Partial — current interface enumeration is unsupported on Windows |
| Event and wait primitives | Waitable events, timers, synchronization objects and completion-port readiness | Platform-specific candidate |
| Services | Read-only Service Control Manager queries for service state and configuration permitted to the caller | Platform-specific candidate |
| Registry inspection | Explicit, read-only key/value query with registry-view and access-right handling | Platform-specific candidate; never silently write or alter policy |
| User and session metadata | Current user identity, session information and selected profile paths | Platform-specific candidate |
| Power and battery | Power status, battery presence/charge and supported power-source metadata | Platform-specific candidate |
| Security context | Query current token identity, groups and enabled privileges for diagnostic display only | Platform-specific candidate; never enable privileges or use the result as authorization |

Windows implementation notes:
- Prefer wide-character Win32 APIs internally and convert at the ABI boundary using explicit UTF-8 rules.
- Distinguish access denied, not found, unsupported, and transient system errors.
- A process handle or token must be opened with the minimum required access rights and closed deterministically.
- Do not add service installation, driver installation, token privilege elevation, registry writes, process injection, or security-control bypass.

## 6. macOS-specific function families

macOS-specific functions belong in a Darwin/macOS backend or a clearly named macOS extension. Prefer documented Apple APIs and respect privacy/TCC and sandbox restrictions.

| Function family | macOS capabilities to support | Designation |
|---|---|---|
| OS identity | macOS product/version/build, Darwin kernel version, architecture and hardware model | Platform-specific candidate |
| Process metadata | Current-process executable path, parent PID, thread and resource information where permitted | Partial in current self-only API; extensions planned |
| CPU and memory | Logical/physical CPU counts, CPU model, memory size, page size and available-memory statistics | Partial — sysctl-backed subset exists |
| System configuration | Read-only sysctl values for documented hardware and kernel metadata | Platform-specific candidate |
| Filesystem and volumes | Mounted volumes, filesystem type, volume capacity, available space, file attributes and resource identifiers | Platform-specific candidate |
| Filesystem events | FSEvents and vnode/kqueue notifications, with explicit scope and lifecycle | Platform-specific candidate |
| Network | Interface and link-layer metadata, addresses, routes, DNS configuration, service order and network-change notifications | Partial — current common interface enumeration uses POSIX interfaces |
| System and user services | Read-only launchd service/job state where supported by documented interfaces and permissions | Platform-specific candidate |
| Power and battery | I/O Kit power-source and battery metadata where available | Platform-specific candidate |
| Hardware metadata | Read-only model, architecture, selected device and power metadata through documented system interfaces | Platform-specific candidate |
| Privacy and sandbox state | Report relevant permission-denied/sandbox-limited conditions; do not attempt to bypass TCC or sandbox policy | Platform-specific candidate |
| Clock and notifications | Mach monotonic clock and supported dispatch/timer facilities | Partial — Mach monotonic time is used by the current system-info implementation |

macOS implementation notes:
- Treat TCC, App Sandbox, SIP and other platform restrictions as authoritative. Report denied or unavailable information rather than attempting workarounds.
- Do not rely on undocumented private frameworks or unstable kernel interfaces.
- Do not add kernel extensions, system extensions, launch daemons, privileged helpers, or persistent background installation as part of this user-mode library.

## 7. Platform capability comparison

The following matrix describes the intended backend coverage, not a claim that every backend is complete today.

| Capability | Linux | Windows | macOS |
|---|---|---|---|
| Host identity and clocks | POSIX + Linux metadata | Win32 | POSIX + Mach |
| Environment and current directory | POSIX/libc | CRT + Win32 | POSIX/libc |
| Current-process inspection | procfs + POSIX | Win32 process APIs | procfs-style APIs are not assumed; Darwin process APIs |
| File metadata and directory listing | POSIX | Win32 | POSIX |
| CPU and memory | sysconf/sysinfo/procfs/sysfs | Win32 system/memory APIs | sysctl/Mach |
| Timezone | libc/timezone database | CRT/Windows timezone APIs | libc/ICU or supported system configuration APIs |
| Network interfaces | getifaddrs/netlink | IP Helper | getifaddrs/SystemConfiguration |
| Network sockets | POSIX sockets | Winsock | BSD sockets |
| Filesystem notifications | inotify | ReadDirectoryChangesW | FSEvents/kqueue |
| System/service inspection | systemd or available service manager | Service Control Manager | launchd-supported interfaces |
| Hardware/power metadata | sysfs and available system interfaces | power APIs | I/O Kit |
| Dynamic library loading | dlopen/dlsym | LoadLibrary/GetProcAddress | dlopen/dlsym |
| Local timers/events | POSIX facilities | waitable objects/timers | Mach/dispatch/POSIX facilities |

A cell identifies a likely native implementation family. It does not promise identical output, identical permissions, or identical semantics across operating systems.

## 8. C ABI and C++ facade requirements

1. Keep the C ABI authoritative and usable from C and C++.
2. Use fixed-width integer types for ABI-visible numeric fields.
3. Use explicit caller-provided buffer sizes or opaque handles; never return pointers to temporary internal storage.
4. Define ownership and lifetime for every returned buffer, handle, callback payload and native resource.
5. Return a stable OSLS status and, where useful, a separate native error code.
6. Provide capability discovery for optional functions and distinguish unsupported from access denied.
7. Keep the C++ facade thin: RAII, value types, strings/vectors and safe handle ownership; do not create a second incompatible semantic API.
8. Use UTF-8 at the portable API boundary. Convert to/from native encodings explicitly.
9. Define time units, epoch, monotonic-clock semantics, path rules, and whether timestamps represent creation, metadata change, or modification.
10. Document whether each function is read-only, process-local, system-wide, blocking, asynchronous, or permission-sensitive.

## 9. Security and authority boundaries

The OS-level-support library is a user-mode portability layer. Its functions report host state or perform explicitly requested, ordinary user-mode operations. They do not confer authority.

The following are out of scope for this library and must not be added as implied consequences of a successful metadata or trust check:

- kernel memory access, kernel hooks, syscall interception, or stealth mechanisms;
- driver or kernel-extension installation, removal, or control;
- process injection, credential theft, security-control bypass, or unauthorized cross-process access;
- automatic privilege elevation or enabling token privileges;
- trust-based execution authorization or automatic trust-root installation;
- silent changes to registry, system configuration, firewall, network routes, services, startup entries, or persistent schedules.

Any future operation that writes files, changes settings, starts a process, or modifies a service must be separately specified, explicitly invoked by the caller, permission-checked by the OS, and documented as a state-changing operation. It must not be hidden behind a read/query API.

## 10. Implementation priorities

1. **Complete and correct the existing common API:** *(revision 1.1)* Windows network-interface enumeration is now implemented via IP Helper; CPU vendor/model and cache-line fields are now populated on all three platforms; the Windows timezone identifier now uses the real time-zone key name. Remaining: improve process metadata consistency and validate timestamp semantics across platforms.
2. **Add capability and error reporting:** expose backend capabilities and native diagnostic codes consistently.
3. **Complete common read-only inspection:** dedicated memory, path, volume, mount, route, DNS, socket and interface-statistics APIs.
4. **Add event facilities:** filesystem notifications and portable wait/timer abstractions with OS-specific backends.
5. **Add carefully scoped platform extensions:** Linux procfs/sysfs/cgroup/service metadata; Windows volume/service/registry-read/power metadata; macOS volume/FSEvents/launchd/power metadata.
6. **Add tests on all three operating systems:** compile-time coverage, runtime tests, unavailable-permission tests, Unicode paths, large buffers, missing facilities, and error mapping.
7. **Publish an implementation matrix:** each function must identify its header, backend source, supported platforms, tests, permission requirements and current status before being described as complete.

## 11. Current public API reference

The current root C API is declared in `osls.h` and implemented across `osls.c` and `osls_extended.c`:

- `osls_get_system_info`
- `osls_get_process_info`
- `osls_enumerate_processes`
- `osls_get_environment_value`
- `osls_enumerate_environment`
- `osls_get_file_info`
- `osls_list_directory`
- `osls_get_cpu_info`
- `osls_get_timezone_info`
- `osls_get_network_interfaces`
- `osls_cron_validate`
- `osls_cron_next_after`

Revision 1.1 adds the following read-only inspection functions, all registered
through `OP_OSLS_CALL` operation IDs 12..24 under the same host-read capability:

- `osls_get_memory_info`
- `osls_get_available_memory`
- `osls_get_disk_space`
- `osls_get_volume_info`
- `osls_get_system_load`
- `osls_get_capabilities`
- `osls_get_runtime_info`
- `osls_get_native_error`
- `osls_path_normalize`
- `osls_path_join`
- `osls_path_resolve`
- `osls_get_path_info`
- `osls_network_resolve`

The remaining planned functions (file open/read/write/close, copy/move/remove,
directory create/remove, sockets, timers, and `osls_sleep`) are **state-changing
or handle/stream operations**. By the authority boundary in section 9 they are
deliberately **not** registered through the read-only `OP_OSLS_CALL` path; each
must be specified with its own explicitly granted capability before it is
exposed, and must never be reachable under the default host-read capability.

The runtime callback/task API is declared in `runtime/osls_runtime.h`. The native shared-module API is declared in `loader/native_loader.h`. Their presence does not imply sandboxing, OS syscall interception, persistent scheduling, or process creation.


## 12. Alphabetical native operating-system function index

This index names native functions and documented system interfaces that may implement the capability families above. Names are grouped by operating system and sorted alphabetically within each group. It is an implementation reference, not a promise that every function is currently called by SLeeLa or is available on every supported OS release. Prefer documented, user-mode APIs; check availability, permissions, and return status at runtime.

### 12.1 Windows — Win32 and documented system APIs

The following Win32 and Windows API names are listed alphabetically. Related API families are included where they are relevant to the designated support surface.

- `CloseHandle`
- `CreateEventW`
- `CreateFileW`
- `CreateIoCompletionPort`
- `CreateMutexW`
- `CreateProcessW`
- `CreateSemaphoreW`
- `CreateTimerQueueTimer`
- `CreateWaitableTimerW`
- `DeleteFileW`
- `DeviceIoControl` — only for documented, permitted device interfaces; not a general driver-control feature
- `FindClose`
- `FindFirstFileW`
- `FindNextFileW`
- `FormatMessageW`
- `GetComputerNameExW`
- `GetCurrentDirectoryW`
- `GetCurrentProcess`
- `GetCurrentProcessId`
- `GetCurrentThreadId`
- `GetDiskFreeSpaceExW`
- `GetEnvironmentStringsW`
- `GetEnvironmentVariableW`
- `GetExitCodeProcess`
- `GetFileAttributesExW`
- `GetFileInformationByHandleEx`
- `GetLastError`
- `GetLogicalDrives`
- `GetLogicalDriveStringsW`
- `GetNativeSystemInfo`
- `GetProcessId`
- `GetProcessImageFileNameW`
- `GetProcessTimes`
- `GetSystemInfo`
- `GetSystemTimeAsFileTime`
- `GetSystemTimePreciseAsFileTime`
- `GetSystemTimes`
- `GetTickCount64`
- `GetTimeZoneInformation`
- `GetUserNameW`
- `GetVolumeInformationW`
- `GetWindowsDirectoryW`
- `GlobalMemoryStatusEx`
- `InitializeProcThreadAttributeList`
- `LoadLibraryW`
- `LocalFree`
- `MoveFileExW`
- `MultiByteToWideChar`
- `OpenProcess`
- `OpenSCManagerW`
- `OpenServiceW`
- `QueryPerformanceCounter`
- `QueryPerformanceFrequency`
- `QueryServiceConfigW`
- `QueryServiceStatusEx`
- `ReadDirectoryChangesW`
- `ReadFile`
- `RegCloseKey`
- `RegOpenKeyExW`
- `RegQueryValueExW`
- `SetCurrentDirectoryW`
- `SetEnvironmentVariableW`
- `SetFilePointerEx`
- `SetWaitableTimer`
- `Sleep`
- `TerminateProcess` — not part of routine inspection; any future use requires a separately specified, explicit process-control API
- `WaitForMultipleObjects`
- `WaitForSingleObject`
- `WideCharToMultiByte`
- `WriteFile`

**Additional Windows API families (alphabetical):**

- IP Helper API — interface, address, route and statistics inspection, including `GetAdaptersAddresses`, `GetIfEntry2`, `GetIpForwardTable2`
- Performance Data Helper (PDH) — performance-counter query and collection
- Service Control Manager (SCM) — read-only service enumeration and status queries
- Winsock 2 — socket operations and name resolution, including `accept`, `bind`, `closesocket`, `connect`, `getaddrinfo`, `getsockname`, `getsockopt`, `listen`, `recv`, `select`, `send`, `setsockopt`, `shutdown`, `socket`, and `WSAStartup`

### 12.2 Linux — POSIX, libc, and Linux system interfaces

The following function names are listed alphabetically. Some are libc/POSIX interfaces and some are Linux-specific interfaces or system calls. Prefer libc wrappers over issuing raw system calls directly.

- `accept`
- `access`
- `bind`
- `clock_getres`
- `clock_gettime`
- `close`
- `closedir`
- `connect`
- `dup`
- `epoll_create1`
- `epoll_ctl`
- `epoll_wait`
- `execve` — process execution; outside read-only inspection and requires an explicit process-launch API
- `fstat`
- `fstatvfs`
- `ftruncate`
- `getaddrinfo`
- `getcwd`
- `getegid`
- `geteuid`
- `getgid`
- `gethostname`
- `getifaddrs`
- `getnameinfo`
- `getpid`
- `getppid`
- `getpriority`
- `getrlimit`
- `getsockname`
- `getsockopt`
- `gettimeofday`
- `getuid`
- `inotify_add_watch`
- `inotify_init1`
- `inotify_rm_watch`
- `ioctl` — only for explicitly documented and permitted interfaces
- `listen`
- `lseek`
- `lstat`
- `mmap`
- `munmap`
- `nanosleep`
- `open`
- `opendir`
- `poll`
- `pread`
- `pwrite`
- `read`
- `readlink`
- `recv`
- `recvfrom`
- `rename`
- `select`
- `send`
- `sendto`
- `setenv`
- `setrlimit`
- `setsockopt`
- `shutdown`
- `socket`
- `stat`
- `statvfs`
- `sysconf`
- `sysinfo`
- `uname`
- `unsetenv`
- `write`

**Additional Linux interfaces (alphabetical):**

- cgroup v1/v2 control files — read permitted controller and process-limit metadata
- netlink — read-only route, link and address inspection (for example, `NETLINK_ROUTE`)
- procfs — process, kernel, memory, load and boot metadata (for example, `/proc/<pid>/stat`, `/proc/meminfo`, `/proc/uptime`)
- sysfs — read-only device, CPU, memory and power metadata (for example, `/sys/devices/system/cpu`)
- systemd D-Bus interfaces — optional, read-only service and session metadata when available

### 12.3 macOS — Darwin, POSIX, Mach, and Apple system APIs

The following function names are listed alphabetically. Availability and permissions vary by macOS release, sandbox profile and privacy authorization.

- `accept`
- `bind`
- `clock_getres`
- `clock_gettime`
- `close`
- `connect`
- `dispatch_after`
- `dispatch_source_create`
- `dlclose`
- `dlopen`
- `dlsym`
- `fcntl`
- `fstat`
- `fstatfs`
- `getaddrinfo`
- `getcwd`
- `getifaddrs`
- `getnameinfo`
- `getpeername`
- `getpid`
- `getppid`
- `getsockname`
- `getsockopt`
- `gettimeofday`
- `host_info`
- `host_page_size`
- `host_statistics64`
- `kevent`
- `kqueue`
- `listen`
- `lstat`
- `mach_absolute_time`
- `mach_host_self`
- `mach_msg`
- `mach_port_deallocate`
- `mach_task_self`
- `mach_timebase_info`
- `mach_vm_region` — query permitted virtual-memory region metadata only; no memory modification or inspection of unrelated processes
- `mmap`
- `munmap`
- `open`
- `opendir`
- `proc_pidinfo`
- `proc_pidpath`
- `read`
- `readlink`
- `recv`
- `recvfrom`
- `select`
- `send`
- `sendto`
- `stat`
- `statfs`
- `sysctl`
- `sysctlbyname`
- `uname`
- `write`

**Additional macOS frameworks and interfaces (alphabetical):**

- BSD sockets — portable socket and name-resolution interfaces
- Core Foundation — system configuration and property-list types where appropriate
- Dispatch (Grand Central Dispatch) — timers, queues and event sources
- FSEvents — filesystem change notification streams
- I/O Kit — read-only device and power-source metadata
- Mach — monotonic clock, host statistics and permitted task/port metadata
- SystemConfiguration — network service, interface and DNS configuration inspection
- vnode/kqueue — filesystem and descriptor event notification

**Index maintenance rule:** Keep each operating-system list alphabetized whenever entries are added. Distinguish function names from API families and pseudo-filesystem interfaces; identify any state-changing call explicitly, and do not describe a candidate API as implemented until the corresponding backend and tests exist.


## 13. SLeeLa source catalogue and VM element model

The SLeeLa source representation for this inventory lives in `lib/os-level-support/`.
It separates capability vocabulary, reusable element descriptors, the existing VM
system-call bridge, the richer portable C ABI, and the three platform symbol lists.

| SLeeLa source element | Responsibility |
|---|---|
| `SLOsSupportCatalog.sleela` | Canonical common capability groups and portable API/function catalogues |
| `SLOsSupportElement.sleela` | One binding descriptor: group, VM element/opcode, portable symbol, native symbol, backend, effect, status, result and arity |
| `SLOsSupportVMBridge.sleela` | VM dispatch vocabulary, status and marshalling terms, and the current opcode map |
| `SLOsSupportVMElements.sleela` | Factory methods for the 21 existing VM OS-call elements |
| `SLOsSupportOSLSElements.sleela` | Factory methods for the 12 current `osls_*` C ABI elements |
| `SLOsSupportLinux.sleela` | Linux groups and alphabetized POSIX/Linux native symbol catalogue |
| `SLOsSupportWindows.sleela` | Windows groups and alphabetized Win32/Winsock native symbol catalogue |
| `SLOsSupportMacOS.sleela` | macOS groups and alphabetized Darwin/POSIX/Mach native symbol catalogue |
| `examples/SLOsSupportCatalogExample.sleela` | Source example enumerating groups, VM mappings and OS-specific catalogues |

### 13.1 Canonical grouping terms

The shared catalogue uses these semantic group identifiers:

- `HostIdentity`
- `ProcessInspection`
- `ProcessControl`
- `Environment`
- `FileSystem`
- `PathManagement`
- `VolumeStorage`
- `CPUArchitecture`
- `MemoryManagement`
- `TimeClock`
- `TimeZoneCalendar`
- `NetworkInterfaces`
- `NetworkConfiguration`
- `SocketIO`
- `Scheduling`
- `TimersAndWait`
- `RuntimeEvents`
- `CooperativeTasks`
- `DynamicModules`
- `CapabilityDiscovery`
- `ErrorDiagnostics`
- `SecurityBoundary`
- `VMInterop`
- `PlatformExtensions`

The platform classes add qualified group terms such as `LinuxCPUTopology`,
`WindowsServices`, and `MacOSFilesystemEvents`. Use the shared term when the
operation is semantically portable; use a qualified term only for a genuine
platform-specific facility.

### 13.2 Existing VM path and registered osls adapter

The current VM already exposes the OS system-call family through
`SleelaVMSystemCallBridge.sleela`, the `OP_OS_*` opcodes 103–123, and the
native host implementation in `impl/core/sleela_os.h` /
`impl/core/sleela_os.c`. The SLeeLa source element factories map these VM
operations to their current `slos_*` backend symbols and mark them implemented.

The root library's `osls_*` inspection ABI is now registered through
`os-level-support/osls_vm_bridge.c` and append-only opcode `OP_OSLS_CALL` (155).
Twelve stable operation IDs bind the declared APIs through a callable native function-pointer registry. Each entry includes the native symbol, typed argument vector, JSON UTF-8 result type, and required host-read capability. The compiler lowers the
`osls...` built-ins to that opcode; the VM checks each operation's arity and
value types, then returns a bounded JSON String containing native status and
result. Raw pointers and C record layouts do not cross into VM values. The
CMake package and SLeeLa VM Makefile compile the adapter and platform
implementation. CMake's `osls_vm_bridge_test` and `make -C impl test-osls-vm`
exercise the native registry and VM dispatch respectively. Platform-specific
extension catalogues remain separate and unregistered until their own native
bindings and cross-platform tests are added.

All element effects must remain explicit: read-only query, read-only enumeration,
process-local operation, blocking operation, or state-changing operation.
Permission decisions remain with the host OS. Existing process launch/termination,
environment mutation, directory changes and filesystem mutation must not be
silently triggered by catalogue lookup or element construction.

---

**Designation:** Linux, Windows, and macOS are the required target platforms. Common behavior belongs in the portable API; genuine OS differences belong in platform backends or explicitly named extensions. All status labels in this document must be updated as implementation and cross-platform tests are completed.
