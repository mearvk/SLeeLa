<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-logo-004.jpg" alt="SLeeLa">






# SLeeLa Java Library

The `/lib/java` library family defines the SLeeLa boundary for Java interoperability.

## Native Library Inventory

The current repository inventory contains **8,993 files under `/lib/java`**. This count is validated from the Git index by the repository CI rather than being treated as an unverified documentation number.

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
- the Git-indexed file count matches the recorded **8,993-file** baseline;
- manifest schema and root metadata are present;
- the expected Java interoperability boundary is documented;
- required source-role metadata is present; and
- the inventory has not silently fallen below its established baseline.

When files are intentionally added or removed, the baseline and manifest must be updated in the same change. CI therefore makes library growth an explicit engineering event rather than allowing the documented count to drift.

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

This makes the Java relationship traceable instead of leaving the 8,993-file collection as an opaque directory.

## Java API Dependency Graph

Run the static counterpart graph audit from the repository root:

```text
python3 lib/java/tools/java_api_dependency_graph.py --json java-api-dependencies.json
python3 tests/java_api_dependency_graph_suite.py
```

The graph indexes Java namespace counterpart types and records direct imports,
static imports, fully-qualified references, same-package and unambiguous
wildcard-import type uses, nested-type ownership, package inventories,
package-info annotations, module requires/exports/opens/uses/provides, and transitive
type reachability. It reports unresolved references and duplicate counterpart
paths with source locations.

The graph is deliberately separate from behavioral qualification. A resolved
edge proves only that a source-level counterpart is present; it does not prove
that its methods, constructors, annotations, inherited members, or runtime
behavior are equivalent. Wildcard package imports are recorded separately
from individual type edges, and ambiguous simple-name matches are surfaced
rather than silently guessed.

## Member-level API dependency audit

The member dependency auditor extends the type graph into declared API
contracts. Run it from the repository root:

```text
python3 lib/java/tools/java_api_member_dependencies.py --json java-api-member-dependencies.json
python3 tests/java_api_member_dependencies_suite.py
make -C tests java-api-member-dependencies
```

It extracts source-level dependencies from field types, method return and
parameter types, constructor parameters, generic bounds, inheritance clauses,
declared exceptions, and annotations. Each edge retains its declaring type,
member name, member kind, dependency role, source line, and resolved target.
Explicit imports whose Java namespace counterpart is missing are reported.

This is a static source inventory, not proof of exact JDK member coverage or
behavioral equivalence. In particular, inherited members are represented by
their declaring parent/interface dependencies; a separate inherited-contract
comparison must establish the effective member set for each concrete type.

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

## Expansion

Future Java library work should extend this family rather than placing Java interoperability types in unrelated library directories.

When extending `/lib/java`, update **the source file, its mapping/documentation where needed, `MANIFEST.json`, and validation expectations together**. This keeps the native library count, language bridge, compiler/loader visibility, and CI contract synchronized.
## Visibility and constructor inheritance audit

Run the source-level visibility/constructor audit with:

```text
python3 lib/java/tools/java_api_visibility_constructor_audit.py --json java-api-visibility.json
python3 tests/java_api_visibility_constructor_suite.py
make -C tests java-api-visibility-constructor
```

The report classifies declared members as public, protected, package-private,
or private; computes inherited public/protected members and same-package
package-private members; and records unresolved Java-namespace supertypes.
Private members are excluded from inheritance.

Java constructors are never inherited. The audit reports accessible superclass
constructors as explicit `super(...)` invocation options, not as members of
the subclass. It also models an implicit default constructor when a class
declares no constructor, using the class visibility.

The parser is intentionally conservative and operates on the repository's
SLeeLa source representations. Results are qualification evidence requiring
review, not a claim that every Java language edge case has been resolved.


## From Java API representations to executable SLeeLa implementations

The existing JDK counterpart files are an API/source representation layer, not
a completed translation of Java's implementation. In particular, a class
envelope containing metadata and generic `construct()` / `invoke()` command
strings does not implement the Java type's methods or semantics. The presence
of thousands of counterpart files must not be reported as thousands of
implemented Java classes.

