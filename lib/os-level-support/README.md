# SLeeLa OS-Level Support Source Package

This directory is the SLeeLa source-language catalogue and VM-facing organization
for the native OS support layer. It is divided into a **general vocabulary**,
**portable/common API**, **VM integration contract**, and **OS-specific backend
catalogues**.

## Source elements

| Source file | Purpose |
|---|---|
| SLPackage.sleela | Package entry point and component names |
| SLOsSupportCatalog.sleela | Canonical capability groups, common API names, runtime/module functions, planned portable functions, and grouping terms |
| SLOsSupportElement.sleela | Reusable element descriptor: group, VM term/opcode, portable symbol, native symbol, backend, effect, status and arity |
| SLOsSupportVMElements.sleela | Factory methods for the 21 existing OP_OS operations, mapped to VM opcodes and slos_* backend symbols |
| SLOsSupportOSLSElements.sleela | Factory methods for the 12 registered osls_* C ABI calls, mapped to OP_OSLS_CALL (155) and operation IDs 0–11 |
| SLOsSupportVMBridge.sleela | VM dispatch stages, status/marshalling vocabulary and current VM opcode mapping |
| examples/SLOsSupportCatalogExample.sleela | Example that enumerates common groups, VM bindings, and each platform's native API catalogue |
| SLOsSupportLinux.sleela | Linux grouping terms and alphabetized POSIX/Linux native symbols |
| SLOsSupportWindows.sleela | Windows grouping terms and alphabetized Win32/Winsock symbols |
| SLOsSupportMacOS.sleela | macOS grouping terms and alphabetized Darwin/POSIX/Mach symbols |
| SLOsSupportLinuxSource.sleela | Linux source facade with VM-backed portable calls and gated Linux extension metadata |
| SLOsSupportWindowsSource.sleela | Windows source facade with VM-backed portable calls and gated Win32/Winsock extension metadata |
| SLOsSupportMacOSSource.sleela | macOS source facade with VM-backed portable calls and gated Darwin/Mach/framework extension metadata |
| SLOsSupportPlatform.sleela | Runtime platform selector for the Linux, Windows and macOS source facades |
| SLOsSupportNative.sleela | Source facade for the 12 VM-callable OSLS functions; returns JSON Strings with OSLS status |

## Canonical grouping vocabulary

The catalog uses stable terms such as:

- HostIdentity, ProcessInspection, ProcessControl, Environment
- FileSystem, PathManagement, VolumeStorage
- CPUArchitecture, MemoryManagement, TimeClock, TimeZoneCalendar
- NetworkInterfaces, NetworkConfiguration, SocketIO
- Scheduling, TimersAndWait, RuntimeEvents, CooperativeTasks
- DynamicModules, CapabilityDiscovery, ErrorDiagnostics
- SecurityBoundary, VMInterop, PlatformExtensions

These are source-level grouping identifiers, not native function names. Use them
as the semantic group of an OS-support element; use the platform catalogues for
native symbols and SLOsSupportElement to describe an individual binding. SLOsSupportVMElements supplies ready-made descriptors for the current VM OS calls. SLOsSupportOSLSElements supplies descriptors for the 12 registered root osls_* calls; each maps to the single OP_OSLS_CALL opcode plus a stable operation ID.

## VM integration boundary

The SLeeLa VM already has an OS system-call path:

- lib/vm/SleelaVMSystemCallBridge.sleela exposes the VM-facing os* calls.
- OP_OS_* opcodes 103–123 are the canonical OS service opcode range.
- impl/core/sleela_os.h and impl/core/sleela_os.c implement the current
  host backend using Win32 on Windows and POSIX plus platform-specific behavior
  on Linux/macOS.

The new source catalogues now reference that existing VM bridge and enumerate
its exact opcode names and numbers. SLOsSupportVMElements maps each existing VM
operation to its OP_OS opcode and the corresponding slos_* C backend symbol.
They do **not** create a second VM syscall path or duplicate opcode numbers.

