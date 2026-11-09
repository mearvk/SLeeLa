# SLeeLa Java Runtime Integration Status

This document separates implemented Java process hosting from Java API declarations
and from future in-process interoperability. A source envelope is not evidence
that an individual Java method can already be invoked from a SLeeLa expression.

## Capability matrix

| Capability | Current status | Boundary |
|---|---|---|
| Detect Java-dependent source | Implemented in runtime/java_runtime_probe.* | Source/path signal detection and local executable discovery |
| Launch Java source files | Implemented through the external Java host | Child process; host JVM performs Java compilation/execution |
| Launch runnable JARs | Implemented through the CLI external-host path | JAR manifest must declare Main-Class |
| Launch compiled class folders | Implemented through the CLI external-host path | Explicit class path and fully qualified main class |
| Forward arguments and process result | Implemented by the native process-launcher path | OS process boundary; actual exit status/output belong to provider |
| Describe Java API types | Implemented by /lib/java source envelopes | SLeeLa declarations and package vocabulary |
| Java-side reflective construction and invocation | Implemented in connector/java through SleelaJdkBridge | Explicit class/package allow-list; public members; non-initializing type lookup |
| Typed Java request/response endpoint | Implemented as SLJIP v1 in the Java connector | Independent binary helper process; configured allow-list; bounded frames |
| Native frame codec and callback exchange | Implemented in runtime/java_interop_protocol.c | Encodes, validates, and exchanges frames over caller-provided exact-I/O callbacks |
| Resolve a Java type and invoke a method from a SLeeLa expression | Not yet implemented | Requires compiler/VM lowering and a native provider connected to the Java endpoint |
| Java-side object handles and structured exceptions | Implemented in SLJIP v1 endpoint | Opaque per-session handles; release/close; exception class and message response |
| Marshal SLeeLa values through the native provider | Not yet implemented end-to-end | Requires VM value conversion and process/pipe integration |
| Embedded JVM / JNI Invocation API | Not implemented by the default build | Separate opt-in build and platform qualification required |

The external process path is not JNI and does not load Java bytecode into the SLeeLa VM.
The Java connector also has a reflective helper (SleelaJdkBridge) backed by
SleelaClassLoader: it requires explicit class/package allow-list entries, resolves
types without initialization by default, and checks static versus instance calls.
The SLJIP endpoint is a separately launchable helper and the native codec is linked
into the native tools. The callback exchange API still needs a concrete process/pipe
provider, and neither layer is yet lowered from SLeeLa expressions into VM operations.

## Existing native host

The external Java host lives in:

- runtime/java_runtime_probe.h and .c
- runtime/java_runtime_bridge.h and .c
- connector/java/com/mearvk/sleela/connector/SleelaClassLoader.java
- connector/java/com/mearvk/sleela/connector/SleelaJdkBridge.java
- the sleela run command's Java dispatch path
- test-suites/java-runtime-probe.sh
- test-suites/java-runtime-bridge.sh

The probe searches SLEELA_JAVA, JAVA_HOME, and then PATH. It detects Java
requirements and prepares a child-process handoff. It does not silently install
a JDK. The platform launcher must own actual process creation, argument quoting,
stdin/stdout/stderr forwarding, timeout policy, and exit-status collection.

The existing CLI forms are documented in runtime/JAVA-HOST-CAPABILITIES.md:

    sleela run app.jar [application arguments...]
    sleela run --class-path build/classes com.example.Main [application arguments...]
    sleela run --class com.example.Main [application arguments...]
    sleela run Program.java [application arguments...]

## SLJIP v1 implementation

The protocol is specified in lib/java/protocol/SLJIP-v1.md and
lib/java/protocol/SLJIP-v1.json. Compiler integration requirements are tracked in
impl/JAVA-INTEROP-LOWERING.md. Its Java endpoint is
connector/java/com/mearvk/sleela/connector/SleelaJdkProtocolServer.java.

Implemented operations:

