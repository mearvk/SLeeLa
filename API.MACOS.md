# SLeeLa — macOS API Reference and Integration Guide

**Project:** SLeeLa  
**Platform:** macOS  
**API family:** Darwin/POSIX, libSystem, Foundation, AppKit, Objective-C Runtime, Core Foundation, Security, IOKit, Network, and native Apple frameworks  
**Purpose:** macOS-native API catalog parallel to API.WIN32.md and API.LINUX.md.

## Scope

macOS exposes several native API layers:

- POSIX/Darwin
- libSystem
- Objective-C Runtime
- Foundation
- AppKit
- Core Foundation/Core Graphics
- Network
- Security
- IOKit
- Metal and other Apple frameworks

Apple documents Foundation as a base framework for data types, persistence, text, dates, networking, XPC, process/thread facilities, and streams/sockets/ports. AppKit provides the principal object-oriented native macOS desktop UI surface. citeturn0search2turn0search10

## API layers

| Layer | Examples | SLeeLa boundary |
|---|---|---|
| POSIX | files, processes, sockets, pthreads | Portable Unix |
| Darwin | kqueue and Mach-related services | macOS native |
| libSystem | native C/system interfaces | C ABI |
| Objective-C Runtime | NSObject, Class, Method, Selector | Runtime boundary |
| Foundation | NSURLSession, NSFileManager, NSProcessInfo | General macOS |
| AppKit | NSApplication, NSWindow, NSView | GUI backend |
| Core Foundation | CFString, CFData, CFRunLoop | C system layer |
| Security | Keychain, trust, authorization | Security backend |
| IOKit | device/hardware services | Hardware boundary |
| Network | connections, listeners, paths | Network backend |
| Metal | GPU/compute | Graphics backend |

Apple documents the Objective-C Runtime as exposing root types plus runtime functions and data structures supporting dynamic Objective-C behavior. citeturn0search4

## Processes and applications

POSIX/Darwin calls include fork, execve, posix_spawn, waitpid, _exit, getpid, getppid, kill, getuid, geteuid, getgid, and getegid.

Foundation and AppKit add NSProcessInfo, NSApplication, and NSRunningApplication. Apple documents NSApplication as managing the main application event loop and NSRunningApplication as representing a running application instance. citeturn0search11

## Threads and concurrency

Relevant native surfaces include pthread_create, pthread_join, pthread_detach, pthread mutexes, condition variables, read/write locks, thread-local storage, Grand Central Dispatch/libdispatch, dispatch queues, dispatch sources, dispatch timers, and Foundation concurrency APIs.

SLeeLa should keep pthread/libdispatch details inside the macOS adapter.

## Memory

Native facilities include mmap, munmap, mprotect, madvise, shared-memory interfaces, and lower-level Mach virtual-memory interfaces where explicitly required.

SLeeLa's managed memory remains the language/runtime policy. The current memory manager default is 2 GiB, configurable down to 256 MiB. Native mappings must not silently bypass that policy.

## Filesystem and I/O

POSIX/Darwin functions include open, openat, close, read, write, pread, pwrite, readv, writev, lseek, fsync, fcntl, stat, fstat, lstat, mkdir, unlink, rename, link, symlink, and readlink.

Foundation provides higher-level objects including NSData, NSString, NSURL, NSFileManager, NSStream, and related services.

SLeeLa should distinguish descriptor-level POSIX I/O from Foundation object APIs.

## Foundation

Foundation is a principal general-purpose macOS framework. Apple documents data types, collections, persistence, text, date/time, networking, XPC, process/thread services, and streams/sockets/ports. citeturn0search2

Representative classes include:

- NSObject
- NSString and NSMutableString
- NSData and NSMutableData
- NSArray
- NSDictionary
- NSSet
- NSDate
- NSURL
- NSURLRequest
- NSURLSession
- NSFileManager
- NSProcessInfo
- NSUserDefaults
- NSNotificationCenter
- NSOperation
- NSRunLoop
- NSStream
- NSPipe
- NSLock

Foundation functions, constants, protocols, and class methods should be cataloged separately.

## Networking

BSD/POSIX calls include socket, socketpair, bind, listen, accept, connect, shutdown, send, sendto, sendmsg, recv, recvfrom, recvmsg, getsockopt, setsockopt, getaddrinfo, and freeaddrinfo.

Higher-level Apple networking includes NSURLSession and Network.framework.

