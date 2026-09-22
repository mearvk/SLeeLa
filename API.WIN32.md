# SLeeLa — Win32 API Reference and Integration Guide

**Project:** SLeeLa  
**Platform:** Microsoft Windows 10+  
**API family:** Win32 / Windows API  
**Architecture:** 32-bit and 64-bit Windows, including x64/AMD64  
**Document purpose:** Reference the Windows native API surface relevant to SLeeLa and record authoritative Microsoft documentation for implementation.

## 1. Scope

The Win32 API is Microsoft's native Windows API for desktop applications. Microsoft now commonly describes this as the **Windows API**; the documentation notes that the name reflects support for both historical 16-bit roots and 64-bit Windows.

For SLeeLa, Win32 is the principal native Windows backend for direct operating-system services, C/C++ integration, desktop functionality, networking, files, processes, threads, security, and system resources.

Microsoft's primary references:

- Win32 API reference: https://learn.microsoft.com/en-us/windows/win32/api/
- Windows API index: https://learn.microsoft.com/en-us/windows/win32/apiindex/windows-api-list
- Desktop Win32 programming: https://learn.microsoft.com/en-us/windows/win32/desktop-programming
- Windows API index portal: https://learn.microsoft.com/en-us/windows/win32/apiindex/api-index-portal
- Windows headers: https://learn.microsoft.com/en-us/windows/win32/winprog/using-the-windows-headers
- Windows API sets: https://learn.microsoft.com/en-us/windows/win32/apiindex/windows-apisets

## 2. 32-bit and 64-bit support

Win32 source is not synonymous with 32-bit-only source. Microsoft states that the Win32 API can be used on both 32-bit and 64-bit Windows.

The Windows headers are specifically designed to permit 32-bit and 64-bit applications to be built from a common source code base. Windows supplies data types and declarations intended to account for differences in pointer and word size.

SLeeLa should therefore maintain a common Windows source layer wherever the API contract is architecture-neutral and explicitly test/build both architectures where supported.

Important compiler/platform distinctions include:

- `_WIN32` identifies the Windows API compilation environment and is also defined for 64-bit Windows builds.
- `_WIN64` identifies a 64-bit Windows compilation environment.
- Microsoft compiler architecture macros such as `_M_X64` identify x64 compilation.
- Pointer-sized and Windows-defined types should be preferred over hard-coded 32-bit integer assumptions.

SLeeLa should not create duplicate Win32 and Win64 implementations merely because the target architecture differs. Architecture-specific source should be introduced only where ABI, instruction set, data layout, calling convention, or operating-system behavior actually requires it.

## 3. Major Win32 API areas relevant to SLeeLa

### User interface and desktop

Relevant areas include:

- Windows and messages
- Windows controls
- Menus and resources
- Windows Shell
- Accessibility
- Internationalization
- Desktop window management
- User input

These APIs provide the native foundation for SLeeLa GUI/application integration.

### Processes and threads

Relevant header families include:

- `processthreadsapi.h`
- Process creation and management
- Thread creation and management
- Thread synchronization
- Process information
- Thread information
- APC-related facilities
- Process and thread security

SLeeLa's process/server facilities can use this layer for native Windows process execution and lifecycle management.

### Memory management

Win32 provides native virtual-memory and process-memory facilities.

SLeeLa's managed memory layer remains an SLeeLa runtime responsibility; Win32 memory APIs should be treated as the operating-system allocation/protection boundary rather than as a replacement for SLeeLa's managed-memory rules.

Architecture-neutral Windows types must be used when memory addresses or sizes cross the native boundary.

### File and storage I/O

Win32 supplies native file, directory, storage, mapping, and I/O APIs.

Important implementation concerns for SLeeLa include:

- Unicode paths
- File handles
- File sharing modes
- Access rights
- File attributes
- Directory operations
- File mappings
- Asynchronous/overlapped I/O where required
- Windows error reporting

### Networking

Windows Sockets 2 (Winsock) is the principal native socket interface.

Typical facilities include:

