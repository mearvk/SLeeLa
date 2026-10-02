# SLeeLa Class Inventory

**SLeeLa Version:** 0.3.22-dev  
**Inventory Revision:** 2.0  
**Inventory Date:** 2026-10-02  
**Known Source Files Explicitly Indexed: 138**
**Repository-wide SLeeLa source files (verified): 10,235 on `main`**
**Standard-library SLeeLa source units in `/lib`: 10,033**
**Standard-library target: 2,048 objects**  
**Unique Foundational C++ Class Files (contract-verified): 50**

> This document is the authoritative starting inventory for the SLeeLa 0.3.0-dev development line. The **50 unique foundational C++ class files** are directly included and contract-checked by `test-suites/cpp/test_class_contracts.cpp`. This is a verified foundational class-file count, not a claim that no other repository source file contains a class, struct, interface, or class-like declaration.

## 1. Verified Foundational Class Files

The current contract suite verifies these 50 foundational class files. The repository-wide SLeeLa source inventory now records 10,235 `.sleela` files, including 10,033 under `/lib`; the library inventory is maintained separately in `lib/LIBRARY.SYMBOLS.md`.

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

## 3. Audio SLeeLa Source Inventory

The `audio/sleela/` directory is confirmed to contain **9 SLeeLa source files**. Each file declares an SLeeLa class and is therefore included in the known source/class inventory.

| # | SLeeLa Class | Source |
|---:|---|---|
| 1 | `Audio` | `audio/sleela/Audio.sleela` |
| 2 | `AudioConfiguration` | `audio/sleela/AudioConfiguration.sleela` |
| 3 | `AudioControls` | `audio/sleela/AudioControls.sleela` |
| 4 | `AudioDevice` | `audio/sleela/AudioDevice.sleela` |
| 5 | `AudioInput` | `audio/sleela/AudioInput.sleela` |
| 6 | `AudioMixer` | `audio/sleela/AudioMixer.sleela` |
| 7 | `AudioNative` | `audio/sleela/AudioNative.sleela` |
| 8 | `AudioStream` | `audio/sleela/AudioStream.sleela` |
| 9 | `AudioSystem` | `audio/sleela/AudioSystem.sleela` |

**Audio SLeeLa source count: 9.**

The audio files are SLeeLa language source, not documentation: they begin with `#sleela 1.3` and declare classes with SLeeLa methods. They are tracked separately from the 50 foundational C++ class-file count.

**Known source files explicitly indexed in this inventory: 14** — 5 data-structures implementation/source files plus 9 audio SLeeLa source files. This is an inventory count, not a claim that the repository contains only 14 source files.

## 4. Unified `/lib` SLeeLa Front End

The new `/lib` tree establishes an object-per-source-file standard-library front end with **69 SLeeLa source files / object types** across core values, collections, text, I/O, VM, OS, networking, and security.

The library targets **2,048 object types**. Only the initial 69 are implemented in this revision. VM/OS objects are semantic front-end contracts and cross into native C/C++ only through explicit binding layers.

### Repository-wide SLeeLa source count

The `master` tree was mechanically enumerated before this addition at **160 `.sleela` source files**. The 69 new `/lib` files bring `master` to **229 `.sleela` source files**. The corresponding `main` tree contains **238 `.sleela` source files**, reflecting nine additional branch-specific SLeeLa source files. These are source-file counts, not declaration counts.

## 5. CommonRails Printing SLeeLa Classes

The CommonRails printing system now has **11 native SLeeLa classes**. They are present in both the library front end (`lib/common-rails/`) and the CommonRails source package (`common-rails/`). The duplicate paths are intentional: `/lib` exposes the standard-library front end while `/common-rails` retains the module's source-side organization.

| Class # | SLeeLa Class | `/lib` Source | `/common-rails` Source |
|---:|---|---|---|
| 51 | `PrintLayout` | `lib/common-rails/PrintLayout.sleela` | `common-rails/PrintLayout.sleela` |
| 52 | `PrintField` | `lib/common-rails/PrintField.sleela` | `common-rails/PrintField.sleela` |
| 53 | `PrintLine` | `lib/common-rails/PrintLine.sleela` | `common-rails/PrintLine.sleela` |
| 54 | `PrintFormatter` | `lib/common-rails/PrintFormatter.sleela` | `common-rails/PrintFormatter.sleela` |
| 55 | `PrintState` | `lib/common-rails/PrintState.sleela` | `common-rails/PrintState.sleela` |
| 56 | `PrintProgress` | `lib/common-rails/PrintProgress.sleela` | `common-rails/PrintProgress.sleela` |
| 57 | `PrintGlyphs` | `lib/common-rails/PrintGlyphs.sleela` | `common-rails/PrintGlyphs.sleela` |
| 58 | `PrintComponent` | `lib/common-rails/PrintComponent.sleela` | `common-rails/PrintComponent.sleela` |
| 59 | `PrintRenderer` | `lib/common-rails/PrintRenderer.sleela` | `common-rails/PrintRenderer.sleela` |
| 60 | `PrintWriter` | `lib/common-rails/PrintWriter.sleela` | `common-rails/PrintWriter.sleela` |
| 61 | `PrintContractTest` | `lib/common-rails/PrintContractTest.sleela` | `common-rails/PrintContractTest.sleela` |

