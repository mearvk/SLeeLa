<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-logo-004.jpg" alt="SLeeLa">






# SLeeLa Java Library

The `/lib/java` library family defines the SLeeLa boundary for Java interoperability.

## Native Library Inventory

The repository inventory on 2026-11-12 contains **15,048 Git-indexed files under `/lib/java`**: 10,243 in `jdk-25/` (10,212 SLeeLa files plus the API dump and tools), 4,774 in `jdk-28/` (4,721 SLeeLa files plus its index, notes and a `module-summary.md` per module), and 31 top-level classes, documents and validators. The recorded baseline of 15,004 files is a floor, not the current count. This count is validated from the Git index by the repository CI rather than being treated as an unverified documentation number.

The collection is a native SLeeLa source surface for Java-facing language and framework concepts. It is intended to give the SLeeLa Compiler, Loader, SST, and Nordshrift a stable source-level vocabulary for Java packages, classes, procedures, interfaces, runtime concepts, and interoperability boundaries.

The inventory is deliberately distinguished from a copy of the Java implementation. A file in this directory represents a SLeeLa-side language object, binding, contract, facade, or interoperability definition unless its documentation explicitly identifies another role.

## The Java container

`/lib/java` is the **Java container**. Java definitions (`Math`, `Integer`,
`ArrayList`, ...) are library content housed here, implicitly by the import that
selects a tier (`import java.jdk-28;`) and explicitly in each unit's own
`javaType` / `javaModule` / `javaSource` fields. They are **not part of the
SLeeLa specification**; see `SLEELA.syntax` section 38.4.

## JDK 28, one SLeeLa file per Java type

[`jdk-28/`](jdk-28/README.md) holds one SLeeLa file for each of the 4,721 public
Java SE / JDK 28 types. The files are at the online API path
`jdk-28/<module>/<package>/<Type>.sleela`, with a folder for each of the 58 JDK 28
modules. Each file names its JDK 28 API page as its Java Source.

## Converting Java source to SLeeLa

[`converter/`](converter/README.md) is a SLeeLa program that walks a Java
source tree (for example a JDK `src.zip`, `<module>/<package>/<Type>.java`) and
writes one SLeeLa file per type to an output tree outside `lib/java`. Every
Java-to-SLeeLa decision is in `converter/JConvRules.sleela`, and the Java
operator, string-conversion and result-type laws are read from the shared model
`impl/subjects/conversion/conversion.model`. A member that uses a construct
SLeeLa cannot express yet becomes a stub that throws
`UnsupportedOperationException` naming the reason. Output declares syntax 3.1.
Status and measurements: [`STATUS.md`](STATUS.md) section 11 and
[`converter/README.md`](converter/README.md).

## Machine-Readable Inventory

`MANIFEST.json` is the machine-readable contract for this directory. It records the expected inventory count and the rules used by CI to validate the collection.

`validate.py` is the authoritative validator used by CI. It can also be run locally from the repository root:

```text
python3 lib/java/validate.py
```

The validator checks:

- the `/lib/java` directory exists;
- the Git-indexed file count is at least the recorded **15,004-file** baseline;
- manifest schema and root metadata are present;
- the expected Java interoperability boundary is documented;
- required source-role metadata is present; and
- the inventory has not silently fallen below its established baseline.

When files are intentionally added or removed, the baseline and manifest must be updated in the same change. CI therefore makes library growth an explicit engineering event rather than allowing the documented count to drift.

## Procedural Implementation (JDK 28)

`/lib/java` covers the Java procedural language in SLeeLa. Java types get
their JDK 28 fields, constructors, methods, return values and imports
written in SLeeLa, running on the SLeeLa VM.

As of 2026-11-12, **920 types have hand-written behavior in the `jdk-25/` tree and
849 in the `jdk-28/` tree** (section 1 of [STATUS.md](STATUS.md)). The first 322 types
(about 1,560 constructors and methods) were:

- the whole `java.*` Throwable hierarchy (290 types, with the real `extends` chain);
- `java.lang`: `Math`, `StrictMath`, `Integer`, `Long`, `Short`, `Byte`, `Double`, `Float`, `Character`, `Boolean`;
- `java.util` collections and utilities: `ArrayList`, `LinkedList`, `ArrayDeque`, `Vector`, `Stack`, `HashMap`, `LinkedHashMap`, `HashSet`, `LinkedHashSet`, `TreeMap`, `TreeSet`, `PriorityQueue`, `Iterator`, `Arrays`, `Collections`, `Objects`, `Optional*`, `Random`.

