<img align="right" src="images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# SLeeLa International — Language and Feature Reference

**Edition:** International Version 3.0  
**Language syntax:** SLeeLa 3.0  
**Source extension:** `.sleela` (Wrapper™)  
**Normative grammar:** [`SLEELA.syntax`](SLEELA.syntax)  
**Compiler/runtime implementation:** see [`VERSION.md`](VERSION.md) and [`markdown/COMPILER.md`](markdown/COMPILER.md)

> This document is an organized, reader-facing overview of the SLeeLa language and its feature surface. The normative grammar and implementation-specific references linked below remain authoritative where a detail differs. A feature's presence in the 3.0 reference does not mean every possible width, platform bridge, library adapter, or optional subsystem has identical end-to-end validation.

---

## 1. What SLeeLa is

SLeeLa is a Java-like, class-oriented programming language built around a native compiler, a C/C++ execution core, a virtual-machine instruction set, and a large SLeeLa-authored standard library.

Its central idea is that a programming language is not only its grammar. It is the connected system of:

- a source language and versioned file format;
- a compiler front end and semantic pipeline;
- a stable native execution core and VM bytecode;
- a growing, recursively discovered standard library;
- explicit operating-system and native-service bridges;
- development, documentation, packaging, and verification tools.

SLeeLa source is compiled to Core bytecode and executed by the native core. The compiler front end is implemented in C++ and drives the C core through its exchange interface; it is not a separate interpreter that independently executes the language.

The project describes its work through four complementary areas: **Math**, **Object**, **Science**, and **Reach**. These describe calculation, the organization of things, disciplined inquiry, and the ability to connect software to other systems and people. The project's national-interest and provenance vocabulary is retained as descriptive metadata where relevant; it does not replace ordinary programming semantics.

## 2. Versioning and compatibility

SLeeLa keeps the language edition separate from the compiler/runtime product release.

| Version track | Meaning |
|---|---|
| **International Version 3.0** | The edition designation used by this overview. |
| **Syntax 3.0** | The current normative source-language grammar. |
| **Compiler/runtime release** | The independently versioned implementation; query `sleela version` and consult `VERSION.md`. |
| **Library/API edition** | The versioned API documentation and snapshots generated from the current `/lib` source tree. |

A source file may declare its grammar edition with a first-line version pragma:

```sleela
#sleela 3.0
```

The current compiler gate accepts declared syntax versions **1.0 through 3.0**. If the pragma is omitted, the compiler defaults to 3.0 with a warning. A version outside the accepted range, or a malformed declaration, is rejected.

Syntax 3.0 consolidates the established 1.x and 2.x language surface. It does **not** erase historical feature gates: a construct introduced in 2.9 remains gated at 2.9 even when compiled by a 3.0-capable compiler.

See [the syntax-version matrix](api/sleela/2.1/language/SYNTAX-VERSION-MATRIX.md).

## 3. Source files and program structure

The human-authored program file is a **Wrapper™**, with the `.sleela` extension. It is UTF-8 text. A program consists of one or more class declarations and may include struct declarations.

A program has exactly one entry method:

```sleela
void main() {
    print("Hello, SLeeLa");
}
```

The entry signature is `void main()`. The source format, version pragma, and lifecycle are described in [`markdown/SOURCE.md`](markdown/SOURCE.md).

### 3.1 Lexical conventions

- Identifiers are case-sensitive and begin with a letter or underscore.
- Whitespace separates tokens and is otherwise insignificant.
- Decimal, hexadecimal (`0x`), binary (`0b`), and octal (`0o`) integer literals are supported; underscores may separate digits.
- Floating-point literals use a decimal point and may include an exponent.
- Boolean literals are `true` and `false`; `null` is the null literal.
- String literals use double quotes and support the documented escapes: newline, tab, carriage return, backslash, and quote.

Both conventional comments (`//`, `/* ... */`) and the preferred angle-bracket comment forms are recognized. The go-forward house style is:

```sleela
< A line comment >
<<
    A block comment
    spanning multiple lines
>>
```

The angle-bracket opener must begin as the first non-whitespace text on its line. See [`SLEELA.syntax` §1](SLEELA.syntax).

## 4. Types and data model

### 4.1 Core source types

The core source vocabulary includes:

- `int` — signed 64-bit integer;
- `double` — floating-point number;
- `boolean` — logical value;
- `String` — first-class string value;
- `void` — method return type;
- declared `struct` types;
- supported generic reference-type signatures;
- additional named integer and character-catalogue signature families described below.