**CommonRails native printing class count: 11.**

These classes cover layout, fields, lines, formatting, state, progress, glyphs, components, rendering, output, and the native SLeeLa contract test. They correspond to the Heritage C/C++/Java printing responsibilities and are part of the SLeeLa Compiler/Loader source-discovery surface.
## 6. SLeeLa Data-Structures Source Classes and Types

The data-structures module now has an explicit per-source-file SLeeLa surface in both `/data-structures` and `/lib/data-structures`.

### SLeeLa structure types

| Type | Kind | Source |
|---|---|---|
| `DSValue` | SLeeLa struct | `data-structures/DSValue.sleela` |
| `DSList` | SLeeLa struct | `data-structures/DSList.sleela` |
| `DSStack` | SLeeLa struct | `data-structures/DSStack.sleela` |
| `DSQueue` | SLeeLa struct | `data-structures/DSQueue.sleela` |
| `DSMap` | SLeeLa struct | `data-structures/DSMap.sleela` |
| `DSObject` | SLeeLa struct | `data-structures/DSObject.sleela` |

### SLeeLa classes

| Class # | SLeeLa Class | `/data-structures` Source | `/lib/data-structures` Source |
|---:|---|---|---|
| 62 | `DSValueOperations` | `data-structures/DSValue.sleela` | `lib/data-structures/DSValue.sleela` |
| 63 | `DSListOperations` | `data-structures/DSList.sleela` | `lib/data-structures/DSList.sleela` |
| 64 | `DSStackOperations` | `data-structures/DSStack.sleela` | `lib/data-structures/DSStack.sleela` |
| 65 | `DSQueueOperations` | `data-structures/DSQueue.sleela` | `lib/data-structures/DSQueue.sleela` |
| 66 | `DSMapOperations` | `data-structures/DSMap.sleela` | `lib/data-structures/DSMap.sleela` |
| 67 | `DSObjectOperations` | `data-structures/DSObject.sleela` | `lib/data-structures/DSObject.sleela` |
| 68 | `DataStructures` | `data-structures/DataStructures.sleela` | `lib/data-structures/DataStructures.sleela` |
| 69 | `DataStructuresTest` | `data-structures/DataStructuresTest.sleela` | `lib/data-structures/DataStructuresTest.sleela` |

The six SLeeLa structure types are tracked separately from class numbering, while the eight operation/facade/test classes are assigned class numbers 62–69. These sources are semantic counterparts to the native C/C++ data-structure contracts and are part of the Compiler/Loader source-discovery surface.

## 7. COORENAGRAPH Moral Vocabulary SLeeLa Classes

The COORENAGRAPH moral vocabulary is now represented as individual SLeeLa class source files in both `coorenagraph/moral/` and `lib/coorenagraph/moral/`. The `/lib` copies are the standard-library front-end definitions; the module copies retain the source-side organization.

| Class # | SLeeLa Class | Part of Speech | `/lib` Source |
|---:|---|---|---|
| 70 | `Morals` | noun | `lib/coorenagraph/moral/Morals.sleela` |
| 71 | `Deontology` | noun | `lib/coorenagraph/moral/Deontology.sleela` |
| 72 | `Utilitarianism` | noun | `lib/coorenagraph/moral/Utilitarianism.sleela` |
| 73 | `VirtueEthics` | noun | `lib/coorenagraph/moral/VirtueEthics.sleela` |
| 74 | `MoralDeliberation` | noun | `lib/coorenagraph/moral/MoralDeliberation.sleela` |
| 75 | `Probity` | noun | `lib/coorenagraph/moral/Probity.sleela` |
| 76 | `Beneficence` | noun | `lib/coorenagraph/moral/Beneficence.sleela` |
| 77 | `Principled` | adjective | `lib/coorenagraph/moral/Principled.sleela` |
| 78 | `Upright` | adjective | `lib/coorenagraph/moral/Upright.sleela` |
| 79 | `Magnanimous` | adjective | `lib/coorenagraph/moral/Magnanimous.sleela` |
| 80 | `Unscrupulous` | adjective | `lib/coorenagraph/moral/Unscrupulous.sleela` |
| 81 | `Pernicious` | adjective | `lib/coorenagraph/moral/Pernicious.sleela` |
| 82 | `Amoral` | adjective | `lib/coorenagraph/moral/Amoral.sleela` |
| 83 | `Reprobate` | noun | `lib/coorenagraph/moral/Reprobate.sleela` |
| 84 | `Moral` | adjective | `lib/coorenagraph/moral/Moral.sleela` |
| 85 | `Morale` | noun | `lib/coorenagraph/moral/Morale.sleela` |
| 86 | `Ethics` | noun | `lib/coorenagraph/moral/Ethics.sleela` |