- PING and non-initializing RESOLVE.
- CONSTRUCT, INVOKE_INSTANCE, and INVOKE_STATIC using public members.
- GET_FIELD, GET_STATIC_FIELD, SET_FIELD, and SET_STATIC_FIELD for public fields.
- RELEASE and CLOSE for opaque, monotonically allocated session handles.
- Typed null, boolean, I32, I64, F64, UTF-8, and object-handle values.
- Primitive argument range checks and explicit numeric conversion rules.
- Structured exceptions with exception class and bounded message; no Java
  exception unwinds through the native C ABI.
- Class directories and JAR roots, configured before the first class resolution;
  roots freeze at first use to preserve class identity for live handles.
- A 1 MiB maximum frame and 256 KiB maximum string.
- Out-of-band allow-list configuration through
  SLEELA_JAVA_ALLOW_CLASSES and SLEELA_JAVA_ALLOW_PACKAGES.
- Optional class directories/JAR roots through SLEELA_JAVA_CLASS_PATHS, separated
  with the platform's path separator and frozen at first type resolution.

The native codec in runtime/java_interop_protocol.c validates and encodes frames.
Its callback-driven exchange function accepts exact-read/exact-write functions,
so a future native process provider can connect it to OS pipes without moving
process management into the protocol parser. The codec is linked into sleela
and sleelvac.

Not yet implemented: Java arrays, arbitrary collection conversion, SLeeLa VM-value
marshalling, managed process/pipe ownership, and compiler/VM lowering.
These are deliberately excluded from v1 rather than represented by incomplete
opcodes.

## Compiler and VM integration boundary

The compiler must lower explicit, statically checked Java interop operations to
a typed host-call node, not parse descriptive java.invoke: strings. The node must
carry operation kind, Java binary owner, member, parameter descriptors, source
span, and arguments. Semantic analysis must reject unresolved types, unsupported
value conversions, and calls outside the configured policy. Lowering must produce
a VM operation that calls the native provider; the provider must serialize only
the SLJIP v1 types and must never serialize arbitrary SLeeLa object memory.

The VM call contract must define:

1. Request IDs and a single serialized request stream per host session.
2. Exact primitive conversion, UTF-8/null handling, and object-handle ownership.
3. Deterministic release on explicit close and VM/session teardown.
4. Structured Java exceptions mapped to a language-level error result, never
   default values or C++ exceptions crossing a C ABI.
5. Cancellation/timeout behavior and child-process termination policy.
6. A policy gate that cannot be widened by untrusted source code.

No new surface syntax or bytecode opcode is claimed in this change. The current
parser/compiler architecture must be extended and tested before any source-level
interop example is advertised as runnable.

## Focused test commands

From the repository root:

    bash test-suites/java-interop-protocol-codec.sh
    bash test-suites/java-interop-protocol-endpoint.sh

From connector/:

    make protocol-test protocol-codec-test

The endpoint test exercises allow-list denial, resolve, construction, static
and instance invocation, public instance/static field reads and writes, handle
release, stale-handle rejection, structured Java exceptions, and clean close. The native test exercises frame encoding,
decoding, bounds, malformed headers, and callback exchange. A Python orchestration
runner at test-suites/java-interop-protocol.py supports Windows, Linux, and macOS
when a JDK, Python, and C compiler are installed. These tests have
not been run in this work session; they are committed for local execution.

## Cross-platform qualification

The protocol itself is endian-stable and has no POSIX-specific implementation.
The focused shell runners require a POSIX-compatible shell; on Windows they can
be run from Git Bash or an equivalent environment. The native process provider
and pipe implementation still require separate Linux, Windows, and macOS tests.
The existing external Java process launcher remains the production path for
running .java files, runnable JARs, and class folders until that provider exists.

## Qualification language

Documentation and status output must use precise terms:

- External Java host available means a Java executable was found.
- JAR/class launch supported means the external process launch path is wired.
- Java API envelope present means a SLeeLa source declaration exists.
- Method invocation supported requires an end-to-end typed call test.
- Embedded JVM supported requires a separately built and qualified JNI profile.

A successful executable probe is not a Java API conformance result.
