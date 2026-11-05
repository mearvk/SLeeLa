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
| Path handling | Normalize paths, resolve relative paths, join/split components, inspect roots/volumes, and query canonical paths where supported | **Planned** |
| CPU and machine information | Logical/physical processor counts, architecture, vendor/model, cache-line information, and total/available memory | **Partial** — `osls_get_cpu_info`; several fields are currently reported as unknown or approximated on some platforms |
| Memory information | Total, available, used and page-size information; process working-set/resident memory where available | **Partial** — total memory is included in `osls_get_cpu_info`; a dedicated memory API is planned |
| Time and clock | Wall-clock time, monotonic time, high-resolution counter, clock frequency/resolution, and UTC/local conversion | **Partial** — system wall-clock and monotonic nanoseconds are returned by `osls_get_system_info` |
| Timezone and calendar | Timezone identifier/abbreviation, UTC offset, daylight-saving state, local-time conversion, and calendar data | **Implemented with portability limits** — `osls_get_timezone_info`; identifier fidelity differs by platform |
| Network interface inspection | Enumerate interfaces and report names, link state, loopback status, MAC address, IPv4 and IPv6 addresses | **Partial** — `osls_get_network_interfaces`; current Windows implementation returns `OSLS_UNSUPPORTED`, and address-family details require completion |
| Network configuration inspection | Read DNS resolver configuration, routes, interface addresses, gateways, MTU and interface statistics | **Planned** — read-only inspection APIs |
| Socket and network I/O | Create/close sockets; bind/listen/accept/connect; send/receive; poll readiness; resolve hostnames; inspect socket endpoints | **Planned** — portable Winsock/POSIX facade with explicit nonblocking and timeout semantics |
| Scheduling expressions | Validate five-field cron expressions and calculate the next local-time occurrence | **Implemented** — `osls_cron_validate`, `osls_cron_next_after`; calculation only, not OS schedule installation |
| Timers and waiting | Monotonic timers, deadlines, sleep, waitable events and cancellation primitives | **Planned** — distinguish process-local timers from OS-managed persistent schedules |
| Runtime event hooks | Register/unregister callbacks, emit in-process events, and dispatch event payloads | **Implemented** — `osls_runtime_subscribe`, `osls_runtime_unsubscribe`, `osls_runtime_emit`; these are library callbacks, not operating-system hooks |
| Cooperative task queue | Post callbacks, run one queued task, and inspect pending task count | **Implemented** — `osls_runtime_post`, `osls_runtime_run_one`, `osls_runtime_pending`; caller pumps the queue |
| Dynamic module loading | Open a trusted shared library, verify the declared ABI version, resolve the declared entry point, call it and close it | **Implemented** — `sl_native_open`, `sl_native_run_module`, `sl_native_close`; native code runs in-process and is not sandboxed |
| OS and runtime capabilities | Query whether an operation is available, permitted, and supported by the current backend | **Planned** — capability discovery must be separate from attempting the operation |
| Error and status reporting | Stable portable status codes plus native error detail for diagnostics | **Partial** — base status enums exist; consistent native-error capture and per-operation detail are planned |

## 3. Linux-specific function families

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

## 4. Windows-specific function families

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

## 5. macOS-specific function families

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

## 6. Platform capability comparison

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

## 7. C ABI and C++ facade requirements

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

## 8. Security and authority boundaries

The OS-level-support library is a user-mode portability layer. Its functions report host state or perform explicitly requested, ordinary user-mode operations. They do not confer authority.

The following are out of scope for this library and must not be added as implied consequences of a successful metadata or trust check:

- kernel memory access, kernel hooks, syscall interception, or stealth mechanisms;
- driver or kernel-extension installation, removal, or control;
- process injection, credential theft, security-control bypass, or unauthorized cross-process access;
- automatic privilege elevation or enabling token privileges;
- trust-based execution authorization or automatic trust-root installation;
- silent changes to registry, system configuration, firewall, network routes, services, startup entries, or persistent schedules.

Any future operation that writes files, changes settings, starts a process, or modifies a service must be separately specified, explicitly invoked by the caller, permission-checked by the OS, and documented as a state-changing operation. It must not be hidden behind a read/query API.

## 9. Implementation priorities

1. **Complete and correct the existing common API:** finish Windows network-interface enumeration; improve CPU vendor/model/topology fields; improve process metadata consistency; validate timezone identifiers and timestamp semantics.
2. **Add capability and error reporting:** expose backend capabilities and native diagnostic codes consistently.
3. **Complete common read-only inspection:** dedicated memory, path, volume, mount, route, DNS, socket and interface-statistics APIs.
4. **Add event facilities:** filesystem notifications and portable wait/timer abstractions with OS-specific backends.
5. **Add carefully scoped platform extensions:** Linux procfs/sysfs/cgroup/service metadata; Windows volume/service/registry-read/power metadata; macOS volume/FSEvents/launchd/power metadata.
6. **Add tests on all three operating systems:** compile-time coverage, runtime tests, unavailable-permission tests, Unicode paths, large buffers, missing facilities, and error mapping.
7. **Publish an implementation matrix:** each function must identify its header, backend source, supported platforms, tests, permission requirements and current status before being described as complete.

## 10. Current public API reference

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

The runtime callback/task API is declared in `runtime/osls_runtime.h`. The native shared-module API is declared in `loader/native_loader.h`. Their presence does not imply sandboxing, OS syscall interception, persistent scheduling, or process creation.

---

**Designation:** Linux, Windows, and macOS are the required target platforms. Common behavior belongs in the portable API; genuine OS differences belong in platform backends or explicitly named extensions. All status labels in this document must be updated as implementation and cross-platform tests are completed.
