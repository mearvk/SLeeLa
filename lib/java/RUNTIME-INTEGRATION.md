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
| Resolve a Java type and invoke a method from a SLeeLa expression | Not yet established by this module | Requires a typed invocation protocol and compiler/VM integration |
| Marshal Java objects and exceptions across the language boundary | Not yet established by this module | Requires ownership, lifetime, type and exception contracts |
| Embedded JVM / JNI Invocation API | Not implemented by the default build | Separate opt-in build and platform qualification required |

The external process path is not JNI and does not load Java bytecode into the SLeeLa VM.
The Java connector also has a reflective helper (SleelaJdkBridge) backed by
SleelaClassLoader: it requires explicit class/package allow-list entries, resolves
types without initialization by default, and checks static versus instance calls.
That Java-side helper is not yet lowered from java.invoke: source strings into
SLeeLa VM operations.

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

## Safe next step: typed invocation protocol

The next real interoperability milestone is not to add more string tokens. It is
to define a versioned request/response contract shared by the SLeeLa compiler,
loader, native C/C++ provider, and Java helper process.

The protocol should represent:

1. A request identifier and protocol version.
2. An operation kind: resolve type, construct, invoke instance, invoke static,
   read field, write field, or release handle.
3. A validated Java binary class name and member name.
4. Parameter type descriptors and tagged argument values.
5. An opaque object handle scoped to one Java host session.
6. A tagged result value or structured Java exception.
7. A maximum frame size and explicit encoding rules.

Do not serialize arbitrary SLeeLa object memory as Java values. Do not accept a
shell command as a protocol request. Java class resolution must use an explicit
allow-list and non-initializing resolution by default; static initialization
must require an explicit authorized operation.

## Required type and lifetime rules

- Primitive values use exact-width, explicitly tagged representations.
- Strings use UTF-8 with a documented null/empty distinction.
- Arrays and collections require explicit element descriptors.
- Java object references cross as opaque handles, never raw pointers.
- Handles are invalidated when their host session closes.
- A Java exception is returned as structured data; it must not unwind through
  the C ABI or be silently converted to a default value.
- Unknown tags, invalid descriptors, oversized frames, stale handles, and
  disallowed classes fail closed.
- Requests must not implicitly run arbitrary class initializers.

## Implementation sequence

1. Keep the external JVM process path stable and independently testable.
2. Add the typed protocol schema and parser tests without enabling invocation.
3. Implement a Java helper endpoint using the existing allow-listed class loader.
4. Implement the native C/C++ transport and bounded request/response framing.
5. Add compiler/loader lowering for explicit Java interop expressions.
6. Add typed conversion, object-handle lifetime, and exception tests.
7. Qualify Linux, Windows, and macOS process handling before considering JNI.
8. Treat embedded JVM as a separate profile; do not enable it by configuration
   alone.

## Qualification language

Documentation and status output must use precise terms:

- External Java host available means a Java executable was found.
- JAR/class launch supported means the external process launch path is wired.
- Java API envelope present means a SLeeLa source declaration exists.
- Method invocation supported requires an end-to-end typed call test.
- Embedded JVM supported requires a separately built and qualified JNI profile.

A successful executable probe is not a Java API conformance result.
