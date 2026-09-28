# SLeeLa Class Inventory

**SLeeLa Version:** 0.3.0-dev  
**Inventory Revision:** 1.1  
**Inventory Date:** 2026-09-28  
**Unique Foundational C++ Class Files (contract-verified): 50**

> This document is the authoritative starting inventory for the SLeeLa 0.3.0-dev development line. The **50 unique foundational C++ class files** are directly included and contract-checked by `test-suites/cpp/test_class_contracts.cpp`. This is a verified foundational class-file count, not a claim that no other repository source file contains a class, struct, interface, or class-like declaration.

## 1. Verified Foundational Class Files

The current contract suite verifies these 50 foundational class files:

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

**Foundational count: 50.**

## 2. Data-Structures Module Inventory

The `data-structures/` module is explicitly included in the class/type inventory. These support-layer declarations are tracked separately from the 50 foundational C++ class-file count.

### 2.1 C++ data-structure types

| Type | Kind | Source |
|---|---|---|
| `Value` | type alias / tagged value | `data-structures/data_structures.hpp` |
| `Vector<T>` | template type alias | `data-structures/data_structures.hpp` |
| `Stack<T>` | template type alias | `data-structures/data_structures.hpp` |
| `Queue<T>` | template type alias | `data-structures/data_structures.hpp` |
| `Map` | type alias / string map | `data-structures/data_structures.hpp` |
| `ObjectRecord` | C++ struct | `data-structures/data_structures.hpp` |
| `ObjectRecord::set` | member function | `data-structures/data_structures.cpp` |
| `ObjectRecord::get` | member function | `data-structures/data_structures.cpp` |

The C++ layer uses `std::variant`, `std::vector`, `std::deque`, and `std::unordered_map` for concrete storage.

### 2.2 C data-structure contracts

| Type | Kind | Source |
|---|---|---|
| `SLDSKind` | enum | `data-structures/data_structures.h` |
| `SLDSValue` | struct | `data-structures/data_structures.h` |
| `SLDSVector` | struct | `data-structures/data_structures.h` |
| `SLDSStack` | struct | `data-structures/data_structures.h` |
| `SLDSQueue` | struct | `data-structures/data_structures.h` |
| `SLDSMap` | struct | `data-structures/data_structures.h` |
| `SLDSObject` | struct | `data-structures/data_structures.h` |

The C implementation is provided by `data-structures/data_structures.c`, including vector, stack, queue, map, and object lifecycle/lookup operations.

### 2.3 SLeeLa data-structure model

| Type | Kind | Source |
|---|---|---|
| `DSValue` | SLeeLa struct | `data-structures/data_structures.sleela` |
| `DSList` | SLeeLa struct | `data-structures/data_structures.sleela` |
| `DSStack` | SLeeLa struct | `data-structures/data_structures.sleela` |
| `DSQueue` | SLeeLa struct | `data-structures/data_structures.sleela` |
| `DSMap` | SLeeLa struct | `data-structures/data_structures.sleela` |
| `DSObject` | SLeeLa struct | `data-structures/data_structures.sleela` |
| `DataStructures` | SLeeLa class | `data-structures/data_structures.sleela` |

The SLeeLa layer is the semantic counterpart of the native C/C++ data-structure layer.

### 2.4 Complete module source set

- `data-structures/data_structures.h`
- `data-structures/data_structures.c`
- `data-structures/data_structures.hpp`
- `data-structures/data_structures.cpp`
- `data-structures/data_structures.sleela`
- `data-structures/DATA_STRUCTURES.md`
- `data-structures/README.md`

These files are part of the repository-wide inventory scope and do not replace the foundational runtime structures under `impl/`.

## 3. Module Coverage

The current 0.3.0-dev tree includes these inventory roots:

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
- `http-1.0/` through `http-9.0/`
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

A module is not excluded merely because its implementation is C, C++, Java, SLeeLa, generated code, a protocol implementation, driver, test harness, or supporting tooling.

## 4. Inventory Classification

The complete inventory distinguishes:

1. **C++ classes** — concrete and abstract/interface-style classes.
2. **C++ structs** — public data contracts and POD-like structures.
3. **C++ type aliases/templates** — public data-structure contracts.
4. **C++ nested classes/structs.**
5. **Java classes/interfaces** — including the Java 28/native bridge layer.
6. **C opaque/object contracts** — C types providing class-like module boundaries.
7. **SLeeLa structs/classes** — language-level declarations and semantic counterparts.
8. **Protocol/module classes** — HTTP, server, transport, RMI, and connector implementations.
9. **Driver classes/contracts.**
10. **Audio/media classes.**
11. **Debugger/decompiler/compiler classes.**
12. **Test/diagnostic classes.**

## 5. Source-of-Truth Rules

The version at the top of this document comes from `VERSION.md`.

The verified foundational class list comes from `test-suites/cpp/test_class_contracts.cpp`.

The `data-structures/` entries are verified against the current repository source files and are tracked as module-level data contracts. They are not added to the 50 foundational C++ class-file count because the C++ module currently exposes `ObjectRecord` as a struct and several type aliases rather than additional foundational class headers.

The repository source tree remains the ultimate source for determining whether a declaration exists. This document is an inventory/index and should be regenerated or checked whenever source declarations change.

## 6. Next Inventory Gate

The next revision should mechanically scan every supported source file and record:

- declaration kind;
- class/struct/interface/type-alias name;
- namespace/package;
- source file;
- implementation file, where applicable;
- module;
- language;
- public/abstract status where determinable;
- duplicate declarations versus unique types;
- test coverage linkage.

The repository-wide declaration count should replace the foundational-only count once the AST/source scan is integrated into CI.

---

**SLeeLa — MEARVK LLC — 2026**
