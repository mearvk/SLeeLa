<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-logo-004.jpg" alt="SLeeLa">






# SLeeLa Java Library

The `/lib/java` library family defines the SLeeLa boundary for Java interoperability.

## Native Library Inventory

The current repository inventory contains **8,990 files under `/lib/java`**. This count is validated from the Git index by the repository CI rather than being treated as an unverified documentation number.

The collection is a native SLeeLa source surface for Java-facing language and framework concepts. It is intended to give the SLeeLa Compiler, Loader, SST, and Nordshrift a stable source-level vocabulary for Java packages, classes, procedures, interfaces, runtime concepts, and interoperability boundaries.

The inventory is deliberately distinguished from a copy of the Java implementation. A file in this directory represents a SLeeLa-side language object, binding, contract, facade, or interoperability definition unless its documentation explicitly identifies another role.

## Machine-Readable Inventory

`MANIFEST.json` is the machine-readable contract for this directory. It records the expected inventory count and the rules used by CI to validate the collection.

`validate.py` is the authoritative validator used by CI. It can also be run locally from the repository root:

```text
python3 lib/java/validate.py
```

The validator checks:

- the `/lib/java` directory exists;
- the Git-indexed library-file count exactly equals the manifest count (8,990 at this revision);
- every declared control file is tracked and excluded from the library count;
- manifest schema, root, bridge, source-role, execution-boundary, and mapping metadata are valid; and
- missing or additional tracked files cause a validation failure until the manifest is deliberately updated.

When library source files are intentionally added or removed, update inventory.count in the same change. Documentation and validator control files are explicitly listed in inventory.control_files and excluded. The validator uses exact equality: both unexpected growth and unexpected shrinkage fail. The current count includes the new protocol specification and machine-readable schema.

## Java-to-SLeeLa Mapping

The intended mapping is:

`Java package/class/procedure → SLeeLa Family symbol → SLeeLa source → compiler/loader → runtime service`

A Java-facing source unit should identify, where applicable:

1. **Java owner** — the Java package, class, interface, record, enum, method, or framework concept represented.
2. **SLeeLa symbol** — the stable SLeeLa class, symbol, facade, or procedural definition.
3. **Source role** — binding, facade, native boundary, runtime service, compatibility definition, or other documented role.
4. **Execution boundary** — SLeeLa-native, C, C++, Java/JVM, or platform-specific implementation.
5. **Loader visibility** — how the Compiler and Loader discover the symbol.
6. **Compatibility surface** — the Java/JDK version or API family represented.

This makes the Java relationship traceable instead of leaving the 8,990-file collection as an opaque directory.

## JDK 28 Framework Bindings

SLeeLa provides controlled source-level bindings for selected JDK 28 framework types. JDK 28 currently has a draft/ad-hoc API specification, so these files describe the framework contract without copying or reimplementing Java's internal implementation.

The initial binding families include:

- `java.lang`
- `java.util`
- `java.io`
- `java.nio`
- `java.nio.file`
- `java.net`
- `java.time`
- `java.math`

Additional JDK framework families will be added as the SLeeLa Java bridge matures, including reflection, concurrency, security, HTTP, class-file APIs, and compiler APIs.

## SLeeLa Control

A Java framework binding does **not** make SLeeLa subordinate to Java. The SLeeLa Compiler and Loader remain responsible for resolving the SLeeLa source model and deciding when a Java framework operation is permitted.

The intended model is:

`SLeeLa source → SLeeLa Compiler → controlled Java binding → JVM/JDK service`

In the parallel execution model, Java procedural behavior may also have a pre-compiled SLeeLa source representation:

`Java procedure ↔ SLeeLa Family representation ↔ pre-compiled SLeeLa source ↔ SLeeLa VM`

This permits a user to employ Java framework facilities while retaining SLeeLa syntax, source definitions, loading rules, and runtime control.

## SLeeLa on a C/C++ Virtual Machine

SLeeLa executes on a **C/C++ native virtual-machine foundation**. C and C++ provide the machine-facing runtime substrate, including memory and process boundaries, platform integration, native facilities, and the mechanisms needed to load and execute SLeeLa procedural representations.

The core execution path is:

`SLeeLa source → Compiler/Loader → SLeeLa VM → C/C++ native runtime`

Java is a cooperating runtime/language surface, not a replacement for the SLeeLa VM:

`Java source/procedure ↔ SLeeLa Family representation ↔ pre-compiled SLeeLa source ↔ SLeeLa VM`