**COORENAGRAPH individual moral class count: 17.**

Each class contains its term, grammatical category, and source-level definition. The adjective/noun distinction is retained in the class structure through `partOfSpeech()`.


## 8. Module Coverage

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

## 9. Inventory Classification

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

## 10. Source-of-Truth Rules

The version at the top of this document comes from `VERSION.md`.

The verified foundational class list comes from `test-suites/cpp/test_class_contracts.cpp`.

The `data-structures/` entries are verified against the current repository source files and are tracked as module-level data contracts. They are not added to the 50 foundational C++ class-file count because the C++ module currently exposes `ObjectRecord` as a struct and several type aliases rather than additional foundational class headers.

The repository source tree remains the ultimate source for determining whether a declaration exists. This document is an inventory/index and should be regenerated or checked whenever source declarations change.

## 11. Next Inventory Gate

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


## Standard Library Expansion 0.2

The `/lib` front-end now contains **264 SLeeLa object source files**. The expansion introduced 192 requested object definitions, with 16 paths overlapping existing library source files; therefore the net repository addition is 176 unique `.sleela` files. covers runtime, reflection, memory, process/threading, filesystem, cryptography, database, HTTP, compiler, debugger, and UI. The long-term target remains **2,048 objects**.


## 7A. Java / JDK 28 SLeeLa Source Class Set

The `/lib/java/` tree is now an explicit part of the SLeeLa class-name inventory. Java classes are represented by **native SLeeLa source files using the Java class names and package paths**.

**Current `/lib/java` SLeeLa source count: 44 `.sleela` files.**

The 44-file set consists of 9 Java/SLeeLa framework and conformance source classes plus 35 package-mapped Java class envelopes.

### Java framework/conformance source classes

| # | SLeeLa Class | Source |
|---:|---|---|
| 87 | `SLPackage` | `lib/java/SLPackage.sleela` |
| 88 | `JavaClass` | `lib/java/JavaClass.sleela` |
| 89 | `JavaObject` | `lib/java/JavaObject.sleela` |
| 90 | `JavaMethod` | `lib/java/JavaMethod.sleela` |
| 91 | `JavaBridge` | `lib/java/JavaBridge.sleela` |
| 92 | `JavaInvocation` | `lib/java/JavaInvocation.sleela` |
| 93 | `JavaType` | `lib/java/JavaType.sleela` |
| 94 | `JavaConform` | `lib/java/JavaConform.sleela` |
| 95 | `JDK28SourceSet` | `lib/java/JDK28SourceSet.sleela` |

### Complete Java 28 package-mapped source set

The complete JDK 28 source-envelope set now contains **4,236 package-mapped SLeeLa source files** under `/lib/java/java`, `/lib/java/javax`, and `/lib/java/jdk`. These are derived from the OpenJDK `jdk-28+17` source tree and retain the Java source-file class name and package path.

| Package | SLeeLa source class set |
|---|---|
| `java.io` | `File`, `InputStream`, `OutputStream`, `Reader`, `Writer` |
| `java.lang` | `Boolean`, `Class`, `Double`, `Exception`, `Integer`, `Long`, `Math`, `Number`, `Object`, `Runnable`, `RuntimeException`, `String`, `System`, `Thread`, `Throwable` |
| `java.math` | `BigInteger` |
| `java.net` | `InetAddress`, `ServerSocket`, `Socket`, `URI` |
| `java.nio` | `ByteBuffer` |
| `java.nio.file` | `Files`, `Path` |
| `java.time` | `Instant`, `LocalDateTime` |
| `java.util` | `ArrayList`, `HashMap`, `HashSet`, `LinkedList`, `Optional` |

**Java package-mapped source-envelope count: 4,236.**

**Java framework/conformance source count: 9.**

**Total `/lib/java` SLeeLa source count: 4,245.**

### Naming rule

A Java binary type name maps directly to the SLeeLa source path:

`java.package.Type` → `lib/java/java/package/Type.sleela`

Examples:

- `java.lang.String` → `lib/java/java/lang/String.sleela`
- `java.util.ArrayList` → `lib/java/java/util/ArrayList.sleela`
- `java.io.File` → `lib/java/java/io/File.sleela`
- `java.time.Instant` → `lib/java/java/time/Instant.sleela`

The **`.sleela` file is the source**. The Java 28 source set is represented as SLeeLa source envelopes; the JDK supplies runtime behavior through the conformance boundary. The Java runtime is the behavior provider through the Java conformance boundary.

### Functional conformance boundary

`JavaConform.sleela` and the existing Java bridge establish the behavioral boundary. The compiler/VM still needs direct Java invocation lowering and value marshalling for ordinary SLeeLa expressions to invoke these envelopes without a separate Java-side driver.

**Inventory rule:** future Java-supported classes should be added under `/lib/java/java/...`, `/lib/java/javax/...`, or `/lib/java/jdk/...` using their exact Java source class name and `.sleela` extension. The inventory count must be updated with the source set.