The implementation phase therefore has a separate completion standard:

1. **API contract** — identify the Java type, constructors, methods, fields,
   overloads, exceptions, and relevant inherited behavior.
2. **SLeeLa implementation** — provide executable SLeeLa method bodies and
   state/data representations, not only names or generic dispatch strings.
3. **Runtime primitives** — add only the native C/C++ operations needed for
   well-defined VM services; keep language behavior in SLeeLa where practical.
4. **Conformance** — test observable behavior against documented Java
   contracts, including edge cases and exception behavior.
5. **Provenance** — record whether a method is SLeeLa-native, a thin native
   primitive, or an explicitly delegated JVM operation.

Start by measuring the current source depth:

```text
python3 tests/java_source_implementation_audit.py --top 40
python3 tests/java_source_implementation_audit.py --json java-source-implementation-audit.json
python3 tests/java_source_implementation_audit_suite.py
make -C tests java-source-implementation-audit
```

The audit separates generic Java dispatch placeholders, empty bodies,
constant-return placeholders, simple field accessors, and substantive-looking
method bodies. It is a prioritization aid, not a parser or behavioral proof.
A substantive-looking method still requires implementation and conformance
tests.

The first implementation families should be foundational and dependency-led:
`java.lang.Object`, primitive wrappers and numeric conversion, `String`,
`Math`, then collection interfaces and core implementations, followed by
I/O, NIO, time, networking, and concurrency. Each family should have a
SLeeLa-native implementation path wherever the language and VM can support it.
JVM delegation remains an explicit compatibility fallback, never a substitute
for claiming that a SLeeLa implementation exists.

Do not copy OpenJDK implementation source wholesale. Implement the documented
API contracts independently in SLeeLa, using the Java specification and
behavioral tests as references. Preserve license/provenance boundaries for
any third-party or upstream material.


## First SLeeLa-native java.lang implementations

Executable implementations are being developed separately from the generated
JDK API counterpart declarations. This prevents an API signature inventory
from being mistaken for working behavior.

Initial source units:

- `implementations/java/lang/JavaObjectIdentity.sleela` — identity equality
  and stable identity-derived hashing for a live object, provided the runtime
  supplies a stable heap identity and object handle.
- `implementations/java/lang/JavaStringValue.sleela` — immutable scalar-backed
  string values, equality, concatenation, non-negative repetition, and joining
  a supplied string array.

These are deliberately scoped subsets, not complete replacements for
`java.lang.Object` or `java.lang.String`. The Object adapter does not yet
implement monitor/wait-notify, cloning, or Java's default textual form. The
String implementation does not yet implement UTF-16 length/indexing,
substring, code-point iteration, case conversion, locale rules, regex,
normalization, or Java's specified hashCode. Null-argument exception behavior
also awaits a qualified SLeeLa exception boundary.

Run the executable smoke suites with:

```text
make -C tests java-lang-native
```

A successful run validates only the cases in those fixtures. The Java API
counterpart files remain contracts until each operation has a native SLeeLa
implementation and behavior-based qualification.


## Java UTF-16 Runtime Primitive Foundation

The native VM core now includes a strict, length-delimited UTF-8 to Java UTF-16
primitive layer at `impl/core/sleela_utf16.h` and
`impl/core/sleela_utf16.c`. It provides UTF-16 code-unit length and
`charAt` extraction, including independent high- and low-surrogate access
for supplementary Unicode scalar values. Malformed UTF-8, negative indices,
and out-of-range indices fail explicitly.

The focused native regression is `impl/core/sleela_utf16_smoke.c`, invoked by
`make -C impl test-java-utf16`; the object is also included in the reusable
`libslcore.a` archive. This is a native runtime primitive foundation, not yet
a JavaStringValue-to-VM binding: the SLeeLa String adapter continues to report
UTF-16 indexing as unsupported until the VM/native-call bridge can invoke these
functions and translate their status codes into Java exception behavior.
