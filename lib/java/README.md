<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-logo-004.jpg" alt="SLeeLa">






# SLeeLa Java Library

The `/lib/java` library family defines the SLeeLa boundary for Java interoperability.

## Native Library Inventory

The current repository inventory contains **8,989 files under `/lib/java`**. This count is validated from the Git index by the repository CI rather than being treated as an unverified documentation number.

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
- the Git-indexed file count matches the recorded **8,989-file** baseline;
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

This makes the Java relationship traceable instead of leaving the 8,989-file collection as an opaque directory.

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

The legacy `JavaStringValue.sleela` adapter still exposes `length()` and
`charAt(int)` through the host-authorized Native Drive scalar/UTF-8 ABI.
That adapter accepts well-formed UTF-8 input and cannot itself preserve
embedded NUL or isolated surrogate code units. It remains a compatibility
facade, not the new VM-backed Java String object.

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

The SLeeLa-level `JavaStringValue.sleela` facade has not yet been switched to
the VM-backed value. The compiler and source library still need a source-visible
UTF-16 String type/constructor and method lowering so Java-facing source can
create and manipulate `SL_UTF16` values without a host C caller. The legacy
`String`/`SL_STR` type, native scalar ABI, and existing UTF-8 semantics remain
unchanged. Typed, catchable Java exception objects are also not yet implemented.

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
values. At this stage the constructor is compile-time/literal-only: runtime
code-unit arrays, instance methods such as `length()` and `charAt()`, and
automatic lowering of every ordinary SLeeLa string literal to `SL_UTF16`
remain separate compiler/runtime integration work. Existing `String`
operations continue to use the legacy `SL_STR` representation unless a
source intrinsic explicitly constructs an `SL_UTF16` value.


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
