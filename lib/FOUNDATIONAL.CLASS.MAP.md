# SLeeLa Foundational C++ → Language Contract Map

**Revision:** 0.1  
**Date:** 2026-09-28

The 50 files under `impl/fundamental/` are the verified C++ foundational class-file set. This map distinguishes their architectural roles and identifies the SLeeLa-facing contract that should sit above each native implementation.

## Classification

- **Language primitive:** semantic type/contract needed by the language model.
- **Runtime primitive:** execution, concurrency, data-flow, or lifecycle facility needed by the runtime.
- **Service:** OS, filesystem, networking, or application-service facility exposed through a controlled bridge.

A native class is not automatically a language primitive. The SLeeLa contract is the stable semantic surface; C/C++ remains an implementation layer.

## Verified map

| C++ foundation | Role | SLeeLa contract | Responsibility |
|---|---|---|---|
| `Buffer` | runtime | `SLBuffer` | bounded byte/value storage |
| `ByteStream` | runtime | `SLByteStream` | sequential byte transport abstraction |
| `CancellationToken` | runtime | `SLCancellationToken` | cooperative cancellation |
| `Client` | service | `SLClient` | client endpoint lifecycle |
| `Clock` | runtime | `SLClock` | monotonic/wall clock access |
| `Condition` | runtime | `SLConditionVariable` | thread synchronization condition |
| `Configuration` | service | `SLConfiguration` | configuration state |
| `Connection` | service | `SLConnection` | generic connection lifecycle |
| `Deserializer` | runtime | `SLDeserializer` | structured data decoding |
| `Directory` | service | `SLDirectory` | directory operations |
| `DnsResolver` | service | `SLDnsResolver` | name resolution |
| `EnumDescriptor` | language | `SLEnumDescriptor` | runtime enum metadata |
| `Environment` | service | `SLEnvironment` | process environment |
| `Error` | language | `SLError` | error identity and reporting |
| `Event` | runtime | `SLEvent` | event value/notification |
| `EventBus` | runtime | `SLEventBus` | event publication/subscription |
| `File` | service | `SLFile` | file resource |
| `Future` | runtime | `SLFuture` | asynchronous result |
| `Handler` | runtime | `SLHandler` | callback/event handler contract |
| `Identifier` | language | `SLIdentifier` | stable identity |
| `Input` | runtime | `SLInput` | input stream contract |
| `Listener` | runtime | `SLListener` | event/network listener contract |
| `Mutex` | runtime | `SLMutex` | mutual exclusion |
| `Name` | language | `SLName` | qualified/name value |
| `NetworkEndpoint` | service | `SLNetworkEndpoint` | network endpoint identity |
| `Option` | language | `SLOption` | optional value semantics |
| `Output` | runtime | `SLOutput` | output stream contract |
| `Parameter` | language | `SLParameter` | callable/type parameter metadata |
| `Path` | service | `SLPath` | filesystem path value |
| `Process` | service | `SLProcess` | operating-system process |
| `Resource` | runtime | `SLResource` | managed resource lifecycle |
| `Result` | language | `SLResult` | success/failure result |
| `Router` | service | `SLRouter` | route selection |
| `Scheduler` | runtime | `SLScheduler` | task scheduling |
| `Schema` | language | `SLSchema` | data/type schema |
| `SecureChannel` | service | `SLSecureChannel` | authenticated/encrypted channel |
| `Serializer` | runtime | `SLSerializer` | structured data encoding |
| `Server` | service | `SLServer` | server lifecycle |
| `Service` | service | `SLService` | service abstraction |
| `Session` | service | `SLSession` | session state/lifecycle |
| `Status` | language | `SLStatus` | status identity |
| `Task` | runtime | `SLTask` | unit of asynchronous work |
| `TcpTransport` | service | `SLTcpTransport` | TCP transport |
| `Thread` | runtime | `SLThread` | thread execution |
| `Timer` | runtime | `SLTimer` | time-based scheduling |
| `Transport` | runtime | `SLTransport` | generic transport contract |
| `TypeDescriptor` | language | `SLTypeDescriptor` | runtime type metadata |
| `UdpTransport` | service | `SLUdpTransport` | UDP transport |
| `Validator` | language | `SLValidator` | contract validation |

## Architectural conclusions

### Language-facing base contracts

The most fundamental semantic contracts are:

`SLEnumDescriptor`, `SLError`, `SLIdentifier`, `SLName`, `SLOption`, `SLParameter`, `SLResult`, `SLSchema`, `SLStatus`, `SLTypeDescriptor`, and `SLValidator`.

These should be usable without requiring an operating-system call.

### Runtime contracts

Execution and concurrency belong in the runtime layer:

`SLBuffer`, `SLByteStream`, `SLCancellationToken`, `SLClock`, `SLConditionVariable`, `SLDeserializer`, `SLEvent`, `SLEventBus`, `SLFuture`, `SLHandler`, `SLInput`, `SLListener`, `SLMutex`, `SLOutput`, `SLResource`, `SLScheduler`, `SLSerializer`, `SLTask`, `SLThread`, `SLTimer`, and `SLTransport`.

### Bridge/service contracts

OS and network facilities should remain explicit bridge points:

`SLClient`, `SLConfiguration`, `SLConnection`, `SLDirectory`, `SLDnsResolver`, `SLEnvironment`, `SLFile`, `SLNetworkEndpoint`, `SLPath`, `SLProcess`, `SLRouter`, `SLSecureChannel`, `SLServer`, `SLService`, `SLSession`, `SLTcpTransport`, and `SLUdpTransport`.

This keeps platform authority below the language layer.

## Bootstrap priority

The next implementation gate should be:

1. Language contracts.
2. Value/error/result/type semantics.
3. Memory/resource and execution primitives.
4. I/O and serialization.
5. Concurrency.
6. Filesystem/process services.
7. Network transports and secure channels.
8. Higher-level server/client services.

The 50 C++ classes therefore form a **foundation map**, not one undifferentiated "base class" layer.

**SLeeLa — MEARVK LLC — 2026**
