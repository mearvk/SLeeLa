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
| SLOsSupportOSLSElements.sleela | Factory methods for the 12 common osls_* C ABI exports; VM opcode remains UNREGISTERED until an adapter is implemented |
| SLOsSupportVMBridge.sleela | VM dispatch stages, status/marshalling vocabulary and current VM opcode mapping |
| examples/SLOsSupportCatalogExample.sleela | Example that enumerates common groups, VM bindings, and each platform's native API catalogue |
| SLOsSupportLinux.sleela | Linux grouping terms and alphabetized POSIX/Linux native symbols |
| SLOsSupportWindows.sleela | Windows grouping terms and alphabetized Win32/Winsock symbols |
| SLOsSupportMacOS.sleela | macOS grouping terms and alphabetized Darwin/POSIX/Mach symbols |
| SLOsSupportLinuxSource.sleela | Linux source facade with VM-backed portable calls and gated Linux extension metadata |
| SLOsSupportWindowsSource.sleela | Windows source facade with VM-backed portable calls and gated Win32/Winsock extension metadata |
| SLOsSupportMacOSSource.sleela | macOS source facade with VM-backed portable calls and gated Darwin/Mach/framework extension metadata |
| SLOsSupportPlatform.sleela | Runtime platform selector for the Linux, Windows and macOS source facades |

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
native symbols and SLOsSupportElement to describe an individual binding. SLOsSupportVMElements supplies ready-made descriptors for the current VM OS calls. SLOsSupportOSLSElements supplies descriptors for the richer root osls_* API while explicitly marking its VM binding as unregistered.

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

The richer root C library under os-level-support/ has its own osls_* metadata
and inspection ABI. The source model explicitly distinguishes the existing
OP_OS_* bridge from a future osls_* VM adapter:
coreOSOpcodeBridgeImplemented() is true, while
oslsCAbiAdapterRegistered() is false until a real host registration,
marshalling implementation and cross-platform tests are added. The new
SLOsSupportOSLSElements class gives each current osls_* export a source element
with group, native symbol, arity, result contract and implementation status;
its VM opcode field is deliberately UNREGISTERED.

## OS-specific source support\n\nThe three `*Source.sleela` classes are executable source-level facades for the\nportable operations already exposed by the VM (`osHostName`, `osUserName`,\n`osCurrentDir`, `osTempDir`, environment access, path existence/type and file\nsize). Their platform-specific metadata includes path separator, line ending,\nexecutable suffix, platform capability groups and native extension symbol lists.\n`SLOsSupportPlatform` detects the host through `osPlatform()` and selects a\nbackend identifier. It fails closed for an unknown platform.\n\nThe facades do not pretend that every listed native extension is callable: their\n`nativeExtensionRegistered` checks currently return false. Linux procfs/sysfs,\nnetlink, epoll, inotify and cgroups; Windows Win32/IP Helper/SCM/Registry/Winsock;\nand macOS Mach/Dispatch/IOKit/FSEvents/SystemConfiguration need explicit native\nbindings, ABI marshalling, platform builds and tests before dispatch.\n\n## Status and execution rules

- Existing VM OP_OS_* operations are implemented by the current VM bridge.
- Existing osls_* functions are declared in the root native library; they are
  not automatically VM-callable merely because they appear in the catalogue.
- Proposed osls_* extensions are catalogued as planned, not exported APIs.
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
class. They should be checked with the repository's current sleelvac compiler
and executed with the SLeeLa VM in the normal source-library verification flow.
This change does not claim a successful SLeeLa compile/run unless those checks
are actually run.

## Related native and VM sources

- os-level-support/DESIGNATE.md — complete common and OS-specific function inventory.
- os-level-support/osls.h — portable C metadata/inspection API.
- lib/vm/SleelaVMSystemCallBridge.sleela — existing VM system-call facade and OSLS grouping integration.
- lib/vm/SLVMNativeBinding.sleela — explicit native binding lifecycle model.
- impl/core/sleela_os.h / impl/core/sleela_os.c — existing host OS backend.


## Source-level catalogue example

The example at examples/SLOsSupportCatalogExample.sleela constructs the
catalogue and the three platform backend objects, then enumerates the common
API, VM element bindings and native API lists. It also prints the two distinct
integration states: the existing OP_OS bridge is implemented, while the
additional osls C ABI adapter is not yet registered.