SLeeLa should use the portable socket abstraction where possible and a macOS Network/Foundation adapter for Apple-specific features.

## Event loops and timers

Relevant facilities include select, poll, kqueue, kevent, Grand Central Dispatch, dispatch queues, dispatch sources, dispatch timers, and NSRunLoop.

kqueue/kevent is a Darwin-specific event facility and should remain an implementation detail of the macOS event backend.

## Time

Relevant interfaces include clock_gettime, clock_getres, nanosleep, gettimeofday, Foundation date/time APIs, and monotonic timing facilities.

SLeeLa must distinguish wall-clock time from monotonic duration measurement.

## Interprocess communication

macOS supports pipes, Unix-domain sockets, shared memory, POSIX semaphores, signals, Mach ports, and XPC.

XPC should be exposed through a bounded SLeeLa IPC abstraction rather than arbitrary service invocation.

## Dynamic libraries

The Darwin loader includes dlopen, dlsym, dlclose, and dlerror.

Module paths and symbol ownership must be validated before native code is loaded.

## Security and identity

Relevant surfaces include POSIX permissions and ownership, Security framework, Keychain Services, certificate/trust evaluation, code-signing information, sandbox entitlements, authorization services, and Secure Enclave-related APIs.

SLeeLa must respect sandboxing, entitlements, user consent, Keychain access controls, and system authorization boundaries.

## Objective-C Runtime

Important runtime concepts include NSObject, Protocol, Class, metaclasses, Selector, Method, message dispatch, runtime introspection, associated objects, and method lookup.

Apple documents classes, protocols, functions, structures, constants, data types, macros, and enumerations for the runtime. citeturn0search4

SLeeLa should not expose unrestricted runtime selector invocation to untrusted source.

## AppKit and GUI

AppKit is the principal object-oriented native macOS desktop GUI framework. Apple documents AppKit as a comprehensive macOS GUI API and documents its interoperability with SwiftUI. citeturn0search10turn0search14

Representative classes include:

- NSApplication
- NSApplicationDelegate
- NSWindow
- NSView
- NSViewController
- NSWindowController
- NSEvent
- NSMenu
- NSMenuItem
- NSButton
- NSTextField
- NSTextView
- NSImage
- NSWorkspace
- NSRunningApplication

AppKit's application environment includes application lifecycle and workspace services. citeturn0search11

SLeeLa's GUI adapter should preserve AppKit object ownership, main-thread requirements, event dispatch, and Objective-C/Swift interoperability outside the portable core.

## SwiftUI

SwiftUI is Apple's declarative UI framework. It should be treated as an optional macOS presentation layer rather than a replacement for the platform-neutral SLeeLa GUI contract.

Apple provides AppKit/SwiftUI interoperability mechanisms including hosting controllers and representable wrappers. citeturn0search14

## Core Foundation and Core Graphics

Representative C-based surfaces include CFString, CFData, CFArray, CFDictionary, CFRunLoop, CFURL, CGContext, CGColor, CGImage, and CGPath.

Core Foundation ownership rules must be preserved. Core Graphics objects must be released according to documented ownership conventions.

## Devices and hardware

IOKit and related frameworks provide services for USB, HID, storage, power management, displays, network interfaces, and hardware/service notifications.

Hardware APIs are privilege- and entitlement-sensitive. SLeeLa should expose specific device contracts rather than unrestricted registry traversal.

## Graphics and GPU

macOS native graphics surfaces include Core Graphics, Core Animation, Core Image, and Metal.

Metal is the native Apple GPU API. SLeeLa should isolate Metal resource and command-queue ownership behind a graphics backend.

## Documents, preferences, and pasteboard

Representative APIs include NSFileManager, NSUserDefaults, NSDocument, NSDocumentController, NSPersistentDocument, and NSPasteboard.

Apple documents NSDocument/NSDocumentController for document management and NSPasteboard for pasteboard data transfer. citeturn0search6

## XPC and services

XPC provides Apple-native interprocess communication and service isolation.

SLeeLa should model connection lifecycle, request/reply, interruption/invalidation, serialization, service identity, and authorization.

Untrusted source must not receive unrestricted access to arbitrary XPC services.

## Architecture and ABI

SLeeLa should support arm64 Apple silicon and x86_64 where supported by the selected macOS deployment target.

Native code must not assume pointer size, structure packing, Objective-C object layout, CPU instruction set, alignment, byte order, or framework availability.