- `WSAStartup`
- `socket`
- `bind`
- `listen`
- `accept`
- `connect`
- `send`
- `recv`
- `closesocket`
- `getsockopt`
- `setsockopt`
- address conversion and resolution
- asynchronous/network event facilities

SLeeLa currently identifies Winsock2 as its Windows networking backend.

Other Microsoft networking surfaces include:

- WinHTTP
- HTTP Server API
- IP Helper
- RPC
- Network Management
- Windows Internet APIs

### Security and identity

Microsoft's Win32 security surface includes:

- Authentication
- Authorization
- Access control
- Security descriptors
- ACLs
- Cryptography
- CNG
- Certificate services
- Credential facilities
- TPM Base Services
- Security auditing
- Identity and directory services

SLeeLa should use the Windows security boundary rather than bypassing operating-system authorization.

### Dynamic libraries

Windows provides native dynamic-library loading facilities, including the `LoadLibrary` family.

SLeeLa's Windows configuration identifies dynamic loading through the Win32 loader interface.

Native library loading must be treated as a security boundary: paths should be controlled, loaded modules should be validated where appropriate, and arbitrary untrusted module loading should not be permitted.

### Console and terminal

Modern Windows provides native console and pseudoconsole facilities, including ConPTY.

SLeeLa can use the Windows console APIs for terminal applications and pseudoterminal integration.

### Time and system information

Win32 provides APIs for:

- system time
- high-resolution performance timing
- timers
- system information
- processor information
- operating-system information

SLeeLa should use Windows-defined time and size types and avoid assuming that timing units or integer widths are identical across platforms.

### COM and system services

Microsoft documents COM as a native component system used for binary software components and interprocess/distributed object interaction.

Other system-service areas include:

- Dynamic-link libraries
- Memory management
- Power management
- Secure Enclaves
- Thread coordination
- Windows services
- Windows messaging
- System information
- Compression

SLeeLa should add COM interfaces only where a Windows-specific feature actually requires them; the core language/runtime should remain platform-neutral.

## 4. Windows API sets

Windows 10 and later organize portions of Win32 into **API sets**.

An API-set contract is not necessarily the physical DLL containing an implementation. Microsoft describes API sets as a separation between the functional contract and the host DLL implementation.

For SLeeLa this means:

1. Prefer documented API contracts.
2. Do not assume an API-set contract name corresponds to a physical DLL file.
3. Use documented availability mechanisms when runtime feature detection is required.
4. Keep the Windows backend separated from the platform-neutral SLeeLa runtime.

This is particularly useful when SLeeLa needs to support multiple Windows versions or Windows device environments.

## 5. Windows headers

The Windows SDK supplies the header declarations required to compile Windows applications.

Microsoft documents that Windows headers:

- support 32-bit and 64-bit applications;
- provide Unicode and ANSI API declarations;
- provide data types intended for both 32-bit and 64-bit source builds;
- support conditional declarations through platform/version macros;
- provide header annotations and type checking facilities.

SLeeLa should normally use Unicode-capable Windows APIs and avoid introducing new ANSI-only dependencies.

## 6. SLeeLa platform mapping

The SLeeLa Windows backend should map its platform-neutral facilities approximately as follows:

| SLeeLa facility | Windows native layer |
|---|---|
| Files | Win32 file APIs |
| Directories | Win32 file/directory APIs |
| Processes | Process APIs |
| Threads | Thread APIs |
| Synchronization | Windows synchronization APIs |
| Memory boundary | Virtual/process memory APIs |
| Networking | Winsock 2 |
| HTTP | WinHTTP / HTTP Server API where appropriate |
| Dynamic libraries | Windows loader APIs |
| Terminal | Console / ConPTY |
| GUI | Windows windowing/message/control APIs |
| Security | Windows Security/Identity APIs |
| Cryptography | Windows CNG and related APIs |
| Time | Windows time/performance APIs |
| Services | Windows Service Control Manager APIs |
| Components | COM where required |
| System information | Windows system-information APIs |

This table is an architectural mapping, not a claim that every facility has already been implemented in SLeeLa.

## 7. x64/AMD64 requirements for SLeeLa