The core runtime has a dynamic value representation underneath the language surface. The syntax specification distinguishes source-level declarations from the runtime's actual enforcement and representation. Do not assume that every declared width or signature automatically has every arithmetic operation implemented end-to-end.

### 4.2 Structs and object handles

A `struct` declares a named aggregate of fields:

```sleela
struct Point {
    int x;
    int y;
}

class Example {
    void main() {
        Point p = new Point();
        p.x = 10;
        p.y = 20;
        print(p.x + p.y);
    }
}
```

Struct values are VM-managed instance handles, not exposed raw pointers. Assignment and parameter passing share the same instance; equality is identity-based. Struct fields are initially null/zero according to the runtime's documented initialization rules. The specification sets limits on struct types, fields, and live instances per VM.

Struct values can be converted to a portable JSON representation using `structPack(instance)` and reconstructed with `structUnpack(TypeName, json)`. This is the supported shape for moving struct data through text-based transports or thread mailboxes; a VM-local handle itself is not a portable wire value.

### 4.3 Classes, fields, and methods

A class contains fields and methods. Fields are class-level shared state in the current execution model; method locals are per-thread. Field initializers are initialized in declaration order as part of program startup.

Methods use declared parameter and return types. The `static` modifier is accepted; the current model treats methods as class-bound. Methods can call other methods and recurse.

### 4.4 Inheritance and nested classes

Syntax 1.10 introduced resolved, single-base class inheritance:

```sleela
class Animal {
    String name;
    String describe() { return "animal " + name; }
}

class Dog extends Animal {
    String describe() { return "dog " + name; }
}
```

Derived classes inherit base fields and methods; a same-name derived method overrides the inherited method. Unknown bases and inheritance cycles are semantic errors.

The same keyword also has a distinct top-level document relationship form, `extends to ...`, which organizes document targets as `linear`, `group`, or `services`. This is not class inheritance; see [`markdown/EXTENDS.md`](markdown/EXTENDS.md).

Syntax 1.11 adds one level of nested class declarations. The enclosing class plus its nested class form the maximum lexical nesting depth; deeper types should be declared at top level and referenced by field.

### 4.5 Generics

Syntax 2.2 adds class-level generic type parameters, parameterized reference types, and wildcards, including `?`, `? extends T`, and `? super T`. The specification restricts the accepted generic forms: primitive and keyword `String` type arguments are not interchangeable with reference-type arguments, and method-level type parameters are not part of the accepted grammar.

### 4.6 Numeric families

In addition to the 64-bit `int`:

- `long` and `int64` are documented as 64-bit signed aliases;
- `int128` is accepted as an integer type but is documented as 64-bit-backed for now;
- syntax 2.7 defines signed power-of-two names from `int32` through `int16777216`;
- syntax 2.8 defines the corresponding unsigned family from `uint32` through `uint16777216`;
- the earlier `U<n>` family describes unsigned values with a declared width from 1 to 1,048,576 bits;
- `C<n>` describes character-catalogue capacity from 1 to 1,048,576 entries. It is a capacity signature, not an integer type or a guarantee that all entries are populated.

The arbitrary-width unsigned value foundation and type recognition are documented separately from complete VM operator coverage. Likewise, character-catalogue declarations must not be mistaken for a claim that every catalogue is Unicode or that every declared entry is populated. See [`markdown/SOURCE.md`](markdown/SOURCE.md) and [`SLEELA.syntax`](SLEELA.syntax).

## 5. Expressions, operators, and statements

### 5.1 Operators

The expression grammar defines:

| Category | Operators |
|---|---|
| Arithmetic | `+`, `-`, `*`, `/`, `%` |
| Comparison | `==`, `!=`, `<`, `<=`, `>`, `>=` |
| Logical | `&&`, `||`, `!` |
| Assignment | `=` |
| Grouping | `()` |

The `+` operator performs numeric addition or String concatenation when a String operand is involved. Equality on struct handles compares identity. The language's precedence is conventional: logical OR, logical AND, equality, comparison, additive, multiplicative, unary, then primary expressions.

### 5.2 Control flow

The language provides:

- blocks with braces;
- local declarations and assignments;
- `if` / `else`;
- `while`;
- `for`;
- `return`;
- `print(...);`;
- expression statements, commonly method or built-in calls.

The `for` form supports the ordinary initializer, condition, and update. The extended form can include a Boolean thread-stability guard and a Boolean VM/heap-health guard. Guards are checked at the start of each iteration before the body. A heap-health guard only reflects real VM health if the host/runtime connects it to a genuine health signal.

### 5.3 Garbage-collection hints

Syntax 1.9 adds the contextual forms:

```sleela
target = gc 50;
target = mem 25;
```