The library has two trees (see [STATUS.md](STATUS.md) section 0). Measured on
2026-11-12 against the reflected API of OpenJDK 25.0.2 (4,801 public types,
51,554 public members):

| | `jdk-25/` tree | `jdk-28/` tree (`import java.jdk-28;`) |
|---|---:|---:|
| Types with a file | 4,801 (100%) | 4,656 (97.0%) |
| Members covered | 23,432 (45.5%) | 12,591 (24.4%) |
| Members with working behavior | 5,165 (10.0%) | 4,123 (8.0%) |
| Exact-signature methods awaiting logic | 24,101 | 33,021 |

Reports: [COVERAGE.md](COVERAGE.md) (`jdk-25/`) and [COVERAGE-JDK28.md](COVERAGE-JDK28.md)
(`jdk-28/`). The files that hold working members are listed in
[IMPLEMENTED.md](IMPLEMENTED.md). In the `jdk-25/` tree, 9,675 abstract methods
throw `AbstractMethodError`, as the JVM does, and the other signatures throw
`UnsupportedOperationException` naming the signature until their logic is
written. Static members are reached through the class name:
`DayOfWeek.MONDAY`, `Integer.MAX_VALUE`.

Overall progress is summarized in [STATUS.md](STATUS.md).

Conventions, the deferred keyword/semantics list, and regeneration steps are
in [JDK28-PROCEDURAL-IMPLEMENTATION.md](JDK28-PROCEDURAL-IMPLEMENTATION.md).
The conformance programs are in [`examples/java`](../../examples/java/README.md).
The remaining types are still envelopes.

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

This makes the Java relationship traceable instead of leaving the `/lib/java` collection as an opaque directory.

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

## SLeeLa-native String UTF-16 operations

The `JavaStringValue.sleela` facade now stores VM-owned `SL_UTF16` values
and delegates String operations to compiler intrinsics. Its former Native
Drive dependency is no longer used for String length, indexing, comparison,
or hashing. Its `bind(moduleHandle, text)` signature is retained for source
compatibility, while the supplied text is converted once from legacy UTF-8.

### VM-backed Java String value

The native VM now appends `SL_UTF16` to the `SLType` enum without changing
any pre-existing tag number. A value carries a VM-local handle to a
length-delimited UTF-16 object; its storage preserves U+0000 and isolated
surrogates. The VM GC marks these values from globals, constant pools, active
stacks, suspended native-callback stacks, struct fields, and arrays. Host code
holding a returned value outside VM-managed storage must explicitly call
`slvm_java_string_root()` and later `slvm_java_string_unroot()`.

The public C API now exposes construction, copy, length, `charAt`,
`codePointAt`, `codePointCount`, `offsetByCodePoints`, `substring`,
`concat`, `equals`, `hashCode`, and `compareTo`. Bytecode `OP_CONST`
can load UTF-16 values placed in the constant pool through
`slvm_add_const_utf16()`; `OP_ADD`, `OP_EQ`, and `OP_NE` handle UTF-16
values without routing them through the legacy UTF-8 intern table.

### Artifact format v3

Artifact format v3 serializes UTF-16 constant and global values as a
32-bit little-endian code-unit count followed by little-endian 16-bit code
units. Loading allocates fresh VM-local handles; runtime handles themselves
are never persisted. The loader continues to accept artifact versions 1 and
2, and rejects the new `SL_UTF16` tag in those older formats. The VM ABI minor
version has advanced to 3.

A separate `SLJS` version-1 record codec is available for components that need
a standalone portable UTF-16 record. It is not a substitute for the artifact
container and is not used to change the legacy `SL_STR` encoding.

### Remaining source integration boundary

The `JavaStringValue.sleela` facade is now switched to the VM-backed `SL_UTF16`
value. Its bind/concat/substring, UTF-16 indexing, code-point, equality,
comparison, and Java hash operations use compiler intrinsics rather than
Native Drive. The compatibility `bind(moduleHandle, text)` signature remains,
but the module handle is no longer used for String operations. The general
SLeeLa `String`/`SL_STR` type and existing UTF-8 semantics remain unchanged;
callers can explicitly convert a legacy string with `javaStringFromUtf8`.
Typed, catchable Java exception objects and automatic instance-method dispatch
for the general `String` type are still not implemented.