Universal/fat binaries are packaging concerns; shared source should remain architecture-neutral unless a documented ABI requirement requires otherwise.

## Version targeting

The backend should distinguish deployment target, SDK version, framework availability, runtime OS version, and architecture support.

For optional APIs:

1. compile against the intended SDK;
2. record the deployment target;
3. perform availability checks;
4. provide controlled fallbacks;
5. fail clearly when a required feature is unavailable.

## Exposed macOS methods, functions, classes, and protocols

| Field | Purpose |
|---|---|
| API name | Function, class, selector/method, protocol, or operation |
| Framework/library | Foundation, AppKit, Security, etc. |
| Header/module | Declaration source |
| Parameters | Native parameters |
| Return value | Native result |
| Error behavior | NSError, OSStatus, errno, return status, etc. |
| Minimum macOS | Availability |
| Architecture | arm64/x86_64 |
| Ownership | ARC/CF/native resource lifetime |
| Main-thread rule | UI/thread affinity |
| Entitlement/security | Sandbox/TCC/Keychain/device requirements |
| SLeeLa binding | SLeeLa API/class |
| Status | Implemented/planned/unsupported/deprecated |

Apple's documentation exposes Foundation classes, protocols, data types, constants, and functions; AppKit exposes classes and methods; Objective-C Runtime exposes classes, protocols, functions, and runtime data structures. citeturn0search2turn0search4

## Representative callable surfaces

### Foundation

NSObject; NSString/NSData; NSArray/NSDictionary/NSSet; NSURL/NSURLSession; NSFileManager; NSProcessInfo; NSUserDefaults; NSRunLoop/NSOperation.

### AppKit

NSApplication; NSWindow; NSView; NSViewController; NSEvent; NSMenu/NSMenuItem; NSWorkspace; NSRunningApplication.

### Objective-C Runtime

NSObject; Protocol; Class; Selector; Method; runtime lookup; runtime metadata.

## Platform mapping

| SLeeLa facility | macOS native layer |
|---|---|
| Files | POSIX/Darwin + Foundation |
| Directories | POSIX/Darwin + NSFileManager |
| Processes | POSIX/Darwin + Foundation/AppKit |
| Threads | pthreads + libdispatch |
| Memory | mmap/mprotect + managed runtime |
| Networking | BSD sockets + Network/Foundation |
| Event loop | kqueue + libdispatch + RunLoop |
| IPC | Unix sockets + XPC |
| Dynamic libraries | dlopen/dlsym |
| Security | Security/Keychain/sandbox |
| GUI | AppKit + optional SwiftUI |
| Graphics | Core Graphics/Core Animation/Metal |
| Devices | IOKit |
| Time | POSIX/Darwin + Foundation |
| Preferences | NSUserDefaults |
| Documents | AppKit/Foundation |
| Pasteboard | NSPasteboard |

## Security boundary

Native macOS calls should validate input, check NSError/OSStatus/errno and native return values, preserve ownership rules, obey AppKit main-thread requirements, respect sandbox and entitlements, avoid arbitrary Objective-C selector invocation, validate dynamic-library paths, avoid credentials in command-line arguments, use Keychain/security APIs for protected credentials, isolate device operations, and distinguish user consent from authorization.

## SLeeLa implementation rule

Documenting macOS does not automatically expose every Apple function, class, selector, or framework.

A native macOS surface becomes a SLeeLa API only after a defined SLeeLa signature, parameter validation, return/error translation, ownership/resource rules, thread-affinity rules, architecture validation, deployment-target behavior, security/entitlement handling, tests, examples, and documentation.

## Primary references

- Apple macOS development: https://developer.apple.com/macos/
- Foundation: https://developer.apple.com/documentation/foundation
- Objective-C Runtime: https://developer.apple.com/documentation/objectivec
- AppKit: https://developer.apple.com/documentation/appkit
- AppKit/SwiftUI integration: https://developer.apple.com/documentation/swiftui/appkit-integration
- AppKit application environment: https://developer.apple.com/documentation/appkit/app-and-environment
- AppKit documents/data/pasteboard: https://developer.apple.com/documentation/appkit/documents-data-and-pasteboard

## Status

This document defines the macOS native API boundary relevant to SLeeLa. It is a catalog and integration policy, not a claim that every listed framework, class, method, or function is implemented.

---

**SLeeLa — macOS API Reference**  
**MEARVK LLC — 2026**