C, C++, Java, and SLeeLa therefore retain distinct responsibilities. SLeeLa supplies the language and procedural source model; the SLeeLa VM supplies execution; C/C++ supplies the native foundation; and Java supplies a cooperating language/runtime surface represented through defined SLeeLa boundaries.

## Compiler and Loader

The Java library is intended to be visible to the SLeeLa Compiler, Loader, and Nordshrift symbol collection.

New Java-facing source units should participate in:

`source → Java/SLeeLa mapping → package/library resolution → semantic analysis → compilation → artifact/loader resolution`

A source file should not be considered integrated merely because it exists under `/lib/java`; it should be discoverable through the same package, symbol, compiler, and loader mechanisms used by the rest of the SLeeLa library.

## Source Standard

Every SLeeLa source file in this directory should include, as applicable:

- A documentation header.
- An explicit Definition.
- A stable SLeeLa class or symbol declaration.
- A declared Java framework type or interoperability role.
- A documented execution boundary.
- No copied Java implementation code.
- No undocumented native behavior.

## Interoperability Test Targets

The Java bridge should be validated in both directions where a feature supports it:

`Java procedure → SLeeLa representation → compile → SLeeLa VM`

`SLeeLa procedure → Java-compatible representation → Java-side execution`

The native execution foundation should also remain testable:

`SLeeLa source → Compiler → C/C++ VM → execution`

These paths are architectural test targets; a particular binding must not claim support until its implementation and CI tests establish that support.

## Runtime Capability Boundary

The library's source envelopes describe Java API types; they do not, by themselves, implement Java method execution. The repository currently has an external-JVM host path for Java source, runnable JARs, and compiled class folders. The Java connector also provides allow-listed reflective construction and invocation through SleelaJdkBridge and SleelaClassLoader. These are distinct integration layers: the external host is a child process, and the Java-side reflection helper is not yet a SLeeLa VM invocation opcode. Neither should be described as JNI or an embedded JVM.

The SLJIP v1 endpoint now implements Java-side construction, method invocation, public field access, opaque object handles, and structured exception responses. The native C codec is linked into the native tools and exposes a callback-based exchange API. However, the SLeeLa compiler/VM does not yet lower source expressions to these operations, and the native runtime does not yet own the endpoint process pipes. Descriptive strings such as java.invoke: are not invocation opcodes. See protocol/SLJIP-v1.md, RUNTIME-INTEGRATION.md, and impl/JAVA-INTEROP-LOWERING.md for the current boundaries.

## JDK Signature Catalog

The JDK 28 signature synchronizer records official Javadoc member declarations as comments inside a generated catalog section. It does not generate executable method bodies or placeholder return values. This keeps API metadata useful without suggesting that a stub returning false, zero, or null is a conforming Java implementation.

Use tools/jdk28_signature_sync.py --check to audit drift, or --limit N for a controlled batch. Run the focused regression tests with python3 test-suites/java-library-integrity.py from the repository root.

## Typed Java Invocation Protocol

The first versioned transport contract is documented in [protocol/SLJIP-v1.md](protocol/SLJIP-v1.md), with a machine-readable description in [protocol/SLJIP-v1.json](protocol/SLJIP-v1.json). The Java helper endpoint is `SleelaJdkProtocolServer`; the native C frame codec is `runtime/java_interop_protocol.c`. The protocol supports bounded frames, typed primitive/string values, allow-listed class resolution, construction, public instance/static invocation, opaque per-session object handles, release, and structured exception responses.

The Java endpoint and native frame codec have independent focused test runners:

- `bash test-suites/java-interop-protocol-endpoint.sh`
- `bash test-suites/java-interop-protocol-codec.sh`
- `python3 test-suites/java-interop-protocol.py` (cross-platform orchestration; runs both suites)
- `make -C connector protocol-test protocol-codec-test`

The native codec is linked into both native tool binaries. The callback-driven exchange API is ready for a process/pipe provider. Compiler/VM lowering is still a separate required integration step: no SLeeLa source expression is yet claimed to execute these protocol operations.

## Expansion

Future Java library work should extend this family rather than placing Java interoperability types in unrelated library directories.

When extending `/lib/java`, update **the source file, its mapping/documentation where needed, `MANIFEST.json`, and validation expectations together**. This keeps the native library count, language bridge, compiler/loader visibility, and CI contract synchronized.