Focused validation targets are:

```sh
make -C impl test-java-string
make -C impl test-java-string-vm
```

The VM smoke test covers NUL/surrogate preservation, VM handle operations,
GC root management, the portable record round trip, and artifact v3 save/load.
These targets have been added but have not yet been executed in this work.


## VM-backed Java UTF-16 String construction

The VM integration branch adds the source-level compiler intrinsic
`javaStringFromCodeUnits(...)` for `#sleela 3.0`. It accepts integer
literals in the inclusive range `0..65535` and emits an `SL_UTF16`
constant directly. This is deliberately distinct from the legacy UTF-8
`String` literal path and does not cross a C-string boundary.

For example:

```sleela
#sleela 3.0
class Example {
    static void main() {
        String value = javaStringFromCodeUnits(0x0041, 0x0000, 0xD800);
        print(value);
    }
}
```

The arguments are UTF-16 *code units*, not Unicode scalar values. The
constructor therefore preserves embedded U+0000 and isolated surrogate
values. It is compile-time/literal-only; runtime conversion of ordinary
well-formed UTF-8 values is provided separately by `javaStringFromUtf8`.
The general SLeeLa `String` type and its ordinary instance-method syntax
still use the legacy `SL_STR` path. The Java library's `JavaStringValue`
wrapper, however, now stores `SL_UTF16` values and implements its String
operations through the explicit intrinsics below.


### Source-level Java String operations

The compiler now lowers the following explicit `#sleela 3.0` intrinsics to
append-only VM opcodes operating on `SL_UTF16` values:

| Intrinsic | Java operation |
|---|---|
| `javaStringLength(value)` | `String.length()` |
| `javaStringCharAt(value, index)` | `String.charAt(int)` (returns a UTF-16 code unit) |
| `javaStringCodePointAt(value, index)` | `String.codePointAt(int)` |
| `javaStringCodePointCount(value, begin, end)` | `String.codePointCount(int, int)` |
| `javaStringOffsetByCodePoints(value, index, offset)` | `String.offsetByCodePoints(int, int)` |
| `javaStringSubstring(value, begin, end)` | `String.substring(int, int)` |
| `javaStringConcat(left, right)` | `String.concat(String)` |
| `javaStringHashCode(value)` | `String.hashCode()` |
| `javaStringCompareTo(left, right)` | `String.compareTo(String)` |

These are explicit intrinsics rather than automatic method dispatch: they reject
legacy `SL_STR` values at runtime and require VM-owned UTF-16 values. The
substring and concatenation opcodes root their input handles across allocation.
Invalid indices currently surface as VM runtime errors; Java-compatible,
catchable exception objects and ordinary instance-method syntax remain future
work. The VM ABI minor version is advanced for the append-only opcode set.


### Runtime UTF-8 conversion

The source intrinsic `javaStringFromUtf8(value)` converts a runtime legacy
SLeeLa `SL_STR` into a VM-owned `SL_UTF16` value. It decodes valid UTF-8
into UTF-16 code units, including supplementary Unicode scalar values as
surrogate pairs. Malformed UTF-8 and allocation failures fail closed with a
VM runtime diagnostic. The current legacy `SL_STR` representation is
NUL-terminated, so this conversion cannot preserve embedded NUL bytes; use
`javaStringFromCodeUnits(...)` when exact code units, embedded U+0000, or
isolated surrogates are required.

The conversion is an append-only VM opcode and advances the VM ABI minor
version. The source smoke program now checks conversion of an ASCII character,
a supplementary character, and a non-ASCII BMP character, then exercises
UTF-16 length and code-point access on the result.


### java.lang.String instance methods (SLEELA.syntax section 41)

A method called on a String value, `s.length()` or `"x".equals(y)`, binds to
the static method of the same name in
`implementations/java/lang/JavaStringMethods.sleela`, the receiver passed as
the first argument. The class is loaded only when a program calls a method on
a String. It covers the core `java.lang.String` instance surface: `length`,
`isEmpty`, `charAt`, `codePointAt`, `codePointBefore`, `codePointCount`,
`toCharArray`, `substring`, `subSequence`, every `indexOf` / `lastIndexOf`
form, `contains`, `startsWith`, `endsWith`, `regionMatches`, `equals`,
`contentEquals`, `equalsIgnoreCase`, `compareTo`, `compareToIgnoreCase`,
`hashCode`, `concat`, `repeat`, both `replace` forms, `toUpperCase`,
`toLowerCase`, `trim`, `strip`, `stripLeading`, `stripTrailing`, `isBlank`,
`intern` and `toString`. Exceptions carry the JDK 25 messages. Not yet
provided: the regex members (`matches`, `split`, `replaceAll`,
`replaceFirst`), the Locale forms, streams (`chars`, `codePoints`, `lines`),
the final-sigma and Turkish dotted-i case rules, and the static members of
`java.lang.String` (`valueOf`, `join`, `format`).