The root C library under os-level-support/ now has a VM-callable adapter:
os-level-support/osls_vm_bridge.c registers all 12 declared osls_* APIs with stable operation IDs 0–11. Each registry row contains a callable native invocation function, native symbol identity, argument type vector, result type, and required capability mask. The compiler lowers the osls... source built-ins to append-only opcode OP_OSLS_CALL=155. The VM resolves the registry entry, validates arity and each stack value's type, enforces the host-read capability, invokes the registered function pointer, and marshals records and collections into bounded JSON Strings containing an OSLS status code. Native pointers and C struct layouts never cross the VM
boundary. SLOsSupportOSLSElements now records these real bindings rather than
UNREGISTERED.

The CMake package and the SLeeLa VM Makefile both compile the adapter and the
existing OSLS platform implementation. test_osls_vm_bridge exercises native symbol resolution, callable entries, typed argument/result marshalling, denied capabilities, invalid signatures, and bounded-result failures; make -C impl test-osls-vm exercises the VM built-ins and verifies runtime rejection of an invalid argument type.
Windows network-interface enumeration currently reports OSLS_UNSUPPORTED;
unsupported or unavailable platform details remain explicit status results.
## OS-specific source support

The three *Source.sleela classes are executable source-level facades for the
portable operations already exposed by the VM (host/user identity, working
directory, temporary directory, environment access, path existence/type and
file size). Their platform metadata includes path separators, line endings,
executable suffixes, capability groups and native extension symbol lists.
SLOsSupportPlatform detects the host and selects a backend identifier.
It fails closed for an unknown platform.

The common OSLS API is registered and callable through the VM. The separate
nativeExtensionRegistered checks on each platform facade still return false.
Linux procfs/sysfs, netlink, epoll, inotify and cgroups; Windows Win32/IP Helper,
SCM/Registry and Winsock; and macOS Mach/Dispatch/IOKit/FSEvents/SystemConfiguration
remain separate platform-extension families and are not exposed as registered
until their own native binding implementations are added.
## Status and execution rules

- Existing VM OP_OS_* operations are implemented by the current VM bridge.
- The 12 existing osls_* inspection functions are VM-callable through
  OP_OSLS_CALL=155 and the typed operation-ID registry.
- Proposed osls_* extensions remain catalogued as planned, not exported APIs.
- Native symbol lists are backend references, not a claim that every symbol is
  called by SLeeLa today.
- State-changing operations (environment changes, directory changes, file
  mutation, process launch/termination) must be explicit source calls and remain
  subject to host OS permissions.
- Native module loading executes code in-process; it is not a sandbox.
- No kernel hooks, syscall interception, privilege elevation, process injection,
  security bypass, or trust-based execution authorization is introduced.

## Build and verification

The C/C++ runtime support package remains buildable independently:

    cmake -S lib/os-level-support -B build/lib-os-level-support
    cmake --build build/lib-os-level-support
    ctest --test-dir build/lib-os-level-support --output-on-failure

The .sleela catalogue files use the repository's existing SLList source
class. The OSLS VM bridge is compiled into the native runtime and can be exercised
with make -C impl test-osls-vm; run the full platform build and CMake tests on
Linux, Windows and macOS before declaring cross-platform conformance.

## Related native and VM sources

- os-level-support/DESIGNATE.md — complete common and OS-specific function inventory.
- os-level-support/osls.h — portable C metadata/inspection API.
- lib/vm/SleelaVMSystemCallBridge.sleela — existing VM system-call facade and OSLS grouping integration.
- lib/vm/SLVMNativeBinding.sleela — explicit native binding lifecycle model.
- impl/core/sleela_os.h / impl/core/sleela_os.c — existing host OS backend.


## Source-level catalogue example

The example at examples/SLOsSupportCatalogExample.sleela constructs the
catalogue and platform backend objects, then enumerates the common API, VM
bindings and native API lists. The OSLS VM adapter is registered through
OP_OSLS_CALL; platform-extension families remain separately gated.
