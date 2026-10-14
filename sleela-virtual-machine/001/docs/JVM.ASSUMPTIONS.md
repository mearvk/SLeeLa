# JVM.ASSUMPTIONS.md

## Purpose
This document records the assumptions made across SLeeLa source, the compiler, SLeeLa Output Symbols/Core representation, SLVM, the JVM bridge, and OS-specific runtime adapters.

These are assumptions to be verified, not permissions to invent behavior. When an assumption is false, the system must report incompatibility rather than silently changing program semantics.

Copyright (c) Max Rupplin - MEARVK LLC - 2026

## 1. Authoritative Language
- .sleela source is authoritative.
- The SLeeLa Lexer, Parser, compiler, Output Symbol generation, Core representation, and SLVM form one execution architecture.
- SLVM is not a reduced or alternate SLeeLa interpreter.
- Valid SLeeLa programs are intended to execute through SLVM.
- The SLeeLa API/function vocabulary is intended to be Turing complete.
- A missing SLVM implementation for a valid language operation is an implementation gap, not a language limitation.
- /lib is part of the authoritative SLeeLa source vocabulary.

## 2. Output Symbol Contract
Every compiler-emitted Output Symbol must have defined:
- identity and version;
- operand format and types;
- stack/local/global effects;
- object and memory effects;
- control-flow effects;
- capability requirements;
- error behavior;
- OS/runtime mapping where applicable.

Unknown, malformed, or incompatible symbols must be rejected explicitly. Compiler and SLVM therefore share a versioned symbol contract.

## 3. SLVM Execution
SLVM is the common execution substrate for values, objects, classes, functions, control flow, recursion, memory, errors, collections, streams, concurrency, synchronization, IPC, networking, files, time, devices, GUI proxies, and JVM broker objects.

Native C/C++ implementations are adapters behind this contract, not alternate language runtimes.

## 4. /lib
/lib is assumed to contain executable SLeeLa source definitions and vocabulary. Library classes use the same compiler-to-SLVM path as application source.

The /lib conformance suite is verification of the language/runtime contract; it does not define a smaller subset of SLeeLa.

## 5. C and C++
C provides stable native ABI boundaries. C++ may provide higher-level runtime implementations, object orchestration, platform adapters, JVM integration, and richer wrappers.

Neither may bypass SLVM security, memory, capability, observer, or lifetime rules.

## 6. OS Abstraction
SLeeLa semantics are platform-neutral. SLVM requests operations through capabilities/runtime interfaces, while adapters implement them for Linux/POSIX, Windows 10+, macOS, or another explicitly supported platform.

Platform differences must be represented as capability differences, documented behavioral differences, or explicit availability errors. They must not silently change SLeeLa semantics.

## 7. Capability Authority
Having a SLeeLa function or class does not imply OS authority. Filesystem, process, network, DNS/resolution, device, dynamic-library, IPC, terminal, cryptographic, GUI/media, and system operations require explicit capabilities.

Capability checks occur before native OS operations.

## 8. Memory
SLVM-managed objects participate in the VM memory manager and garbage collector. The current architectural default is a 512 MiB managed-memory ceiling.

That ceiling does not automatically mean total process RSS, native stacks, executable memory, JVM heap, kernel allocations, or external device memory.

Memory security operates before allocation and cooperates with garbage collection.

## 9. JVM
The JVM is an external runtime host, not the authoritative SLeeLa language runtime.

The JVM may provide Java objects, JavaFX, Java libraries, JVM threads, and JVM-managed resources. SLVM remains responsible for SLeeLa semantics, authorization, VM object identity, capabilities, SLVM memory/security policy, and observer events.

## 10. JavaFX
When JavaFX is used remotely, JavaFX thread confinement belongs to the JVM side. GUI objects are represented in SLVM by opaque broker handles. SLVM does not manipulate Java object memory directly.

GUI events return through the broker and remain subject to SLVM authorization and security policy.

## 11. JVM Object Broker
The broker is the controlled boundary between native SLVM and a remote JVM host.

Initial message family:
- HELLO
- OBJECT_DECLARE
- OBJECT_CALL
- OBJECT_RETURN
- GUI_CREATE
- GUI_EVENT
- GUI_CLOSE
- OBJECT_RELEASE
- ERROR
- CLOSE

The broker assumes authenticated peers, encrypted transport, version negotiation, bounded frames, explicit object IDs, explicit declaration/call/return/release, and structured errors.

Transport is neutral: secure TCP/TLS, Unix sockets, named pipes, or another approved secure transport may implement it.

## 12. Credentials and Passwords
Passwords, private keys, and PSKs are references to protected credentials, not ordinary application payloads.

Preferred sources include OS credential stores, JVM/platform secret providers, protected configuration stores, and hardware-backed credential facilities where available.

Credentials must not be transmitted as plaintext protocol fields, serialized into broker objects, written to logs, or copied into observer records.

Cryptographic operations, certificate validation, key rotation, and secure storage are delegated to an approved TLS/cryptographic provider.