### Runtime conversion back to UTF-8, and run-time code units

Two append-only opcodes (VM ABI 1.8) close the bridge in the other direction:

- `javaStringToUtf8(value)` (`OP_JSTR_TO_UTF8`, 183) converts a VM-owned
  `SL_UTF16` value into an equivalent legacy `SL_STR`, for any Unicode text,
  supplementary characters included. The conversion is lossless or refused:
  an unpaired surrogate (no UTF-8 form), an embedded NUL (the legacy String is
  NUL-terminated) and text longer than `SL_MAX_STRTEXT` bytes fail closed with
  a VM runtime diagnostic.
- `javaStringFromCodeUnit(codeUnit)` (`OP_JSTR_FROM_CODE_UNIT`, 184) builds a
  one-unit `SL_UTF16` value from a run-time code unit 0..65535, the run-time
  companion of the compile-time `javaStringFromCodeUnits(...)`.

`implementations/java/lang/JavaText.sleela` is the library entry point. Its
`plainOf` checks for NUL, unpaired surrogates and over-length text first and
throws a catchable `UnsupportedOperationException` instead of reaching the VM
diagnostic; `codePointU` and `unitU` build text one character at a time in
UTF-16, so a surrogate pair is never split. `StringBuilder`, `StringBuffer`,
the character writers and readers, `File` and `Properties` use them, so those
classes carry any Unicode text. Both intrinsics require `#sleela 3.0`.

The Java String facade source now delegates its operations to the VM-backed
intrinsics. Its stored value is initialized as an empty `SL_UTF16` string;
`bind(moduleHandle, text)` preserves its older signature while converting the
input once and no longer calls Native Drive. The wrapper exposes UTF-16
`length`/`charAt`, code-point navigation, substring, concat, equals,
compareTo, and Java hashCode.



## Java API dependency and contract audits

The Java API counterpart inventory now has three complementary static audits:

- `java_api_dependency_graph.py` records direct and transitive type dependencies, imports, nested-type ownership, wildcard package references, module/package metadata, unresolved references, and duplicate counterpart paths.
- `java_api_member_dependencies.py` extracts field, return, parameter, constructor, generic-bound, inheritance, throws, and annotation dependencies at member level.
- `java_api_visibility_constructor_audit.py` distinguishes declared from inherited members, models public/protected/package-private/private visibility, and records accessible superclass constructors as explicit `super(...)` options. Constructors are never treated as inherited members.

Run the focused qualification suite with:

```sh
make -C tests java-api-dependencies
make -C tests java-api-dependency-graph
make -C tests java-api-member-dependencies
make -C tests java-api-visibility-constructor
```

These tools are conservative source analyzers. They establish counterpart structure and report gaps; they do not prove Java behavioral equivalence, full JDK member coverage, or runtime compatibility.

## Source implementation depth audit

The API inventory is not itself an implementation. The source implementation audit classifies counterpart bodies into generic dispatch placeholders, empty bodies, constant-return placeholders, simple accessors, and substantive-looking implementations. It is a prioritization aid rather than a parser or behavioral proof.

```sh
make -C tests java-source-implementation-audit
```

Implementations should be developed dependency-first, with explicit provenance for SLeeLa-native behavior, native C/C++ primitives, or delegated JVM compatibility. Do not report API envelopes as completed Java implementations.

## Initial java.lang implementation contracts

`implementations/java/lang/JavaObjectIdentity.sleela` defines identity equality and identity-derived hashing for a live object when the runtime supplies a stable heap identity and object handle. Monitor/wait-notify, cloning, and default textual conversion remain explicitly unsupported.

`implementations/java/lang/JavaStringValue.sleela` is the VM-backed UTF-16 facade documented above. Its operations use explicit UTF-16 intrinsics; it is not the older scalar-backed subset and does not imply automatic instance-method dispatch for the general SLeeLa `String` type.