SLeeLa's Windows build should explicitly support x64/AMD64.

The source should:

- avoid pointer-to-`int` truncation;
- use `size_t`, `ptrdiff_t`, `intptr_t`, `uintptr_t`, or Windows pointer-sized types where appropriate;
- avoid assumptions that handles fit in 32-bit integers;
- avoid hard-coded structure sizes;
- avoid hard-coded pointer alignment;
- test serialization formats independently from native structure layout;
- distinguish API compatibility from compiler/ABI compatibility;
- compile the same common source for x64 whenever no architecture-specific implementation is required.

A dedicated x64 build configuration may be useful even when the source remains shared.

## 8. Windows version targeting

SLeeLa's Windows backend targets Windows 10 and later.

Platform/version declarations should be made deliberately through the Windows SDK's supported target-version mechanisms rather than by assuming that every API exists on every Windows release.

When a newer API is optional, SLeeLa should:

1. identify the documented minimum Windows version;
2. compile against the appropriate SDK declarations;
3. perform runtime availability checks where necessary;
4. provide a controlled fallback when the feature is optional;
5. fail clearly when a required capability is unavailable.

## 9. API documentation strategy

SLeeLa's `/api` documentation should distinguish:

- SLeeLa API contracts;
- native Windows implementation details;
- Microsoft Win32 dependencies;
- architecture requirements;
- Windows version requirements.

The Microsoft documentation should remain the authoritative external reference for Windows API semantics. SLeeLa documentation should explain how SLeeLa maps its own abstractions onto those APIs.

## 10. Security considerations

Win32 is a powerful native interface. SLeeLa should therefore treat native calls as privileged implementation boundaries.

Recommended rules:

- validate all external input before native calls;
- use Unicode APIs consistently;
- check return values and Windows error codes;
- close native handles deterministically;
- do not expose arbitrary native function execution through the SLeeLa language;
- validate DLL/library paths before loading;
- do not place credentials in command-line arguments;
- use OS security facilities for authorization;
- preserve least-privilege behavior;
- distinguish administrative operations from ordinary application operations;
- keep platform-specific native code isolated from portable SLeeLa code.

## 11. Primary Microsoft reference categories

The Microsoft Win32 reference is organized into broad technology areas including:

- User interface
- Windows Shell
- User input and messaging
- Data access and storage
- Diagnostics
- Graphics and multimedia
- Devices
- System services
- Security and identity
- Application installation and servicing
- System administration and management
- Networking and Internet
- Legacy/deprecated APIs

The complete reference should be treated as a catalog rather than as a requirement for SLeeLa to implement every Windows API.

## 12. Selected official references

Microsoft Learn:

- Win32 API reference: https://learn.microsoft.com/en-us/windows/win32/api/
- Windows API index: https://learn.microsoft.com/en-us/windows/win32/apiindex/windows-api-list
- Win32 desktop programming: https://learn.microsoft.com/en-us/windows/win32/desktop-programming
- API Index portal: https://learn.microsoft.com/en-us/windows/win32/apiindex/api-index-portal
- Using the Windows headers: https://learn.microsoft.com/en-us/windows/win32/winprog/using-the-windows-headers
- Windows API sets: https://learn.microsoft.com/en-us/windows/win32/apiindex/windows-apisets
- Processes and threads: https://learn.microsoft.com/en-us/windows/win32/api/_processthreadsapi/
- Security and Identity: https://learn.microsoft.com/en-us/windows/win32/api/_security/
- System Services: https://learn.microsoft.com/en-us/windows/win32/system-services
- Windows Sockets: https://learn.microsoft.com/en-us/windows/win32/api/winsock/

## 13. Status

This document records the Microsoft Win32/Windows API surface relevant to SLeeLa as an architectural and implementation reference.

It does **not** claim that SLeeLa currently implements every API listed here.

Future SLeeLa work should add concrete API bindings under `/api` only when the corresponding SLeeLa interface, implementation, validation, error handling, tests, examples, and platform behavior are defined.

---

**SLeeLa — Win32 API Reference**  
**MEARVK LLC — 2026**