## 13. TLS
Remote JVM communication is expected to use modern authenticated encryption, preferably TLS 1.3 with mutual authentication where appropriate.

The system assumes certificate validation, peer identity validation, expiration handling, rotation, and rejection of invalid peers. Production configuration must not silently downgrade to plaintext.

## 14. Serialization
Only explicitly defined value kinds cross the broker: scalar, string, bytes, list, map, class, GUI, and stream.

Serialization preserves type identity, required object identity, numeric semantics, encoding, length, null/absence semantics, and errors.

Native pointers are never serialized.

## 15. Functions and Returns
Function boundaries are observable. The Official Observer may receive function entry, each parameter, native capability activity where applicable, object operations, and function return/status.

Parameters containing secrets carry secret metadata and are redacted before observer callbacks.

## 16. Official Observer
The Official Observer is a separate security principal.

It may inspect function calls, parameters, returns, object lifecycle, memory, I/O, security state, capability decisions, broker traffic, and certificate/attestation records.

It is read-only by default and cannot silently grant capabilities. Active control requires an explicit capability and audit trail.

## 17. Certificates and Attestation
Certificate records may bind runtime identity, SLeeLa source/artifact identity, symbol/Core version, class/object identity, observer policy version, security and memory state, capability decisions, broker peer identity, event sequence/digest, timestamp, and nonce.

Certificates attest to evidence and policy state rather than copying secrets.

## 18. Security Layers
Conceptually: SLeeLa execution -> runtime security -> memory security/GC -> I/O heuristic -> capability broker -> platform adapter -> OS.

The JVM broker and Official Observer are additional controlled boundaries. No heuristic replaces capability authorization.

## 19. I/O and Resolver
I/O is observable and policy-controlled using request frequency, volume, file activity, network activity, device activity, dynamic loading, and burst behavior.

Resolver operations remain behind the capability/runtime boundary. A newly resolved IP or route does not automatically acquire authorization merely because DNS or dynamic path resolution produced it.

## 20. Concurrency and Lifetime
SLeeLa concurrency executes through VM-controlled scheduling/resource primitives. Foreign JVM or OS threads require explicit ownership, lifecycle tracking, synchronization, and shutdown behavior.

Files, sockets, pipes, devices, GUI objects, JVM proxies, native libraries, processes, threads, and memory regions have explicit lifecycles and must be released on normal and failure paths.

## 21. Errors
Boundary errors remain structured. The system should preserve SLeeLa error identity, SLVM status, capability denial, OS error code, JVM exception identity, broker error, and security/certificate status where available.

An OS-specific failure must not silently become an unrelated successful result.

## 22. Versioning
SLeeLa language, Output Symbols, Core representation, SLVM, /lib, JVM broker, observer protocol, capability API, and native platform adapters are independently versionable contracts.

Incompatible symbol/runtime versions must be negotiated or rejected rather than guessed.

## 23. Determinism
Pure SLeeLa computation retains language-defined semantics. Operations involving clocks, randomness, networks, filesystems, devices, scheduling, GUI events, or remote JVM objects are environment-dependent and must not be falsely represented as deterministic.

## 24. What We Must Not Assume
- Java is required for ordinary SLeeLa execution.
- JavaFX is required for ordinary SLeeLa execution.
- Linux behavior automatically applies to Windows or macOS.
- An OS pointer can become a SLeeLa object.
- A Java reference can become an SLVM pointer.
- A password belongs in a broker frame.
- Plaintext is an acceptable silent fallback for secure broker communication.
- A library file is executable merely because it exists.
- An unknown Output Symbol can be guessed.
- An observer automatically receives execution authority.
- A function name implies a capability.
- An I/O heuristic is equivalent to authorization.
- Managed-memory accounting equals total system memory.
- Successful compilation alone proves successful execution.
- A successful local OS operation guarantees portability.

## 25. Verification Contract
Automated verification should establish:
- every Output Symbol has SLVM semantics;
- every /lib .sleela source can compile and execute;
- every supported OS adapter implements its capability contract;
- broker version negotiation works;
- invalid broker peers are rejected;
- credentials never enter protocol payloads;
- secret observer values are redacted;
- function parameters and returns reach observer hooks;
- memory/security limits remain enforced;
- JVM GUI objects remain opaque broker objects;
- broker failures propagate as structured errors;
- resources are released on success and failure;
- incompatible symbol/runtime versions are rejected.

## 26. Final Architectural Assumption
SLeeLa defines the program. The compiler defines its Output Symbols. SLVM defines their execution semantics. Capabilities define permitted authority. Platform adapters define how permitted operations are performed on the current OS. The JVM is an optional external host for explicitly brokered Java/JVM functionality, including GUI presentation. The Official Observer provides independently controlled analysis and certification visibility.

No lower layer is permitted to silently redefine the semantics established by an authoritative higher layer.

Copyright (c) Max Rupplin - MEARVK LLC - 2026