These request short-term garbage-collection cleanup with an optional 0–100 aggressiveness value. They are not ordinary assignments and do not guarantee immediate reclamation; reachable objects remain valid. The target retains its value.

## 6. Conducted methods and the system object catalogue

A distinctive SLeeLa facility is the conducted-method vocabulary, which addresses named objects in `SHEET.sheet`. The contextual built-ins are:

| Built-in | Purpose |
|---|---|
| `conduct(name)` | Test whether a named object is catalogued. |
| `role(name)` | Return the object's role. |
| `insight(name)` | Return its descriptive insight. |
| `congruent(a, b)` | Test the documented congruence relationship. |
| `route(a, b)` | Return a route description when the objects are congruent. |
| `sysdepth()` | Read the system-depth limit. |
| `degreemax()` | Read the maximum complexity degree. |

The catalogue describes sixteen role categories, including root, start, idiom, highlight, call, method, attention, gain, conversation, piping, transitive, completitive, inheritance, smart-move, congruence, and limit.

The documented system invariants are a maximum system depth of **3024** and a maximum complexity degree of **4**. These values describe the project's catalogued connection model; they are not a substitute for application-level validation.

## 7. Concurrency and communication

SLeeLa exposes a bounded thread-friendly model through contextual built-ins:

| Built-in | Purpose |
|---|---|
| `spawn(method)` | Start a no-argument method on a new thread and obtain its thread identifier. |
| `join()` | Wait for spawned threads to finish. |
| `lock(n)` / `unlock(n)` | Acquire or release a lock-table slot. |
| `send(slot, value)` | Place a value on a mailbox slot. |
| `recv(slot)` | Wait for a value on a mailbox slot. |

The documented core limit is 128 threads and the lock-table slot range is 0–31. Shared fields should be protected with locks; mailboxes are the intended way to exchange values between threads. Printed lines are atomic across threads.

### 7.1 Structural send

The `:::` operator expresses a String transfer to a receiver handle:

```sleela
source ::: receiver;
```

The VM performs the element-by-element stream framing and transport. The left operand is a String and the receiver must evaluate to a suitable reach handle. The expression evaluates to the original source String, allowing a chain of receivers. This is a VM transport operation, not ordinary punctuation or a replacement for general networking APIs.

## 8. Reach, measurement, and selection built-ins

The language includes contextual built-in families beyond the core control-flow vocabulary:

- **Munction** — `Munction.start(name)` and its fluent reach verbs, including connection, enablement, send, consume, observe, propagation, latch, and receipt-oriented closure operations. The chain is a structured reach sentence; consult [`MUNCTION.md`](MUNCTION.md) for the full contract.
- **Synchro** — `synchroOpen`, `synchroDispatch`, `synchroReport`, `synchroClose`, and measurement operations for sent/received counts, mean, minimum, maximum, P95, and loss. These are measurement surfaces; see [`SYNCHRO.md`](SYNCHRO.md).
- **Best-of** — the `bestOf...` family models candidate, architecture, cost-budget, version, score, choice, report, mean, loss, jitter, and certainty data. See [`TIMINGS.md`](TIMINGS.md) and [`NETWORK.md`](NETWORK.md).
- **System namespace** — `ran::` qualifies system references such as `ran::mem` and `ran::gc`.

These names are contextual built-ins, not all reserved words. They are recognized in their documented call or namespace positions and should be used according to their subsystem references.

## 9. Syntax 3.0 behavioral additions

Syntax 3.0 includes several additions that go beyond declarative source annotations.

### 9.1 Loop timing

- `@loopDelay` inserts a fixed pause per loop iteration.
- `@loopBudget` distributes a known iteration count over a requested wall-clock budget.
- `@spill` marks a loop timer-start behavior as defined by the normative syntax specification.
- `sleep(ms)` blocks the current thread for a millisecond count.

The loop annotations lower to the VM sleep operation (`OP_SLEEP`, ISA index 157). See [the loop-timing API reference](api/sleela/3.0/LOOP_TIMING_API.md) and [the runnable example](examples/loop-timing-annotation-demo.sleela).

### 9.2 Big-integer operations

Syntax 3.0 defines native built-ins:

- `bigmul(a, b)`;
- `bigadd(a, b)`;
- `bigdivmod(a, b)`.

They operate on base-1,000,000,000 limb arrays and lower to dedicated VM arithmetic operations. The documented array ceiling is 2^24 elements. This interface is a limb-array arithmetic facility; it should not be confused with the ordinary 64-bit `int` type or a claim that every integer width in the language has arbitrary-precision semantics.

### 9.3 Explicit floating-point narrowing

