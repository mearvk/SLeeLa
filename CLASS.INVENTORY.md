# SLeeLa Class Inventory

**SLeeLa Version:** 0.3.0-dev  
**Inventory Revision:** 1.0  
**Inventory Branch:** `main`  
**Inventory Date:** 2026-09-28  
**Unique Class Files (contract-verified): 47**

> This document is the authoritative starting inventory for the SLeeLa 0.3.0-dev development line. The **47 unique class files** at the top of this document are the C++ foundational class headers directly included and contract-checked by `test-suites/cpp/test_class_contracts.cpp`. This is a verified class-file count, not a claim that no other repository source file contains a class, struct, interface, or class-like declaration.

## 1. Verified Foundational Class Files

The following 47 unique class files are explicitly included by the current foundational class-contract test:

| # | Class | Source |
|---:|---|---|
| 1 | Buffer | `impl/fundamental/Buffer.hpp` |
| 2 | ByteStream | `impl/fundamental/ByteStream.hpp` |
| 3 | CancellationToken | `impl/fundamental/CancellationToken.hpp` |
| 4 | Client | `impl/fundamental/Client.hpp` |
| 5 | Clock | `impl/fundamental/Clock.hpp` |
| 6 | Condition | `impl/fundamental/Condition.hpp` |
| 7 | Configuration | `impl/fundamental/Configuration.hpp` |
| 8 | Connection | `impl/fundamental/Connection.hpp` |
| 9 | Deserializer | `impl/fundamental/Deserializer.hpp` |
| 10 | Directory | `impl/fundamental/Directory.hpp` |
| 11 | DnsResolver | `impl/fundamental/DnsResolver.hpp` |
| 12 | EnumDescriptor | `impl/fundamental/EnumDescriptor.hpp` |
| 13 | Environment | `impl/fundamental/Environment.hpp` |
| 14 | Error | `impl/fundamental/Error.hpp` |
| 15 | Event | `impl/fundamental/Event.hpp` |
| 16 | EventBus | `impl/fundamental/EventBus.hpp` |
| 17 | File | `impl/fundamental/File.hpp` |
| 18 | Future | `impl/fundamental/Future.hpp` |
| 19 | Handler | `impl/fundamental/Handler.hpp` |
| 20 | Identifier | `impl/fundamental/Identifier.hpp` |
| 21 | Input | `impl/fundamental/Input.hpp` |
| 22 | Listener | `impl/fundamental/Listener.hpp` |
| 23 | Mutex | `impl/fundamental/Mutex.hpp` |
| 24 | Name | `impl/fundamental/Name.hpp` |
| 25 | NetworkEndpoint | `impl/fundamental/NetworkEndpoint.hpp` |
| 26 | Option | `impl/fundamental/Option.hpp` |
| 27 | Output | `impl/fundamental/Output.hpp` |
| 28 | Parameter | `impl/fundamental/Parameter.hpp` |
| 29 | Path | `impl/fundamental/Path.hpp` |
| 30 | Process | `impl/fundamental/Process.hpp` |
| 31 | Resource | `impl/fundamental/Resource.hpp` |
| 32 | Result | `impl/fundamental/Result.hpp` |
| 33 | Router | `impl/fundamental/Router.hpp` |
| 34 | Scheduler | `impl/fundamental/Scheduler.hpp` |
| 35 | Schema | `impl/fundamental/Schema.hpp` |
| 36 | SecureChannel | `impl/fundamental/SecureChannel.hpp` |
| 37 | Serializer | `impl/fundamental/Serializer.hpp` |
| 38 | Server | `impl/fundamental/Server.hpp` |
| 39 | Service | `impl/fundamental/Service.hpp` |
| 40 | Session | `impl/fundamental/Session.hpp` |
| 41 | Status | `impl/fundamental/Status.hpp` |
| 42 | Task | `impl/fundamental/Task.hpp` |
| 43 | TcpTransport | `impl/fundamental/TcpTransport.hpp` |
| 44 | Thread | `impl/fundamental/Thread.hpp` |
| 45 | Timer | `impl/fundamental/Timer.hpp` |
| 46 | Transport | `impl/fundamental/Transport.hpp` |
| 47 | TypeDescriptor | `impl/fundamental/TypeDescriptor.hpp` |
| 48 | UdpTransport | `impl/fundamental/UdpTransport.hpp` |
| 49 | Validator | `impl/fundamental/Validator.hpp` |
| 50 | Version | `impl/fundamental/Version.hpp` |

**Important count correction:** the current contract file contains **50** `CHECK(...)` declarations, not 47. Therefore the authoritative verified count for this revision is **50 unique foundational class files**.

## 2. Module Coverage

The repository-wide source tree contains implementation/support areas that must be represented in the eventual complete class inventory. The current 0.3.0-dev tree includes:

- `antivirus/`
- `api/`
- `audio/`
- `codecs/`
- `connector/`
- `coorenagraph/`
- `data-structures/`
- `debugger/`
- `decompiler/`
- `drivers/`
- `examples/`
- `gui/`
- `http/`
- `http-1.0/`
- `http-2.0/`
- `http-3.0/`
- `http-4.0/`
- `http-5.0/`
- `http-6.0/`
- `http-7.0/`
- `http-8.0/`
- `http-9.0/`
- `http-servers/`
- `impl/`
- `java28/`
- `ledger/`
- `modules/`
- `munction/`
- `native/`
- `rmi/`
- `runtime/`
- `server-edition/`
- `sleela-terminal/`
- `sleela/`
- `telephony-skya/`
- `terminal_pixel/`
- `test-suites/`
- `tests/`
- `tools/`

These module roots are part of the inventory scope. A module is not excluded merely because its implementation is C, Java, generated code, a protocol implementation, a driver, a test harness, or supporting tooling.

## 3. Inventory Classification

The complete inventory is intended to distinguish:

1. **C++ classes** — concrete classes and abstract/interface-style classes.
2. **C++ structs** — public data contracts and POD-like structures.
3. **C++ nested classes/structs** — declarations contained within another type.
4. **Java classes/interfaces** — including the Java 28/native bridge layer.
5. **C opaque/object contracts** — C types that provide class-like module boundaries.
6. **Protocol/module classes** — HTTP, server, transport, RMI, and connector implementations.
7. **Driver classes/contracts** — device and platform support.
8. **Audio/media classes** — audio, codec, and native media layers.
9. **Debugger/decompiler/compiler classes** — development and language-toolchain support.
10. **Test/diagnostic classes** — test-only contracts and verification infrastructure.

## 4. Source-of-Truth Rules

The version at the top of this document comes from `VERSION.md`.

The verified foundational class list comes from the current `test-suites/cpp/test_class_contracts.cpp` contract suite. Each listed type is required to be complete and destructible by that test.

The repository source tree, rather than this document alone, remains the ultimate source for determining whether a declaration exists. This document is an inventory/index and should be regenerated or checked whenever source classes change.

## 5. Next Inventory Gate

The next revision should mechanically scan every supported source file in the 0.3.0-dev tree and record:

- declaration kind;
- class/struct/interface name;
- namespace/package;
- source file;
- implementation file, where applicable;
- module;
- language;
- public/abstract status where determinable;
- duplicate declarations versus unique types;
- test coverage linkage.

The resulting repository-wide count should replace the foundational-only count above once the AST/source scan is integrated into CI.

---

**SLeeLa — MEARVK LLC — 2026**