The syntax 3.0 specification records explicit casts from `double` to integer types. Narrowing is explicit rather than an implicit conversion.

## 10. Preonems and document markers

SLeeLa includes a contextual family of declarative document markers, collectively called the **Preonems**. They describe the standing or nature of a source document. Except where specifically noted, they do not change parsing, type checking, code generation, or runtime behavior.

The documented marker vocabulary includes:

- `author` / `authorial` — the compiler-active marker associated with professional-council handling, duplicate member declarations, declaration-order identity, SHA-256 branding, and an originated source twin;
- `material` — marks a document as a settled source/material document;
- `memory` — marks a document as a memory document;
- `determine` — marks a document as open to further processing or completion;
- `object` — marks object standing in the document catalogue, with its reified standing specified from syntax 2.8;
- `field` — marks the ground of relation in which objects stand;
- `pure` and `gasoline` — syntax 3.0 declarative markers concerning the document's stated standing by its own honesty.
- `spark` — syntax 3.0 marker that indicates Java style declarative: the document declares itself SLeeLa constructs written in Java style, supported at the interpreter/VM level (see `SLEELA.syntax` section 43).

These words are contextual: outside their defined document-marker position they remain ordinary identifiers. Do not treat a marker as authorization to execute a program or as proof of external legal status.

The `prestige` marker described in the syntax document is a **future proposal**, not an implemented 3.0 feature.

## 11. Metadata, annotations, and supporter records

The Metadata Class standard gives class-bearing source units a descriptive metadata record for provenance, legality, and jurisdictional scope. Metadata describes a unit; it does not grant execution authority.

The compiler also carries source annotations through its annotation pipeline. Syntax 2.6 adds keyword supporter records for selected control-flow keywords (`try`, `catch`, `throw`, `synchronized`, and `super`), linking each occurrence to a stable hash and a corresponding support-document location. This documentation mechanism does not change the underlying keyword's control-flow semantics.

Syntax 3.0 also documents `@modium` and `@registry` subsystem-role annotations. These are labels for the Modium/Registry responsibility split, not new grammar or execution privileges.

## 12. Standard library and package model

The `/lib` tree is the unified SLeeLa source layer for the language-facing SDK and VM/OS object model. Packages are recursively discovered by the compiler and Nordshrift loader; adding a package does not require a separate hard-coded compiler allow-list.

The library spans foundational and application-facing areas, including:

- core runtime, collections, text, math, and data structures;
- file systems, streams, process, memory, and operating-system services;
- networking, HTTP, Wi-Fi, telephony, and server components;
- cryptography, security, codecs, XML, regular expressions, and databases;
- user interfaces, GUI, terminal, graphics, and CAD-related models;
- CPU, VM, opcode, compiler, decompiler, reflection, and debugging tools;
- AI-facing packages for large language models, neural networks, and related interfaces;
- documentation, source-sheet, registry, and structured-document facilities.

The package catalogue is extensive and changes as the library develops. The canonical class vocabulary is [`lib/LIBRARY.SYMBOLS.md`](lib/LIBRARY.SYMBOLS.md); the package overview is [`lib/README.md`](lib/README.md); generated class/member documentation is available at [`sleeladoc-api/index.html`](sleeladoc-api/index.html).

### 12.1 Cross-platform model

The project targets **Linux, macOS, and Windows 10+**. Pure SLeeLa library code is portable by construction. Facilities requiring host services cross an explicit VM/OS bridge, whose native backend resolves to the host platform. Platform-specific differences are exposed through OS-specific classes rather than silently treated as identical.

A platform-support statement is not a claim that every optional device, driver, or external dependency is present on every machine. Consult [`docs/PLATFORM-SUPPORT.md`](docs/PLATFORM-SUPPORT.md) and the relevant package README for requirements.

### 12.2 Explicit native boundary

C and C++ remain below explicit VM/OS bridge boundaries for operations requiring kernel, device, memory, networking, cryptography, or process primitives. The source library expresses the SLeeLa-facing contracts; native implementations provide the host-specific service where required.

## 13. Compiler, runtime, and virtual machines

The documented compilation path is:

```text
.sleela Wrapper™
      |
      v
syntax-version resolution
      |
      v
lexer -> parser -> AST / annotations
      |
      v
semantic analysis and code generation
      |
      v
SLeeLa Core bytecode
      |
      v
native C SLVM / runtime
```

The compiler's principal stages are version resolution, lexing, parsing, semantic analysis, and code generation. The C core provides the value model, operand stack, call frames, and opcode dispatch. The front end builds and runs through the core's exchange interface.

SLeeLa also documents a family of VM and runtime services, including memory management, garbage collection, instruction dispatch, system bridges, and VM selection/inspection tooling. The Native VM is the recommended starting point in the user guide. VM generations and premium profiles must be evaluated against their own implementation and validation records; a specification or profile name alone is not proof of complete runtime support.

See [`markdown/COMPILER.md`](markdown/COMPILER.md), [`virtual-machine/USER-GUIDE.md`](virtual-machine/USER-GUIDE.md), and [`docs/ARCHITECTURE.md`](docs/ARCHITECTURE.md).

## 14. Documentation and developer tools

SLeeLa includes a documentation system designed to document intent as well as signatures.

**SleelaDoc** uses structured angle-bracket doc comments with `@doc` / `@page` sentinels and tags such as `@intent`, `@thesis`, `@axiom`, `@param`, `@see`, and `@mastery`. The extractor renders Markdown or HTML API pages. The generated site organizes the library by package, class, and member.

The command-line documentation interface supports topic and class lookup, case-insensitive resolution, suggestions for near misses, offline generated corpora, and facade-topic filesystem discovery within documented limits.

Other developer-facing facilities include:

- `sleela check` for source validation without execution;
- `sleela run` for compile-and-run;
- `sleela version` for toolchain and supported syntax information;
- `sleela docs` for documentation lookup;
- Nordshrift for declarative `.sst` build sheets and recursive source discovery;
- generated API sites and immutable versioned API snapshots;
- local project generation and standalone utility programs.

See [`lib/sleeladoc/README.md`](lib/sleeladoc/README.md), [`manifesto/SLEELADOC.md`](manifesto/SLEELADOC.md), and [`api/sleela/3.0/SESSION-ADDITIONS-API.md`](api/sleela/3.0/SESSION-ADDITIONS-API.md).

## 15. Build and try SLeeLa

A typical source file:

```sleela
#sleela 3.0

class Hello {
    void main() {
        String greeting = "Hello";
        int answer = 40 + 2;

        print(greeting + ", International SLeeLa " + answer);
    }
}
```

Build and run from a configured checkout:

```sh
make
./build/sleela version
./build/sleela check path/to/Hello.sleela
./build/sleela run path/to/Hello.sleela
```

For the native core alone, see the instructions in [`impl/README.md`](impl/README.md). The repository root build may build additional products beyond the compiler/runtime. Use [`markdown/TESTING.md`](markdown/TESTING.md), the test-suite guide, and the live GitHub Actions results to assess the exact checkout.

## 16. Implementation and conformance boundaries

SLeeLa is a broad language-and-runtime project. Its documentation should distinguish:

1. **Normative syntax** — what the language specification defines.
2. **Compiler acceptance** — what the current front end parses and validates.
3. **Runtime behavior** — what the Core/VM actually executes.
4. **Library availability** — what source classes and native bridges are present.
5. **Verified conformance** — what the current tests and CI have demonstrated.

Important qualifications in the current references include:

- Syntax 3.0 is the current accepted grammar ceiling, not an automatic enablement of every historical or proposed feature.
- Constructor overload resolution and `this(...)` / base-constructor delegation are not verified and are rejected explicitly.
- `prestige` remains a proposal.
- `U<n>`, `C<n>`, and the named wide-integer families have distinct documented meanings and implementation boundaries; do not infer universal end-to-end support from their grammar or type recognition.
- Optional native bridges, devices, network services, and platform integrations require their own build and runtime tests.
- A passing historical CI run does not establish the status of the current commit. Use the live Actions page and current test logs.

This separation is intentional: SLeeLa aims to be ambitious in its language and system model while keeping implementation claims traceable to code and tests.

## 17. Primary references

- [Normative language syntax — `SLEELA.syntax`](SLEELA.syntax)
- [Compiler pipeline and versioning](markdown/COMPILER.md)
- [Wrapper™ source-file contract](markdown/SOURCE.md)
- [Syntax version matrix](api/sleela/2.1/language/SYNTAX-VERSION-MATRIX.md)
- [Class inheritance and document grouping](markdown/EXTENDS.md)
- [Standard library overview](lib/README.md)
- [Canonical library symbols](lib/LIBRARY.SYMBOLS.md)
- [Generated SleelaDoc API](sleeladoc-api/index.html)
- [Platform support](docs/PLATFORM-SUPPORT.md)
- [Virtual-machine user guide](virtual-machine/USER-GUIDE.md)
- [Testing policy](markdown/TESTING.md)
- [GitHub Actions](https://github.com/mearvk/SLeeLa/actions)

---

**SLeeLa International 3.0**  
*The Now. The Ingredient.*
